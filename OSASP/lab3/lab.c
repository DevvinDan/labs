#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>
#include "dirent.h"
#include "string.h"

#define bool char
#define true 1
#define false 0
#define BUF_SIZE 1024
#define MAX_FILES 100
#define FILE_NAME_SIZE 500


bool compareFiles(char *filePath1, char *filePath2, int *totalBytes){

    FILE *file1 = fopen(filePath1, "r");
    FILE *file2 = fopen(filePath2, "r");

    *totalBytes = 0;

    while (!(feof(file1) && (feof(file2)))){
        if (feof(file1) || feof(file2)){
            fclose(file1);
            fclose(file2);
            return false;
        }
        int c1 = fgetc(file1);
        int c2 = fgetc(file2);
        *totalBytes++;
        if (c1 != c2){
            fclose(file1);
            fclose(file2);
            return false;
        }
    }

    fclose(file1);
    fclose(file2);

    return true;

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
    int maxProcesses = atoi(argv[3]);

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

    int runningProcesses = 0;

    for (int i = 0; i < filesCount1; i++){
        for (int j = 0; j < filesCount2; j++){
            pid_t pid = fork();
            if (pid == 0){
                int totalBytes = 0;
                bool equal = compareFiles(files1[i], files2[j], &totalBytes);
                printf("PID: %d PPID: %d COMPARING: %s %s BYTES COMPARED: %d RESULT: %s", getpid(), getppid(), files1[i], files2[j], totalBytes, equal ? "TRUE" : "FALSE");
                exit(0);
            } else if (pid > 0){
                runningProcesses++;
                if (runningProcesses >= maxProcesses){
                    pid_t terminatedChild = wait(NULL);
                    printf("Child with PID %ld successfully terminated\n", (long) pid);
                    runningProcesses--;
                }
            } else {
                error("Error creating a process");
            }
        }
    }


}
