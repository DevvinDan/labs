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

pid_t process1, process2, process3, process4;

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


void process1_handler(int signo, siginfo_t *siginfo, void *context){
	messageNumber++;
    if (signo == SIGUSR1){
        printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS1 RECEIVED SIGUSR1 from PID: %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), siginfo->si_pid);
		usleep(100000);
		messageNumber++;
		printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS1 SEND SIGUSR1 TO PROCESS2 %d AND PROCESS3 %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), process2, process3);
		kill(process2, SIGUSR1);
		kill(process3, SIGUSR1);
    } else if (signo == SIGUSR2){
		printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS1 RECEIVED SIGUSR2 from PID: %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), siginfo->si_pid);
		usleep(100000);
		messageNumber++;
		printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS1 SEND SIGUSR2 TO PROCESS2 %d AND PROCESS3 %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), process2, process3);
		kill(process2, SIGUSR2);
		kill(process3, SIGUSR2);
	} else {
		printf("Unexpected error");
	}	
}

void process2_handler(int signo, siginfo_t *siginfo, void *context){
	messageNumber++;
    if (signo == SIGUSR1){
        printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS2 RECEIVED SIGUSR1 from PID: %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), siginfo->si_pid);
    } else if (signo == SIGUSR2){
		printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS2 RECEIVED SIGUSR2 from PID: %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), siginfo->si_pid);
	} else {
		printf("Unexpected error");
	}	
}

void process3_handler(int signo, siginfo_t *siginfo, void *context){
	messageNumber++;
    if (signo == SIGUSR1){
        printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS3 RECEIVED SIGUSR1 from PID: %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), siginfo->si_pid);
		usleep(100000);
		messageNumber++;
		printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS3 SEND SIGUSR1 TO PROCESS4 %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), process4);
		kill(process4, SIGUSR1);
    } else if (signo == SIGUSR2){
		printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS3 RECEIVED SIGUSR2 from PID: %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), siginfo->si_pid);
		usleep(100000);
		messageNumber++;
		printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS3 SEND SIGUSR2 TO PROCESS4 %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), process4);
		kill(process4, SIGUSR2);
	} else {
		printf("Unexpected error");
	}	
}

void process4_handler(int signo, siginfo_t *siginfo, void *context){
	messageNumber++;
    if (signo == SIGUSR1){
        printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS4 RECEIVED SIGUSR1 from PID: %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), siginfo->si_pid);
		usleep(100000);
		messageNumber++;
		printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS4 SEND SIGUSR2 TO PROCESS1 %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), process1);
		kill(process1, SIGUSR2);
    } else if (signo == SIGUSR2){
		printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS4 RECEIVED SIGUSR2 from PID: %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), siginfo->si_pid);
		usleep(100000);
		messageNumber++;
		printf("[%d] PID: %d PPID: %d TIME: %d || PROCESS4 SEND SIGUSR1 TO PROCESS1 %d\n", messageNumber, getpid(), getppid(), getCurrentTime(), process1);
		kill(process1, SIGUSR1);
	} else {
		printf("Unexpected error");
	}	
}


void main(){

	struct sigaction signal1, signal2, signal3, signal4;

	memset(&signal1, '\0', sizeof(signal1));
	signal1.sa_sigaction = &process1_handler;
	signal1.sa_flags = SA_SIGINFO;

	memset(&signal2, '\0', sizeof(signal2));
	signal2.sa_sigaction = &process2_handler;
	signal2.sa_flags = SA_SIGINFO;

	memset(&signal3, '\0', sizeof(signal3));
	signal3.sa_sigaction = &process3_handler;
	signal3.sa_flags = SA_SIGINFO;

	memset(&signal4, '\0', sizeof(signal4));
	signal4.sa_sigaction = &process4_handler;
	signal4.sa_flags = SA_SIGINFO;
	

	process1 = fork();
	if (process1 == 0){
		sigaction(SIGUSR1, &signal1, NULL);
		sigaction(SIGUSR2, &signal1, NULL);
	
		process1 = getpid();

		process2 = fork();
		
		if (process2 == 0){
			sigaction(SIGUSR1, &signal2, NULL);
			sigaction(SIGUSR2, &signal2, NULL);
			doSomething();

		} else if (process2 > 0){
			
			process3 = fork();

			if (process3 == 0){
				sigaction(SIGUSR1, &signal3, NULL);
				sigaction(SIGUSR2, &signal3, NULL);
				
				process4 = fork();

				if (process4 == 0){
					sigaction(SIGUSR1, &signal4, NULL);
					sigaction(SIGUSR2, &signal4, NULL);
					doSomething();
				} else if (process4 > 0){
					doSomething();
				} else {
					printf("Error creating process");
				}

			} else if (process3 > 0){
				
				printf("Wait for other processes to set signal handlers...\n");
				sleep(5);
				kill(process2, SIGUSR1);
				kill(process3, SIGUSR1);
				doSomething();
			} else {
				printf("Error creating process");
			}

		} else {
			printf("Error creating process");
		}

	} else if (process1 > 0){
		doSomething();
	} else {
		printf("Error creating process");
	}
		

}
