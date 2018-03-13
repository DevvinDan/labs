#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "string.h"
#include <signal.h>
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define bool char
#define true 1
#define false 0
#define BUF_SIZE 256


pid_t process1, process2, process3, wpid;


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


int main(){

	char buf[BUF_SIZE];

	int p1pipes[2], p2pipes[2], p3pipes[2], control1[2], control2[2], control3[2];

	pipe(p1pipes);	
	pipe(p2pipes);
	pipe(p3pipes);
	pipe(control1);
	pipe(control2);
	pipe(control3);

	bool notFinished = true;
	
	process1 = fork();
	if (process1 == 0){
		// inside the process1

		char data[BUF_SIZE];
		close(p1pipes[1]);
		close(control1[1]);
		sprintf(data, "[Process 1 data]: PID: %d PPID: %d", getpid(), getppid());

		do {

			write(p2pipes[1], data, BUF_SIZE);
			write(p3pipes[1], data, BUF_SIZE);
			sleep(1);
			for (int i = 0; i < 2; i++){
				read(p1pipes[0], buf, BUF_SIZE);
				printf("Process 1 got data: %s\n", buf);
			}
			read(control1[0], buf, BUF_SIZE);
			if (!strcmp(buf, "STOP")){
				notFinished = false;
			}

		} while(notFinished);

		printf("DONE process1\n");

		close(p1pipes[0]);
		exit(0);


	} else {
	
	process2 = fork();
	if (process2 == 0){
		// inside the process2

		char data[BUF_SIZE];
		close(p2pipes[1]);
		close(control2[1]);
		sprintf(data, "[Process 2 data]: PID: %d PPID: %d", getpid(), getppid());

		do {

			write(p1pipes[1], data, BUF_SIZE);
			write(p3pipes[1], data, BUF_SIZE);
			sleep(1);
			for (int i = 0; i < 2; i++){
				read(p2pipes[0], buf, BUF_SIZE);
				printf("Process 2 got data: %s\n", buf);
			}
			read(control2[0], buf, BUF_SIZE);
			if (!strcmp(buf, "STOP")){
				notFinished = false;
			}

		} while(notFinished);

		printf("DONE process2\n");

		close(p2pipes[0]);
		exit(0);


	} else {

	process3 = fork();
	if (process3 == 0){
		// inside the process3

		char data[BUF_SIZE];
		close(p3pipes[1]);
		close(control3[1]);
		sprintf(data, "[Process 3 data]: PID: %d PPID: %d", getpid(), getppid());

		do {

			write(p1pipes[1], data, BUF_SIZE);
			write(p2pipes[1], data, BUF_SIZE);
			sleep(1);
			for (int i = 0; i < 2; i++){
				read(p3pipes[0], buf, BUF_SIZE);
				printf("Process 3 got data: %s\n", buf);
			}
			read(control3[0], buf, BUF_SIZE);
			if (!strcmp(buf, "STOP")){
				notFinished = false;
			}

		} while(notFinished);

		printf("DONE process3\n");

		close(p3pipes[0]);
		exit(0);

	} else {
	}	
		int status = 0;
		// inside the control process
		printf("Control going to sleep...\n");
		strcpy(buf, "OK");
		for (int i = 0; i < 5; i++){
			write(control1[1], buf, BUF_SIZE);
			write(control2[1], buf, BUF_SIZE);
			write(control3[1], buf, BUF_SIZE);
			sleep(1);	
			if (i == 3){
				strcpy(buf, "STOP");
			}		
		}
		

		printf("Sent STOP signal...\n");
		while ((wpid = wait(&status)) > 0);
		exit(0);

	}



	}

	

}
