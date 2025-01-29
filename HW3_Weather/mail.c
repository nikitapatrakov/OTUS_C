#include <stdio.h>
#include <stdlib.h>

#include "getter.h"
#include "parser.h"
 
int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Не верные параметры");
    return 1;
  }
  char *url = get_url(argv[1]);
  if (url==NULL)
  {
    printf("Не удалось сгенерировать ссылку");
    return 1;
  }
  char *resp = get_data(url);

  free(url);

  parse_weather_json(resp);


  return 0;
}
