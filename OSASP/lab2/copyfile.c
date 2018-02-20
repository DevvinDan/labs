#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


int main(int argc, char *argv[]){

	if (argc != 3){
		printf("Error. Wrong argument number.\n");
		return -1;
	}

	char *SRC =  argv[1];
	char *DEST = argv[2];
	FILE *read_ptr = fopen(SRC, "r");
	FILE *write_ptr = fopen(DEST, "w");

	if (read_ptr == NULL || write_ptr == NULL){
		printf("Cant open file. Error code: %d\n", errno);
		return -1;
	}

	struct stat fileStats;
	
	if (stat(SRC, &fileStats) < 0){
		printf("Error. Can't read file statistics. Error code: %d\n", errno);
		return -1;
	}
	
	if (chmod(DEST, fileStats.st_mode) != 0){
		printf("Error. Can't copy properties. Error code: %d\n", errno);
	}


	while (!feof(read_ptr)){

		char c = fgetc(read_ptr);
		fputc(c, write_ptr);

	}	

	if(fclose(read_ptr) || fclose(write_ptr)){
		printf("Error. Can't close the file(s)\n");
	}
	
	return 0;

}
