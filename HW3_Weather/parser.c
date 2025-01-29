#include "parser.h"
#include<json-c/json.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int parse_weather_json(char* buffer)
{
    struct json_object *data = json_tokener_parse(buffer);
    
    struct json_object* tokens = NULL;

    json_object_object_get_ex(data, "current_condition", &tokens);

    struct json_object* buf = NULL;

    struct json_object* arr_item = json_object_array_get_idx(tokens, 0);

    json_object_object_get_ex(arr_item, "FeelsLikeC", &buf);

    printf("Температура воздуха: %.1f C\n", json_object_get_double(buf));

    json_object_object_get_ex(arr_item, "windspeedKmph", &buf);

    printf("Скорость ветра: %.1f км.ч\n", json_object_get_double(buf));

    json_object_object_get_ex(arr_item, "winddir16Point", &buf);

    printf("Направление ветра: %s\n", json_object_to_json_string(buf));

    return 0;
}