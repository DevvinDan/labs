#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#define __USE_XOPEN
#include <time.h>
#include <string.h>


char* formatdate(char* str, time_t val){
        strftime(str, 36, "%d.%m.%Y", localtime(&val));
        return str;
}

void readDirectory(char *path, int min, int max, time_t left, time_t right, FILE *logs){
	
	DIR *directory = opendir(path);
	if (directory == NULL){
		printf("Can't open directory. Error code: %d\n", errno);
		return;
	}

	struct dirent *content;
	struct stat fileStats;
	strcat(path, "/");
	while ((content = readdir(directory)) != NULL){
		char filePath[100];
		strcpy(filePath, path);
		strcat(filePath, content->d_name);
		if (stat(filePath, &fileStats) != 0){
			printf("Error. Can't read statistics.\n");
			return;
		}
		if (content->d_type == DT_DIR){
			if ( strcmp(content->d_name, ".") && strcmp(content->d_name, "..") ){
				readDirectory(filePath, min, max, left, right, logs);
			}
		} else {
			char date[36];
			formatdate(date, fileStats.st_ctime);
			if ( !(fileStats.st_size < min || fileStats.st_size > max) && (difftime(fileStats.st_ctime, left) >= 0) && (difftime(fileStats.st_ctime, right) < 0) ){
				printf("Name: %s\nSize: %lu\nDate: %s\n\n", content->d_name, fileStats.st_size, date);
				fprintf(logs, "Name: %s\nSize: %lu\nDate: %s\n\n", content->d_name, fileStats.st_size, date);
			}
		}
	}

}

int main(int argc, char *argv[]){
	
	if (argc != 7){
		printf("Error. Wrong number of arguments\n");
		return -1;
	}
	char *catalogName = argv[1];
	char *fileName = argv[2];
	int minSize = atoi(argv[3]);
	int maxSize = atoi(argv[4]);
	char *minDate = argv[5];
	char *maxDate = argv[6];	

	FILE *write_file = fopen(fileName, "w");
	if (write_file == NULL){
		printf("Error. Can't open file.\n");
		return -1;
	}	
	

	struct tm timeStruct;
	strptime(minDate, "%d.%m.%Y", &timeStruct);
	time_t dateMin = mktime(&timeStruct);
	strptime(maxDate, "%d.%m.%Y", &timeStruct);
	time_t dateMax = mktime(&timeStruct);
	struct stat fileStats;
	readDirectory(catalogName, minSize, maxSize, dateMin, dateMax, write_file);
	
	if (fclose(write_file)){
		printf("Error. Can't close file.\n");
	}
	
	return 0;

}
