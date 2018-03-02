#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "string.h"
#include <signal.h>
#include <math.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define bool char
#define true 1
#define false 0
#define BUF_SIZE 256



int getCurrentTime(){

	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	int timeInMilliseconds = round(ts.tv_nsec/1000000);
	return timeInMilliseconds;
	
}

void doSomething(){
	while(1){	
	}
}


int main(int argc, char *argv[]){

	char buf[BUF_SIZE];
	char *filePath = argv[1];

	int semaphore = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);

	union semun {

		int val;
		struct semid_ds *buf;
		unsigned short *array;
		struct seminfo *__buf;

	} arguments;

	arguments.val = 0;


	if (semaphore == -1){
		printf("Error creating semaphore\n");
		return -1;
	}	

	if (semctl(semaphore, 0, SETVAL, arguments) == -1 ){
		printf("Error setting value to a semaphore\n");
		return -1;
	}

	pid_t child = fork();

	FILE* writeFile = fopen(filePath, "w");
	
	if (child == 0){		

		struct sembuf writeOps[1];
		writeOps[0].sem_num = 0;
		writeOps[0].sem_op = 1;
		writeOps[0].sem_flg = 0;

		for (int i = 0; i < 100; i++){

			sprintf(buf, "Message number: %d Sender PID: %d Time: %d\n", (i+1), getpid(), getCurrentTime());
			fputs(buf, writeFile);
			fflush(writeFile);
			// increase semaphore value by 1 

			if (semop(semaphore, writeOps, 1) == -1){
				printf("Error incrementing semaphore\n");
				return -1;
			}
			
		}

		fclose(writeFile);

	} else if (child > 0) {

		fclose(writeFile);
		FILE* readFile = fopen(filePath, "r");

		struct sembuf readOps[1];
		readOps[0].sem_num = 0;
		readOps[0].sem_op = -1;
		readOps[0].sem_flg = 0;

		for (int i = 0; i < 100; i++){

			// decrease semaphore value by 1
			if (semop(semaphore, readOps, 1) == -1){
				printf("Error decrementing semaphore\n");
				return -1;
			}

			fgets(buf, BUF_SIZE, readFile);
			printf("PID: %d || %s", getpid(), buf);
		
		}

		fclose(readFile);

	} else {
		printf("Error creating process\n");
		return 1;
	}

	

}
