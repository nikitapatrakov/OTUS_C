#include "parser.h"
#include<json-c/json.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int parse_weather_json(char* buffer)
{
    struct json_object *data = json_tokener_parse(buffer);
    
    struct json_object* tokens = NULL;


    return 0;
}