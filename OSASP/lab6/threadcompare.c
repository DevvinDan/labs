#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <wait.h>
#include <pthread.h>
#include <stdlib.h>
#include "dirent.h"
#include "string.h"

#define bool char
#define true 1
#define false 0
#define BUF_SIZE 1024
#define MAX_FILES 100
#define FILE_NAME_SIZE 500

typedef struct arguments {
    char filePath1[FILE_NAME_SIZE];
    char filePath2[FILE_NAME_SIZE];
} arguments;


int runningThreads = 0;

void *compareFiles(void *args){

    arguments *structPointer = (arguments *) args;            

    int totalBytes = 0;
    bool equal = true;

    FILE *file1 = fopen(structPointer->filePath1, "r");
    FILE *file2 = fopen(structPointer->filePath2, "r");

    if (file1 == NULL || file2 == NULL){
	printf("Error opening file\n");
    }

    while (!(feof(file1) && (feof(file2)))){
        if (feof(file1) || feof(file2)){
            equal = false;
            break;
        }
        int c1 = fgetc(file1);
        int c2 = fgetc(file2);
        totalBytes += 1;
        if (c1 != c2){
            equal = false;
            break;
        }
    }

    printf("Thread ID: %lu COMPARING: %s %s BYTES COMPARED: %d RESULT: %s\n", pthread_self(), structPointer->filePath1, structPointer->filePath2, totalBytes, equal ? "EQUAL" : "NOT EQUAL");

    fclose(file1);
    fclose(file2);
    
    printf("Thread %lu successfully terminated\n", pthread_self());    

    runningThreads--;

}

void error(char *msg){
    perror(msg);
    exit(1);
}

void main(int argc, char *argv[]){

    if (argc < 4){
        error("Enter correct number of arguments");
    }

    char *dir1 = argv[1];
    char *dir2 = argv[2];
    int maxThreads = atoi(argv[3]);

    char files1[MAX_FILES][FILE_NAME_SIZE];
    char files2[MAX_FILES][FILE_NAME_SIZE];


    DIR *firstDir = opendir(dir1);
    DIR *secondDir = opendir(dir2);

    if ((firstDir == NULL) || (secondDir == NULL)){
        error("Can't open directory");
    }

    struct dirent *fdFileInfo, *sdFileInfo;

    int filesCount1 = 0;

    while ((fdFileInfo = readdir(firstDir)) != NULL){

        if (fdFileInfo->d_type == DT_REG){

            strcpy(files1[filesCount1], dir1);
            strcat(files1[filesCount1], "/");
            strcat(files1[filesCount1], fdFileInfo->d_name);
            filesCount1++;

        }

    }

    int filesCount2 = 0;

    while ((sdFileInfo = readdir(secondDir)) != NULL){

        if (sdFileInfo->d_type == DT_REG){

            strcpy(files2[filesCount2], dir2);
            strcat(files2[filesCount2], "/");
            strcat(files2[filesCount2], sdFileInfo->d_name);
            filesCount2++;

        }

    }

	int filesSum = filesCount1 * filesCount2;
	arguments args[filesSum];
	int countPointer = 0;

    for (int i = 0; i < filesCount1; i++){
        for (int j = 0; j < filesCount2; j++){

	        strcpy(args[countPointer].filePath1, files1[i]);
	        strcpy(args[countPointer].filePath2, files2[j]);

            while (runningThreads >= maxThreads){
                // wait until one of the threads ends
            }

	   	    runningThreads++;
	        pthread_t newThread;

            if ( (pthread_create(&newThread, NULL, &compareFiles, (void *) &args[countPointer])) ){
                printf("Error creating thread");
                return;
            }

	    	countPointer++;
        }
    }

	while (runningThreads != 0){
	}


}
