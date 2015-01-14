//Example server application

#include <sys/socket.h> //Socket features
#include <netinet/in.h> //Internet-specific features of sockets
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    //Open socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //Create server/client address structures:
    // Server needs to know both the address of itself and the client
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(9876);
    serveraddr.sin_addr.s_addr = INADDR_ANY;    //Specifies the address on which the server will listen to for clients...
                                                //INADDR_ANY specifies to listen on ANY address, but with a fixed port

    //Bind: Similar to 'connect()': Associating the socket with the address, without contacting the address
    //Bind, on the server side, tells all incoming data on the server address to go to the specified socket
    bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));

    //Listen:
    //  socket descriptor
    //  size of backlog of connections: Drop connections after N UNHANDLED connections
    listen(sockfd, 10);

    //Handle connections
    while(1) {
        //Accept connections to the socket, returning a CLIENT socket descriptor, used ONLY for communicating with that
        //specific client
        //Accept is BLOCKING: It will wait until at least 1 client tries to connect
        //The clientaddr structure is filled by accept, filling the client IP address and port number
        int len = sizeof(clientaddr);
        int clientsocket = accept(sockfd, (struct sockaddr *) &clientaddr, &len);

        //Now have a socket descriptor for a specific client

        //Receive on the client socket
        char r_line[1024];
        int n = recv(clientsocket, r_line, 1024, 0);

        //Display data received
        printf("Received %d bytes from client: %s\n", n, r_line);

        //Echo data back to client
        send(clientsocket, r_line, strlen(r_line), 0);

        //Close the client socket
        close(clientsocket);
    }

    //Only close(sockfd) when I am COMPLETELY done accepting connections/clients
}
