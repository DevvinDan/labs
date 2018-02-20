#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "pthread.h"

#define BUF_SIZE 1024
#define USER_TABLE_SIZE 50
#define USERNAME_SIZE 20
#define true 1
#define false 0
#define bool char

typedef struct arguments {
    int socketDescriptor;
    int *userTable;
    char *username;
} arguments;


// Sending message to each user in the table

void broadcastMessage(int *table, char *message){
    for (int i = 0; i < USER_TABLE_SIZE; i++){
        if (table[i] != 0){
            write(table[i], message, strlen(message) + 1);
        }
    }
}

void *handleConnection(void *ptr){


    arguments *structPointer = (arguments *) ptr;
    char buffer[BUF_SIZE];
    char outputString[USERNAME_SIZE + BUF_SIZE];

    int client = structPointer->socketDescriptor;
    int *userTable = structPointer->userTable;
    char username[USERNAME_SIZE];
    strcpy(username, structPointer->username);

    while ( (read(client, buffer, BUF_SIZE)) > 0 ){

        // Adding username and sending message to each user in table
        sprintf(outputString, "%s: %s", username, buffer);
        broadcastMessage(userTable, outputString);

    }

    sprintf(outputString, "User disconnected: %s\n", username);
    printf("%s", outputString);
    broadcastMessage(userTable, outputString);

    // Removing user from global user table

    for (int i = 0; i < USER_TABLE_SIZE; i++){
        if (userTable[i] == client){
            userTable[i] = 0;
        }
    }

    // Closing connection

    close(client);


}

void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]) {

    int server, client; // Descriptors
    int port;
    int user_table[USER_TABLE_SIZE] = { 0 };


    struct sockaddr_in server_address, client_address;

    if (argc < 2){
        error("Error. No port provided");
    }

    // Creating socket using TCP/IP

    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server < 0){
        error("Error establishing socket");
    }

    // Binding socket on a server with specific address

    bzero((char *) &server_address, sizeof(server_address));

    port = atoi(argv[1]);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    if (bind(server, (struct sockaddr*)&server_address, sizeof(server_address)) < 0){
        error("Error. Can't bind a connection");
    }


    // Listening for connections...

    listen(server, 1);

    printf("Server is listening...\n");

    int addrLen = sizeof(struct sockaddr_in);

    while ( (client = accept(server, (struct sockaddr *) &client_address, (socklen_t *)&addrLen)) ){

        // Getting username and notifying other users

        char username[USERNAME_SIZE];
        read(client, username, USERNAME_SIZE);
        char alert[BUF_SIZE];
        sprintf(alert, "User connected: %s\n", username);
        printf("%s", alert);
        broadcastMessage(user_table, alert);

        // Adding client to global user table identifying with socket id

        for (int i = 0; i < USER_TABLE_SIZE; i++){
            if (user_table[i] == 0){
                user_table[i] = client;
                break;
            }
        }

        // Creating arguments structure to pass into new thread

        arguments userInfo;

        userInfo.userTable = user_table;
        userInfo.socketDescriptor = client;
        userInfo.username = username;

        // Creating new thread that handles user connection

        pthread_t userThread;

        if ( (pthread_create(&userThread, NULL, &handleConnection, (void *) &userInfo)) ){

            error("Error creating thread.");

        }

    }

    printf("Server stops working...\n");

    return 0;

}