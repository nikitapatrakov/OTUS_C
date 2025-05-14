#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <math.h>
#include <string.h>

void calculate_statistics(PGconn* conn, const char* table, const char* column) {
    char query[512];
    snprintf(query, sizeof(query), "SELECT %s FROM %s;", column, table);
    
    PGresult* res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Ошибка выполнения запроса: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    if (rows == 0) {
        printf("Таблица пуста\n");
        PQclear(res);
        return;
    }

    double sum = 0;
    double max = -INFINITY;
    double min = INFINITY;
    double mean = 0;
    double variance = 0;
    double count = 0;

    // Проходим по всем строкам таблицы
    for (int i = 0; i < rows; i++) {
        char* value = PQgetvalue(res, i, 0);
        
        // Пропускаем строки с пустыми значениями
        if (value == NULL || strlen(value) == 0) {
            continue;
        }

        char* endptr;
        double num = strtod(value, &endptr);

        // Если это не число, пропускаем строку
        if (*endptr != '\0') {
            printf("Невозможно преобразовать значение в числе: %s\n", value);
            continue;
        }

        sum += num;
        if (num > max) max = num;
        if (num < min) min = num;

        count++;
    }

    if (count == 0) {
        printf("Нет числовых данных для расчета статистики.\n");
        PQclear(res);
        return;
    }

    mean = sum / count;

    // Вычисляем дисперсию
    for (int i = 0; i < rows; i++) {
        char* value = PQgetvalue(res, i, 0);
        
        if (value == NULL || strlen(value) == 0) {
            continue;
        }

        char* endptr;
        double num = strtod(value, &endptr);

        if (*endptr != '\0') continue;

        variance += pow(num - mean, 2);
    }
    variance /= count;

    printf("Среднее: %.2f\n", mean);
    printf("Максимум: %.2f\n", max);
    printf("Минимум: %.2f\n", min);
    printf("Дисперсия: %.2f\n", variance);
    printf("Сумма: %.2f\n", sum);

    PQclear(res);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Использование: %s <dbname> <table> <column>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* dbname = argv[1];
    const char* table = argv[2];
    const char* column = argv[3];

    // Подключаемся к базе данных с использованием dbname
    char conninfo[256];
    snprintf(conninfo, sizeof(conninfo), "host=localhost port=5432 user=postgres password=1234 dbname=%s", dbname);
    PGconn* conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Ошибка подключения: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return EXIT_FAILURE;
    }

    // Вычисление статистики для заданной таблицы и колонки
    calculate_statistics(conn, table, column);

    PQfinish(conn);
    return EXIT_SUCCESS;
}
