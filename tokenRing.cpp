// Client side

#include "philosophers.h"

char buffer[BUFL] = {0};
int sock = 0, valread;

int requestChopsticks()
{
    int sticks;
    printf("Requesting Chopsticks.\n");
    send(sock, ASSIGNCHOPSTICKS, strlen(ASSIGNCHOPSTICKS), 0);

    valread = read(sock, buffer, BUFL);
    ss << buffer;
    ss >> valread;

    sticks = atoi(buffer);
    printf("Server says: %s\n", buffer);
    printf("num value: %d\n", valread);
    printf("sticks: %d\n", sticks);
    return sticks;
}

int requestFork(int forkIndex)
{
    strcpy(buffer, "");
    switch (forkIndex)
    {

    case 0:
        strcpy(buffer, FORK1);
        break;
    case 1:
        strcpy(buffer, FORK2);
        break;
    case 2:
        strcpy(buffer, FORK3);
        break;
    case 3:
        strcpy(buffer, FORK4);
        break;
    case 4:
        strcpy(buffer, FORK5);
        break;
    }

    send(sock, REQUESTFORK, strlen(REQUESTFORK), 0);

    printf("Requesting Fork: %d\n", forkIndex);
    send(sock, buffer, strlen(buffer), 0);
    printf("sending: %s\n", buffer);

    strcpy(buffer, "");
    valread = read(sock, buffer, BUFL);
    printf("Server says: %s\n", buffer);

    if (strcmp(buffer, "OKK"))
        return true;
    else if (strcmp(buffer, "NOO"))
        return false;

    return 0;
}

int returnFork(int forkIndex)
{
    switch (forkIndex)
    {

    case 0:
        strcpy(buffer, FORK1);
        break;
    case 1:
        strcpy(buffer, FORK2);
        break;
    case 2:
        strcpy(buffer, FORK3);
        break;
    case 3:
        strcpy(buffer, FORK4);
        break;
    case 4:
        strcpy(buffer, FORK5);
        break;
    }

    printf("Returning Fork: %d\n", forkIndex);
    send(sock, RETURNFORK, strlen(RETURNFORK), 0);

    send(sock, buffer, BUFL, 0);

    valread = read(sock, buffer, BUFL);
    printf("Fork Returned: %s\n", buffer);

    return 0;
}

void eat(struct philosopher phil)
{
    phil.eatTime = rand() % TIMEMAX + 1;
    printf("Phil with chopsticks %d is eating for %d\n", phil.chopsSticks, phil.eatTime);
    sleep(phil.eatTime);
}

void think(struct philosopher phil)
{
    phil.thinkTime = rand() % TIMEMAX + 1;
    printf("Phil with chopsticks %d is thinking for %d\n", phil.chopSticks, phil.thinkTime);
    sleep(phil.thinkTime);
}

// need to do main yet
int main(int argc, char const *argv[])
{
}
