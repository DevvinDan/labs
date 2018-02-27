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

int messageNumber = 0;

pid_t child1, child2;

int getCurrentTime(){

	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	int timeInMilliseconds = round(ts.tv_nsec/1000000);
	return timeInMilliseconds;
	
}

void doSomething(){
	while (1){
	}
}


void sig1handler(int signo){
	messageNumber++;
    if (signo == SIGUSR1){
        printf("[%d] PID: %d PPID: %d TIME: %d || RECIEVED SIGUSR1\n", messageNumber, getpid(), getppid(), getCurrentTime());
    } else {
		printf("Some wrong signal\n");
	}
	messageNumber++;
    printf("[%d] PID: %d PPID: %d TIME: %d || SEND SIGUSR2\n", messageNumber, getpid(), getppid(), getCurrentTime());
	kill(getppid(), SIGUSR2);
}

void sig2handler(int signo, siginfo_t *siginfo, void *context){
	messageNumber++;
    if (signo == SIGUSR2){
        printf("[%d] PID: %d PPID: %d TIME: %d || RECIEVED SIGUSR2 from PID: %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), siginfo->si_pid);
    } else {
		printf("Some wrong signal\n");
	}
	usleep(100000);
	messageNumber++;
    printf("[%d] PID: %d PPID: %d TIME: %d || SEND SIGUSR1\n", messageNumber, getpid(), getppid(), getCurrentTime());
	kill(child1, SIGUSR1);
	kill(child2, SIGUSR1);
}

void main(){

	struct sigaction signal1, signal2;
	memset(&signal1, '\0', sizeof(signal1));
	memset(&signal2, '\0', sizeof(signal2));

	signal1.sa_handler = &sig1handler;
	signal2.sa_sigaction = &sig2handler;
	signal2.sa_flags = SA_SIGINFO;

	sigaction(SIGUSR1, &signal1, NULL);
	sigaction(SIGUSR2, &signal2, NULL);
	

	child1 = fork();
	if (child1 == 0){
		doSomething();
	} else {
		child2 = fork();
		if (child2 == 0){
			doSomething();
		} else {
			kill(child1, SIGUSR1);
			kill(child2, SIGUSR1);
			doSomething();
		}
	}
		

}
