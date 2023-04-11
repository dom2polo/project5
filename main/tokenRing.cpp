// Client side

#include "philosophers.h"

char buffer[BUFL] = {0};
int sock = 0, valread;


// sends a request to get chopsticks and waits for a response from the server
int requestChopsticks(int chopsticksIndex) // chopsticksIndex param is the index of the chopsticks to request
{
    strcpy(buffer, "");
    switch (chopsticksIndex)
    {

    case 0:
        strcpy(buffer, CHOPSTICKS1);
        break;
    case 1:
        strcpy(buffer, CHOPSTICKS2);
        break;
    case 2:
        strcpy(buffer, CHOPSTICKS3);
        break;
    case 3:
        strcpy(buffer, CHOPSTICKS4);
        break;
    case 4:
        strcpy(buffer, CHOPSTICKS5);
        break;
    }

    // sends a request to get chopsticks
    send(sock, REQUESTCHOPSTICKS, strlen(REQUESTCHOPSTICKS), 0);
    printf("Requesting Chopsticks: %d\n", chopsticksIndex);
    
    // sends the index of the chopsticks to request
    send(sock, buffer, strlen(buffer), 0);
    printf("sending: %s\n", buffer);

    strcpy(buffer, "");
    // will read the response from the server
    valread = read(sock, buffer, BUFL);
    printf("Server says: %s\n", buffer);

    // if the response is OK the chopsticks are granted
    if (strcmp(buffer, "OK"))
        return true;
    // if the response is NOT the chopsticks are not granted
    else if (strcmp(buffer, "NOT"))
        return false;

    return 0;
}

// sends a request to return chopsticks and waits for a response from the server
int returnChopsticks(int chopsticksIndex) // chopsticksIndex param is the index of the chopsticks to return
{
    switch (chopsticksIndex)
    {

    case 0:
        strcpy(buffer, CHOPSTICKS1);
        break;
    case 1:
        strcpy(buffer, CHOPSTICKS2);
        break;
    case 2:
        strcpy(buffer, CHOPSTICKS3);
        break;
    case 3:
        strcpy(buffer, CHOPSTICKS4);
        break;
    case 4:
        strcpy(buffer, CHOPSTICKS5);
        break;
    }

    printf("Returning Chopsticks: %d\n", chopsticksIndex);
    
    // sends request to return chopsticks
    send(sock, RETURNCHOPSTICKS, strlen(RETURNCHOPSTICKS), 0);
    
    // sends the index of the chopsticks to be returned
    send(sock, buffer, BUFL, 0);
    
    // reads the response from the server.
    valread = read(sock, buffer, BUFL);
    printf("Chopsticks Returned: %s\n", buffer);

    return 0;
}

// function simulates the philosopher eating
void eat(struct philosopher phil)
{
    phil.eatTime = rand() % TIMEMAX + 1;
    printf("Phil with chopsticks %d is eating for %d\n", phil.eatTime);
    sleep(phil.eatTime);
}

// function simulates the philosopher thinking
void think(struct philosopher phil)
{
    phil.thinkTime = rand() % TIMEMAX + 1;
    printf("Phil with chopsticks %d is thinking for %d\n", phil.thinkTime);
    sleep(phil.thinkTime);
}

// need to do main yet
int main(int argc, char const *argv[])
{
}
