#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char *argv[]){

	if (argc != 2){
		printf("Error. Wrong argument number.\n");
		return -1;
	}

	char *FILENAME =  argv[1];
	FILE *fp = fopen(FILENAME, "w");

	if (fp == NULL){
		printf("Cant open file. Error code: %d\n", errno);
		return -1;
	}
	
	printf("Enter a string: \n");

	char c = getc(stdin);
	while (c != '\n'){
		fputc(c, fp);
		c = getc(stdin);
	}

	if (fclose(fp)){
		printf("Error. Can't close file\n");
		return -1;
	}
	
	return 0;

}
