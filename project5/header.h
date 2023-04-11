#ifndef HEADER_H
#define HEADER_H


// Used header files
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <pthread.h>
#include <unistd.h> 

// Ports for Philosopher servers and Coordinator's server
#define PORT1         25635
#define PORT2         20558
#define PORT3         19599
#define PORT4         17505
#define PORT5         27669
#define CPORT         27697


// Server ip and name
#define IP_SERVER       "172.17.254.52"
#define SERVER_NAME     "dom2polo"

#define BUFFLEN 100 // buffer length of philosopher
#define N 5 // number of philosophers


#endif
