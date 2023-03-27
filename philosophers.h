#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>


#define SERVERPORT 
#define SERVERIP "199.17.28.80"
#define SERVERNAME ""
#define BUFL

#define NUMPHIL 5
#define TIMEMAX 5

using namespace std;

struct philosopher{
	int eatTime;
	int thinkTime;
};
