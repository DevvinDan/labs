#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <map>
#include <pthread.h>


#define BUF_SIZE 1024
#define CLIENT_TABLE_SIZE 50

using namespace std;

void error(const char *msg){
    perror(msg);
    exit(1);
}

void handleConnection(int socket){

    char buf[BUF_SIZE];
    int bytesRead;

    while ( (bytesRead = read(socket, buf, BUF_SIZE)) > 0){
        cout << "Client message is: " << buf << endl;
        write(socket, buf, BUF_SIZE);
        
    }

    cout << "Reading stopped." << endl;

    close(socket);

    return;

}

int main(int argc, char* argv[]) {

    int server, client; // Descriptors
    int port;
    char buf[BUF_SIZE];
    int id_table[CLIENT_TABLE_SIZE];


    struct sockaddr_in server_address, client_address;

    if (argc < 2){
        error("Error. No port provided");
    }

    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server < 0){
        error("Error establishing socket");
    }

    bzero((char *) &server_address, sizeof(server_address));

    port = atoi(argv[1]);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    if (bind(server, (struct sockaddr*)&server_address, sizeof(server_address)) < 0){
        error("Error. Can't bind a connection");
    }



    listen(server, 5);
    int addrLen = sizeof(struct sockaddr_in);
    while ( (client = accept(server, (struct sockaddr *) &client_address, (socklen_t *)&addrLen)) ){

        cout << "Connection accepted" << endl;
        const char *message = "Welcome!";

        handleConnection(client);

        cout << "Handle connection is over" << endl;
    }

    return 0;

}
