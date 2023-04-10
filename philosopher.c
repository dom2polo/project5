#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "199.17.28.80"
#define SERVER_PORT 5000

#define MAX_MSG_SIZE 1024

int main(int argc, char **argv) {
    // Create a socket for communicating with the server
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Create a sockaddr_in struct for the server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // Main loop for the philosopher process
    while (1) {
        // Think for a random amount of time
        int think_time = rand() % 10 + 1;
        printf("Philosopher %d thinking for %d seconds\n", getpid(), think_time);
        sleep(think_time);

        // Send a request to the server to pick up two chopsticks
        int chopstick1 = rand() % 5;
        int chopstick2 = (chopstick1 + 1) % 5;
        char request_msg[MAX_MSG_SIZE];
        snprintf(request_msg, MAX_MSG_SIZE, "%d,%d,%d", getpid(), chopstick1, chopstick2);
        if (send(sock_fd, request_msg, strlen(request_msg) + 1, 0) == -1) {
            perror("Error sending request to server");
            exit(EXIT_FAILURE);
        }

        // Wait for a response from the server
        char response_msg[MAX_MSG_SIZE];
        int bytes_received = recv(sock_fd, response_msg, MAX_MSG_SIZE, 0);
        if (bytes_received == -1) {
            perror("Error receiving response from server");
            exit(EXIT_FAILURE);
        }
        response_msg[bytes_received] = '\0';

        // If permission is granted, eat for a random amount of time
        if (strcmp(response_msg, "granted") == 0) {
            int eat_time = rand() % 10 + 1;
            printf("Philosopher %d eating for %d seconds with chopsticks %d and %d\n", getpid(), eat_time, chopstick1, chopstick2);
            sleep(eat_time);
        } else {
            printf("Philosopher %d denied permission to pick up chopsticks %d and %d\n", getpid(), chopstick1, chopstick2);
        }

        // Return the chopsticks to the server
        char release_msg[MAX_MSG_SIZE];
        snprintf(release_msg, MAX_MSG_SIZE, "release,%d,%d", chopstick1, chopstick2);
        if (send(sock_fd, release_msg, strlen(release_msg) + 1, 0) == -1) {
            perror("Error sending release message to server");
            exit(EXIT_FAILURE);
        }
    }

    // Close the socket connection
    if (close(sock_fd) == -1) {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }

    return 0;
}
