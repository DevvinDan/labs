#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "string.h"
#include <math.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h> 

#define bool char
#define true 1
#define false 0
#define BUF_SIZE 256
#define NUMBER_OF_SEMAPHORES 4

/* gcc -lm -lrt */

pid_t process1, process2, process3, process4;


int getCurrentTime(){

	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	int timeInMilliseconds = round(ts.tv_nsec/1000000);
	return timeInMilliseconds;
	
}

int main(int argc, char *argv[]){

	char buf[BUF_SIZE];


	int semaphore = semget(IPC_PRIVATE, 4, 0666 | IPC_CREAT);

	if (semaphore == -1){
		printf("Error creating semaphore\n");
		return -1;
	}	

	union semun {

		int val;
		struct semid_ds *buf;
		unsigned short *array;
		struct seminfo *__buf;

	} arguments;

	arguments.val = 0;

	for (int i = 0; i < NUMBER_OF_SEMAPHORES; i++){
		if (semctl(semaphore, i, SETVAL, arguments) == -1 ){
			printf("Error setting value to a semaphore\n");
			return -1;
		}
	}

	
	int sharedMemory = shm_open("/sharedMemory", O_CREAT | O_RDWR, 0666);
	if (sharedMemory == -1){
		printf("Error creating shared memory\n");
		return -1;
	} else {
		printf("Shared memory created...\n");
	}


	char *mapArea = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemory, 0);

	if (mapArea == MAP_FAILED){
		printf("Error mmapping\n");
		return -1;
	} else {
		printf("Successfully mmaped\n");
	}

	ftruncate(sharedMemory, BUF_SIZE);


	struct sembuf semaphoreIncrement[1];
	semaphoreIncrement[0].sem_op = 1;
	semaphoreIncrement[0].sem_flg = 0;

	struct sembuf semaphoreDecrement[1];
	semaphoreDecrement[0].sem_op = -1;
	semaphoreDecrement[0].sem_flg = 0;


	process1 = fork();
	

	if (process1 == 0){
		// inside the process1

		semaphoreIncrement[0].sem_num = 1;
		semaphoreDecrement[0].sem_num = 0;

		process2 = fork();
		if (process2 == 0){
			// inside the process2

			semaphoreIncrement[0].sem_num = 2;
			semaphoreDecrement[0].sem_num = 1;
		
			process3 = fork();
			if (process3 == 0){
				// inside the process3

				semaphoreIncrement[0].sem_num = 3;
				semaphoreDecrement[0].sem_num = 2;

				process4 = fork();
				if (process4 == 0){
					// inside the process4
					semaphoreIncrement[0].sem_num = 0;
					semaphoreDecrement[0].sem_num = 3;

					// Reading lines written by process3, write lines to process1, increments porcess1's semaphore value to allow reading 
					while (1){
						if (semop(semaphore, semaphoreDecrement, 1) == -1){
							printf("Error decrementing semaphore #4\n");
							return -1;
						}
		
						printf("Inside the process4:\n");
						printf("%s", mapArea);
		
						sprintf(mapArea, "4 PID: %d PPID: %d Time: %d process4\n", getpid(), getppid(), getCurrentTime());

						usleep(100000);
						if (semop(semaphore, semaphoreIncrement, 1) == -1){
							printf("Error incrementing semaphore #4\n");
							return -1;
						}
		
					}


				} else if (process4 > 0){
					// outside of the process4

					// Reading lines written by process2, write lines to process4, increments porcess4's semaphore value to allow reading 
					while (1){
						if (semop(semaphore, semaphoreDecrement, 1) == -1){
							printf("Error decrementing semaphore #3\n");
							return -1;
						}
		
						printf("Inside the process3:\n");
	
						printf("%s", mapArea);
		
						sprintf(mapArea, "3 PID: %d PPID: %d Time: %d process3\n", getpid(), getppid(), getCurrentTime());
	
						usleep(100000);
						if (semop(semaphore, semaphoreIncrement, 1) == -1){
							printf("Error incrementing semaphore #4\n");
							return -1;
						}
		
					}


				} else {
					printf("Error creating a process\n");
					return -1;
				}


			} else if (process3 > 0){
				// outside of the process3

				// Reading lines written by process1, write lines to process3, increments porcess3's semaphore value to allow reading 
				while (1){
					if (semop(semaphore, semaphoreDecrement, 1) == -1){
						printf("Error decrementing semaphore #2\n");
						return -1;
					}
	
					printf("Inside the process2:\n");

					printf("%s", mapArea);

					sprintf(mapArea, "2 PID: %d PPID: %d Time: %d process2\n", getpid(), getppid(), getCurrentTime());

					usleep(100000);
					if (semop(semaphore, semaphoreIncrement, 1) == -1){
						printf("Error incrementing semaphore #3\n");
						return -1;
					}

				}


			} else {
				printf("Error creating a process\n");
				return -1;
			}



		} else if (process2 > 0){
			// outside of the process2

			// Reading lines written by process4, write lines to process2, increments porcess2's semaphore value to allow reading 
			while (1){
				if (semop(semaphore, semaphoreDecrement, 1) == -1){
					printf("Error decrementing semaphore #1\n");
					return -1;
				}

				printf("Inside the process1:\n");

				printf("%s", mapArea);
				
				sprintf(mapArea, "1 PID: %d PPID: %d Time: %d process1\n", getpid(), getppid(), getCurrentTime());
				
				usleep(100000);
				if (semop(semaphore, semaphoreIncrement, 1) == -1){
					printf("Error incrementing semaphore #2\n");
					return -1;
				}

			}

		} else {
			printf("Error creating a process\n");
			return -1;
		}
		
	} else if (process1 > 0){
		
		semaphoreIncrement[0].sem_num = 0;
		sprintf(mapArea, "Initialization...\n");
		if (semop(semaphore, semaphoreIncrement, 1) == -1){
			printf("Error incrementing semaphore #1\n");
			return -1;
		}

		while(1){
		}

	} else {
		printf("Error creating a process\n");
		return -1;
	}	

}
