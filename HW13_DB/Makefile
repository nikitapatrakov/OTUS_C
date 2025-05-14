# Устанавливаем имя программы
TARGET = stats_program

# Устанавливаем исходный файл
SRC = main.c

# Устанавливаем компилятор
CC = gcc

# Используем pkg-config для получения флагов компилятора и линковщика для libpq
CFLAGS = $(shell pkg-config --cflags libpq) -Wall -Wextra -lm
LDFLAGS = $(shell pkg-config --libs libpq)

# Правила для сборки программы
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# Очистка сгенерированных файлов
clean:
	rm -f $(TARGET)

# Правила для запуска программы (опционально)
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
