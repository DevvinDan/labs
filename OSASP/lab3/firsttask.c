#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <wait.h>
#include <stdlib.h>

#define bool char
#define true 1
#define false 0
#define BUF_SIZE 100


int main(){
	
	int arr[3];
	int status;
	pid_t child1, child2;
	child1 = fork();
	if (child1 == 0){

		// Iside the child process

		
	} else {

		// Parent process
		
		child2 = fork();
		
	}

	time_t timeNow = time(NULL); 
	struct tm *time = localtime(&timeNow);
	
	printf("PID: %d Parent PID: %d || TIME: %d: %d: %d\n", getpid(), getppid(), time->tm_hour, time->tm_min, time->tm_sec);	
	system("ps -x");

}
