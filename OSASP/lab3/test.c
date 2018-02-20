#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>



int main(){
	
	int arr[3];
	int status;
	pid_t pid;
	pid = fork();
	if (pid == 0){
		arr[0] = getpid();
		printf("Inside child process, arr[0] = %d", arr[0]);
	} else {
		wait(&status);
		printf("Exited, arr[0] = %d", arr[0]);
	}

}
