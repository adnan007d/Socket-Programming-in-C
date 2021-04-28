#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 80
#define BUFFER 4096

int is_end(const char *, const int);
void print_error(const char *);

int main(const int argc, const char **argv)
{

    if (argc != 2)
        print_error("usage: client <ip_address>\nExample: client 192.168.0.1");

    int sock = 0; // socket;
    int valread;
    struct sockaddr_in serv_addr;
    char *request = "GET / HTTP/1.1\r\n\r\n"; // HTTP GET Request
    char buffer[BUFFER];                      // buffer to store the data and BUFFER is the buffer length

    // Creating Socket
    // AF_INET means we will use IPv4 protocol
    // SOCK_STREAM is TCP Connection
    // 0 to select default protocol (TCP/IP Most Case)
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
        print_error("Socket Creation Error");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Changing the ipaddress from string to binary
    if ((inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)) <= 0)
        print_error("Invalid Address");

    // Connecting the socket
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        print_error("Connection Failed");

    // Sending the HTTP Request
    send(sock, request, strlen(request), 0);

    // Clearing the buffer by assigning 0 to each byte
    memset(buffer, 0, BUFFER);
    while (1)
    {
        // Reading Data
        valread = read(sock, buffer, BUFFER);

        printf("%s", buffer);
        fflush(stdout);

        // Breaking the loop if its end of the request
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
    // Checking if end of the request which is \r\n\r\n
    if (buff[size - 1] == '\n' && buff[size - 2] == '\r' && buff[size - 3] == '\n' && buff[size - 4] == '\r')
        return 1;
    return 0;
}