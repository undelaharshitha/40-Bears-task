
Client Program (client.c)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
  int client_socket;
  struct sockaddr_in server_addr;
  char buffer[BUFFER_SIZE];

  // Create socket
  if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket creation error");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
    perror("Invalid address/ Address not supported");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  // Connect to server
  if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Connection Failed");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  // Send message to server
  printf("Enter message: ");
  fgets(buffer, BUFFER_SIZE, stdin);
  send(client_socket, buffer, strlen(buffer), 0);

  // Receive echoed message
  int bytes_received = read(client_socket, buffer, BUFFER_SIZE);
  if (bytes_received < 0) {
    perror("Read error");
  } else {
    buffer[bytes_received] = '\0';  // Null-terminate the received data
    printf("Echoed message: %s\n", buffer);
  }

  close(client_socket);
  return 0;
}
 Explanation:
Client Program (client.c)
Socket Creation: Creates a TCP socket using socket().
Server Address Setup: Sets up the server address using inet_pton() to convert the IP address.
Connecting to Server: Connects to the server using connect().
Sending Message: Reads a message from the user and sends it to the server.
Receiving Echo: Receives the echoed message from the server and displays it.
Error Handling: Checks and handles errors at each step (socket creation, address conversion, connection, reading, and writing).
