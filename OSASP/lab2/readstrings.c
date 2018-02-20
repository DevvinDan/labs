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

	char *FILENAME =  argv[1];
	int stringGroupNumber = atoi(argv[2]);
	FILE *fp = fopen(FILENAME, "r");

	if (fp == NULL){
		printf("Cant open file. Error code: %d\n", errno);
		return -1;
	}
	
	char c;


	if (!stringGroupNumber){
		
		c = fgetc(fp);
		while (!feof(fp)){
			putc(c, stdout);
			c = fgetc(fp);
		}

	} else {

		while (!feof(fp)){
			for (int i = 0; i < stringGroupNumber; i++){
				if (!feof(fp)){
					c = fgetc(fp);
					while (c != '\n' && !feof(fp)){
						putc(c, stdout);
						c = fgetc(fp);
					}
					
					putc('\n', stdout);					
				}
			}
			c = getc(stdin);
		}

	}


	if (!fclose(fp)){
		printf("Error. Can't close file\n");
		return -1;
	}
	
	return 0;

}
