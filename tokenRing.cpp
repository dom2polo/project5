// Client side

#include "philosophers.h"

char buffer[BUFL] = {0};
int sock = 0, valread;


int requestChopsticks(int chopsticksIndex)
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

    send(sock, REQUESTCHOPSTICKS, strlen(REQUESTCHOPSTICKS), 0);

    printf("Requesting Chopsticks: %d\n", chopsticksIndex);
    send(sock, buffer, strlen(buffer), 0);
    printf("sending: %s\n", buffer);

    strcpy(buffer, "");
    valread = read(sock, buffer, BUFL);
    printf("Server says: %s\n", buffer);

    if (strcmp(buffer, "OK"))
        return true;
    else if (strcmp(buffer, "NO"))
        return false;

    return 0;
}

int returnChopsticks(int chopsticksIndex)
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
    send(sock, RETURNCHOPSTICKS, strlen(RETURNCHOPSTICKS), 0);

    send(sock, buffer, BUFL, 0);

    valread = read(sock, buffer, BUFL);
    printf("Chopsticks Returned: %s\n", buffer);

    return 0;
}

void eat(struct philosopher phil)
{
    phil.eatTime = rand() % TIMEMAX + 1;
    printf("Phil with chopsticks %d is eating for %d\n", phil.eatTime);
    sleep(phil.eatTime);
}

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
