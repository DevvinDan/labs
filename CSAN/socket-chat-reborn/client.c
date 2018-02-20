#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "pthread.h"
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define USERNAME_SIZE 20
#define true 1
#define false 0
#define bool char

void error(char *msg)
{
    perror(msg);
    exit(0);
}

void *readMessages(void *ptr){

    char buffer[BUF_SIZE];
    int server = *((int *)ptr);

    while ( (read(server, buffer, BUF_SIZE)) > 0 ){

        printf("%s", buffer);

    }

}


int main(int argc, char *argv[]){

    char buffer[BUF_SIZE];
    char username[USERNAME_SIZE];

    int server, port, exit;
    struct sockaddr_in server_address;


    if (argc < 3){
        error("Wrong arguments number");
    }

    port = atoi(argv[2]);

    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server < 0){
        error("Error establishing socket");
    }

    bzero((char *) &server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(argv[1]);

    // Connecting to a server

    if (connect(server, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){
        error("Error connecting to server");
    }

    printf("Connected to the server.\n");

    // Asks user to enter username, sends it to server

    printf("Enter your username (%d chars max): ", USERNAME_SIZE);
    bool correctUsername = false;

    while (!correctUsername){
        fgets(username, USERNAME_SIZE, stdin);
        if (strlen(username) > 0){

            char *pos;
            if ((pos = strchr(username, '\n')) != NULL){
                *pos = '\0';
            }
            correctUsername = true;
        } else {
            printf("Please, input valid username: ");
        }
    }

    write(server, username, USERNAME_SIZE);


    // Creating a thread to asynchronously read messages from server

    pthread_t readingThread;

    if ( (pthread_create(&readingThread, NULL, &readMessages, (void *) &server)) ){

        error("Error creating thread.");

    }

    exit = 0;

    printf("Input your messages now: \n");

    while (!exit){

        fgets(buffer, BUF_SIZE, stdin);

        // Leaving char if user enters /exit

        if (!(strcmp(buffer, "/exit"))){
            printf("User typed /exit\n");
            exit = 1;
        }

        if ( (write(server, buffer, BUF_SIZE)) < 0) {
            error("Error writing to socket");
        }

    }

    return 0;

}


