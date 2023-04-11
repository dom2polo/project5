#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define MAX_CHOPSTICKS 5

int chopsticks[MAX_CHOPSTICKS]; // 0: available, 1: in use
int server_fd;
int client_fds[MAX_CLIENTS];

void init_chopsticks() {
    for (int i = 0; i < MAX_CHOPSTICKS; i++) {
        chopsticks[i] = 0;
    }
}

int get_available_chopsticks(int philosopher_id, int chopstick_ids[2]) {
    for (int i = 0; i < MAX_CHOPSTICKS; i++) {
        if (chopsticks[i] == 0) {
            chopsticks[i] = 1;
            chopstick_ids[0] = i;
            for (int j = i+1; j < MAX_CHOPSTICKS; j++) {
                if (chopsticks[j] == 0) {
                    chopsticks[j] = 1;
                    chopstick_ids[1] = j;
                    return 1; // success
                }
            }
            // release the first chopstick if the second one is not available
            chopsticks[i] = 0;
            break;
        }
    }
    // no available chopsticks
    return 0;
}

void release_chopsticks(int chopstick_ids[2]) {
    chopsticks[chopstick_ids[0]] = 0;
    chopsticks[chopstick_ids[1]] = 0;
}

void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    char buffer[1024] = {0};
    int philosopher_id;
    int chopstick_ids[2];
    while (1) {
        // receive message from client
        int valread = read(client_fd, buffer, 1024);
        if (valread <= 0) {
            // client disconnected or error occurred
            close(client_fd);
            return NULL;
        }
        sscanf(buffer, "%d %d %d", &philosopher_id, &chopstick_ids[0], &chopstick_ids[1]);
        // check if chopsticks are available
        if (get_available_chopsticks(philosopher_id, chopstick_ids)) {
            // send permission to client
            char reply[1024];
            sprintf(reply, "Permission %d %d %d\n", philosopher_id, chopstick_ids[0], chopstick_ids[1]);
            send(client_fd, reply, strlen(reply), 0);
        } else {
            // send denial to client
            char reply[] = "Denial\n";
            send(client_fd, reply, strlen(reply), 0);
        }
    }
}

int main(int argc, char const *argv[]) {
    init_chopsticks();

    // create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // bind socket to port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started, listening on port %d...\n", PORT);

    while (1) {
        // accept incoming connection
        struct sockaddr_in client_address;
        int addrlen = sizeof(client_address);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*)&addrlen);
        if (client_fd < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
        printf("New connection from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        // handle client in a separate thread
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_fd) < 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
        // store client file descriptor for later use
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_fds[i] == 0) {
                client_fds[i] = client_fd;
                break;
            }
        }
    }

    return 0;
}
