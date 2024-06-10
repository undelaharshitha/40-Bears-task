Documentation:
Server Program (server.c)
Socket Creation: Creates a TCP socket using socket().
Binding: Binds the socket to the specified port (PORT).
Listening: Puts the server in a listening state using listen().
Accepting Connections: Accepts incoming client connections using accept().
Handling Clients: Reads messages from the client and echoes them back until the connection is closed.
Error Handling: Checks and handles errors at each step (socket creation, binding, listening, accepting, reading, and writing).

Detailed Explanation:
Server Program (server.c)
Include Header Files:

#include <stdio.h>: Standard I/O functions.
#include <stdlib.h>: Standard library functions.
#include <string.h>: String handling functions.
#include <unistd.h>: POSIX API functions.
#include <sys/socket.h>: Definitions for sockets.
#include <netinet/in.h>: Definitions for Internet domain addresses.
#include <arpa/inet.h>: Definitions for Internet operations.
Define Constants:

#define PORT 8080: Port number for the server.
#define BUFFER_SIZE 1024: Size of the buffer for storing messages.
Handle Client Function:

void handle_client(int client_socket): Function to handle communication with a client.
char buffer[BUFFER_SIZE]: Buffer to store the received message.
int bytes_read: Variable to store the number of bytes read.
while ((bytes_read = read(client_socket, buffer, BUFFER_SIZE)) > 0): Loop to read data from the client.
write(client_socket, buffer, bytes_read): Echo the message back to the client.
if (bytes_read < 0): Check for read errors.
else if (bytes_read == 0): Handle client disconnection.
close(client_socket): Close the client socket.
Main Function:

int main(): Entry point of the server program.
int server_socket, client_socket: Variables for server and client sockets.
struct sockaddr_in server_addr, client_addr: Structures for server and client addresses.
socklen_t client_addr_len = sizeof(client_addr): Length of the client address.
if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0): Create a socket.
server_addr.sin_family = AF_INET: Set address family to IPv4.
server_addr.sin_addr.s_addr = INADDR_ANY: Bind to all available interfaces.
server_addr.sin_port = htons(PORT): Set port number.
if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0): Bind socket.
if (listen(server_socket, 3) < 0): Listen for connections.
printf("Server listening on port %d\n", PORT): Print server status.
while (1): Loop to accept and handle client connections.
if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) >= 0): Accept client connection.
printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port)): Print client info
