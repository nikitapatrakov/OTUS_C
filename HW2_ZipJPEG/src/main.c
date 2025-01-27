#include <stdio.h>
#include <sys/stat.h>
#include "utils.h"

int main(int argc, char** argv)
{
	if (argc<=1) 
	{
		printf("Не указан путь к файлу(ам)!!!\n");
		return 1;
	}

	FILE *file = fopen(argv[1], "rb");
	if (file==NULL)
	{
		printf(" Не возможно открыть файл %s", argv[1]);
		return 1;
	}

	struct stat file_stats;
	if (stat(argv[1], &file_stats) != 0)
	{	
		fclose(file);
		printf("Не возможно получить данные от файла %s", argv[1]);
		return 1;
	}

	off_t fileSize = (off_t)file_stats.st_size;

	if (!checkRarjpegFile(file, fileSize))
	{
		fclose(file);
		printf("В файле %s нет архива!!!!!", argv[1]);
		return 1;	
	}
	printCont(file, fileSize);
	fclose(file);
	return 0;
}