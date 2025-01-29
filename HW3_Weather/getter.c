#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <curl/curl.h>
#include "getter.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory! */
    printf("ошибка памяти\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

char *get_data(const char *url)
{
CURL *curl_handle;
  CURLcode res;
 
  struct MemoryStruct chunk;
 
  chunk.memory = malloc(1);
  chunk.size = 0;
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  curl_handle = curl_easy_init();
 
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 
  res = curl_easy_perform(curl_handle);
 
  /* check for errors */
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  curl_easy_cleanup(curl_handle);
  curl_global_cleanup();
 
  return chunk.memory;
}

char *get_url(const char *city)
{
    char *url = malloc(strlen(city) + 28);
    if (url==NULL)
    {
        return NULL;
    }
    sprintf(url, "https://wttr.in/%s?format=j1", city);
    return url;
}
