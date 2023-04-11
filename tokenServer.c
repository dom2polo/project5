#include "philosophers.h"

// create an array of sockets for the philosophers
int sockets[NUMPHIL];

// the server function that will run on a separate thread
void *server(void *arg)
{
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // set the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVERPORT);

    // bind the socket to the server address
    if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(socket_fd); // close the socket to clean up
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Bind successful\n");
    }

    // listen for incoming connections
    if (listen(socket_fd, NUMPHIL) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Listen successful\n");
    }

    // accept incoming connections and store their sockets in the array
    int current_socket = 0;
    while (1)
    {
        int client_socket = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0)
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        sockets[current_socket] = client_socket;
        current_socket = (current_socket + 1) % NUMPHIL;
    }

    // exit thread
    return NULL;
}

// main function
int main()
{
    // create a thread for the server function
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, server, NULL);

    // wait for the server thread to exit
    pthread_join(server_thread, NULL);

    // exit program
    return 0;
}
