#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 6969
#define BUFFER 4096

int is_end(const char *, const int);
void print_error(const char *);

int main(const int argc, const char **argv)
{
    // sockfd server socket
    // connfd client socket
    // valread to store the bytes of values read
    int sockfd, connfd, valread;
    char buffer[BUFFER];  // buffer to store the request form client
    char reponse[BUFFER]; // for storing the response to be sent

    // Creating Socket
    // AF_INET means we will use IPv4 protocol
    // SOCK_STREAM is TCP Connection
    // 0 to select default protocol (TCP/IP Most Case)
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        print_error("Socket Creation error");

    struct sockaddr_in serv_addr;         // Storig the address
    bzero(&serv_addr, sizeof(serv_addr)); // clearing serv_addr with zeros

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Will Accept any kind of address
    serv_addr.sin_port = htons(PORT);              // Convertig the port from little or big endian to network bytes

    // This is to avoid binding error (Mostly occurs while using tcp connection)
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        print_error("setsockopt error");

    // Binding the socket
    if ((bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
        print_error("Socket bind error");

    // Listen for 20 connections at max
    if ((listen(sockfd, 20)) < 0)
        print_error("socket listen error");

    while (1)
    {

        printf("Waiting for connection on port %d\n", PORT);
        fflush(stdout);

        // To Store the client address
        struct sockaddr_in addr;
        socklen_t addr_len;
        char client_address[BUFFER];

        // Accepting the connection from the client
        connfd = accept(sockfd, (struct sockaddr *)&addr, &addr_len);

        // Converting client address from network bytes to string
        inet_ntop(AF_INET, &addr.sin_addr, client_address, BUFFER);

        printf("\nConnection recieved from %s\n\n", client_address);

        memset(buffer, 0, BUFFER); // Clearing the buffer
        while ((valread = read(connfd, buffer, BUFFER)) > 0)
        {
            printf("%s", buffer);
            // Break if its the end of an HTTP Request
            if (is_end(buffer, valread))
                break;
            memset(buffer, 0, BUFFER); // Clearing the buffer
        }

        // if valread is negative means there was an error
        if (valread < 0)
            print_error("Read error");

        // Writing the response to be sent in response
        // size if BUFFER + BUFFER + BUFFER because we are passing buffer and client address using %s and the size of each is BUFFER
        snprintf((char *)reponse, BUFFER + BUFFER + BUFFER, "Someone has found our Super Secret Server\nHere is your IP: %s\n\nAnd your message:\n%s\nNevermind this is just a test we are closing your connection\nHave a nice day, keep coding :)\r\n\r\n", client_address, buffer);

        // Sending the response back to client
        write(connfd, (char *)reponse, strlen((char *)reponse));
        close(connfd);
    }

    return 0;
}

void print_error(const char *error)
{
    fprintf(stderr, "%s\n", error);
    exit(EXIT_FAILURE);
}

int is_end(const char *buff, const int size)
{
    // Checking if end of the request which is \r\n\r\n
    if (buff[size - 1] == '\n' && buff[size - 2] == '\r' && buff[size - 3] == '\n' && buff[size - 4] == '\r')
        return 1;
    return 0;
}