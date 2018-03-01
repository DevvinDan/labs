#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "string.h"
#include <signal.h>
#include <math.h>
#include <stdio.h>

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


int main(){

	char buf[BUF_SIZE];

	int pipefd[2];
	if (pipe(pipefd) == -1){
		printf("Error creating pipe\n");
		return 1;
	}

	pid_t child = fork();
	
	if (child == 0){
		close(pipefd[0]); // close read end of the pipe
		for (int i = 0; i < 100; i++){
			sprintf(buf, "Message number: %d Sender PID: %d Time: %d", (i+1), getpid(), getCurrentTime());
			write(pipefd[1], buf, BUF_SIZE);
		}
		close(pipefd[1]); // reader will get EOF
		exit(0);

	} else if (child > 0) {
		close(pipefd[1]);
		while (read(pipefd[0], buf, BUF_SIZE) > 0){
			printf("Reader PID: %d || %s\n", getpid(), buf);
		}
		close(pipefd[0]);
		exit(0);

	} else {
		printf("Error creating process\n");
		return 1;
	}

	

}
