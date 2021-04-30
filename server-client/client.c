#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Hard Coding the port and address for this example
#define PORT 6969
#define HOST "127.0.0.1"

#define BUFFER 4096

int is_end(const char *, const int);
void print_error(const char *);

int main(const int argc, const char **argv)
{

    int sock = 0; // socket;
    int valread;
    struct sockaddr_in serv_addr;
    char *message = "Hello I am client!!\nCan you serve me anything \"Server\"?\r\n\r\n"; // The message to be sent we will end our each message with \r\n\r\n
    char buffer[BUFFER];                                                                  // buffer to store the data and BUFFER is the buffer length

    // Creating Socket
    // AF_INET means we will use IPv4 protocol
    // SOCK_STREAM is TCP Connection
    // 0 to select default protocol (TCP/IP Most Case)
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        print_error("Socket Creation Error");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT); // Convertig the port from little or big endian to network bytes

    // Changing the ipaddress from string to binary
    if ((inet_pton(AF_INET, HOST, &serv_addr.sin_addr)) < 0)
        print_error("Invalid Address");

    // Connecting the socket
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        print_error("Connection Failed");

    // Sending the message to server
    send(sock, message, strlen(message), 0);

    // Clearing the buffer by assigning 0 to each byte
    memset(buffer, 0, BUFFER);
    while (1)
    {
        // Reading Data
        valread = read(sock, buffer, BUFFER);

        printf("%s", buffer);
        fflush(stdout);

        // Breaking the loop if its end of the message
        if (is_end(buffer, valread))
            break;

        memset(buffer, 0, BUFFER); // Clearing the buffer by assigning 0 to each byte
    }

    // Closing the socket
    close(sock);
    return 0;
}

void print_error(const char *error)
{
    fprintf(stderr, "%s\n", error);
    exit(EXIT_FAILURE);
}

int is_end(const char *buff, const int size)
{
    // Checking if end of the message which is \r\n\r\n
    if (buff[size - 1] == '\n' && buff[size - 2] == '\r' && buff[size - 3] == '\n' && buff[size - 4] == '\r')
        return 1;
    return 0;
}