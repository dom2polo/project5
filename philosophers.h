#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <sstream>

#define SERVERPORT 80
#define SERVERIP "199.17.28.80"
#define SERVERNAME "team3"
#define BUFL 1000

#define NUMPHIL 5
#define TIMEMAX 5

using namespace std;

struct philosopher
{
    int eatTime;
    int thinkTime;
    int chopsticks;
};

#define FORK1 "001"
#define FORK2 "002"
#define FORK3 "003"
#define FORK4 "004"
#define FORK5 "005"

#define CHOPSTICKS1 "010"
#define CHOPSTICKS2 "020"
#define CHOPSTICKS3 "030"
#define CHOPSTICKS4 "040"
#define CHOPSTICKS5 "050"

#define CONNECTING "000"
#define ASSIGNCHOPSTICKS "100"
#define REQUESTFORK "200"
#define RETURNFORK "300"
#define WHICHFORK "400"
#define WHICHCHOPSTICKS "500"

