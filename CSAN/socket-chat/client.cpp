#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <strings.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <pthread.h>

#define BUF_SIZE 1024

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){

    char buffer[BUF_SIZE];

    int server; // Descriptors
    int port;
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

    if (connect(server, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){
        error("Error connecting to server");
    }

    cout << "Connected to the server..." << endl;

    bool connection = true;

    while (connection){

        cout << "Please input message: " << endl;

        fgets(buffer, BUF_SIZE, stdin);
	
	sprintf(buffer, "UserName: %s", buffer);

        int bytesNumber = write(server, buffer, BUF_SIZE);

        if (bytesNumber < 0){
            error("Can't write to socket");
        }

        bytesNumber = read(server, buffer, BUF_SIZE);

        if (bytesNumber < 0){
            error("Error reading from socket");
        }

        cout << "Server: " << buffer << endl;


    }

    return 0;

}
