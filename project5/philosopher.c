#include "header.h"

// function
void Coordinator(int thisID, int randomPhilId);
void sendMSG(struct sockaddr_in cAddr, int portNumbers[], int thisID, int cSocket, int randomPhilId, char MSGbuf[], int max);
int recieveMSG(int cSocket, int sSocket, struct sockaddr_in cAddr, int cSocLen, char MSGbuf[], int thisID, int max, int randomPhilId);
int findMax(char MSGbuf[]);
void sig_handler(int signum);
void think_eat(int thisID, int eatingIterations, int randomPhilId, int thinkingIterations);

void Coordinator(int thisID, int randomPhilId)
{
    int err; 
    int sSocket; 
    int cSocket; 
    struct sockaddr_in sAddr; // server socket
    struct sockaddr_in cAddr; // client socket
    int cSocLen; // client socket length
    char MSGbuf[BUFFLEN]; // msg in a buffer
    int portNumbers[] = {PORT1, PORT2, PORT3, PORT4, PORT5}; // philosopher's port number
    int max = 0; 
    int id;
    signal(SIGALRM, sig_handler); // signal handler
    
    // Creates server
    sSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(sSocket == -1) {
        printf("Philosopher %d : socket creation failed", randomPhilId);
        exit(4);
    }
    
    memset(&sAddr, 0, sizeof (struct sockaddr_in));
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(portNumbers[thisID-1]);
    sAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    err = bind (sSocket, (struct sockaddr*)&sAddr, sizeof (struct sockaddr_in));
    if(err == -1) {
        printf("Philosopher %d : bind address to socket failed\n", randomPhilId);
        exit(5);
    }
    
    err = listen (sSocket, 5);
    if(err == -1) {
        printf("Philosopher %d : listen failed", randomPhilId);
        exit(6);
    }
    
    if(thisID == 1) // Occurs for first process to connect to next process
    {
        // Waits 1 second
        alarm(1);
        pause();
        
        sendMSG(cAddr, portNumbers, thisID, cSocket, randomPhilId, MSGbuf, max);
        memset(MSGbuf, 0, strlen(MSGbuf));
        recieveMSG(cSocket, sSocket, cAddr, cSocLen, MSGbuf, thisID, max, randomPhilId);
        
        max = findMax(MSGbuf);
        sendMSG(cAddr, portNumbers, thisID, cSocket, randomPhilId, MSGbuf, max);
    }
    else
    {
        recieveMSG(cSocket, sSocket, cAddr, cSocLen, MSGbuf, thisID, max, randomPhilId);
        sendMSG(cAddr, portNumbers, thisID, cSocket, randomPhilId, MSGbuf, max);
        
        max = -1; // Resets max value to -1
        max = recieveMSG(cSocket, sSocket, cAddr, cSocLen, MSGbuf, thisID, max, randomPhilId);
        
        // Send coordinator message
        if(thisID != 5)
            sendMSG(cAddr, portNumbers, thisID, cSocket, randomPhilId, MSGbuf, max);
    }
    
    if(max == randomPhilId)
    {
        id = fork(); // parent and child process
        
        if(id == 0) // In child process
        {
            char randomPhilIdStr[12];
            sprintf(randomPhilIdStr, "%d", randomPhilId);
            
            err = execl("./centralized", randomPhilIdStr, (char *)NULL); 
            if(err == -1)
            {
                printf("Philosopher %d Error on execl execution: %d\n", randomPhilId, err);
                exit(11);
            }
        }
    }
    
    // Close server and client sockets
    close(cSocket);
    close(sSocket);
}


void sendMSG(struct sockaddr_in cAddr, int portNumbers[], int thisID, int cSocket, int randomPhilId, char MSGbuf[], int max)
{
    int err; 
    char MSGbuf2[BUFFLEN];
    
    // Sets up client socket
    memset(&cAddr, 0, sizeof(struct sockaddr_in));
    cAddr.sin_family = AF_INET;
    cAddr.sin_port = htons(portNumbers[thisID%5]);
    cAddr.sin_addr.s_addr = inet_addr(IP_SERVER);
    
    cSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET
    if(cSocket == -1) {
        printf("Philosopher %d socket creation failed", randomPhilId);
        exit(7);
    }
    
    err = connect(cSocket, (struct sockaddr *)&cAddr, sizeof(struct sockaddr_in));
    if(err == -1) {
        printf("Philosopher %d connect to Philosopher %d failed", thisID, ((thisID+1)%5));
        exit(8);
    }
    
    if(max == 0) // Occurs for sending ELECTION msg
    {
        sprintf(MSGbuf2, "%d", randomPhilId);
        strcat(MSGbuf, " ");
        strcat(MSGbuf, MSGbuf2);
        
        err = send(cSocket, MSGbuf, BUFFLEN, 0);
    }
    else // Occurs for sending COORDINATOR msg
    {
        sprintf(MSGbuf, "%d", max);
        err = send(cSocket, MSGbuf, BUFFLEN, 0);
    }
}


int recieveMSG(int cSocket, int sSocket, struct sockaddr_in cAddr, int cSocLen, char MSGbuf[], int thisID, int max, int randomPhilId)
{
    int err;
    char MSGbuf2[BUFFLEN];
    
    // Sets up client socket from philosopher
    cSocket = accept(sSocket, (struct sockaddr*)&cAddr, &cSocLen);
    if(cSocket == -1) 
    {
        printf("Philosopher[%d]: accept failed", randomPhilId);
        exit(9);
    }
    err = recv(cSocket, MSGbuf2, BUFFLEN, 0);
    if(err == -1)
    {
        printf("Philosopher[%d]: read failed", randomPhilId);
        exit(10);
    }
    
    
    if(max == 0)
    {
        strcat(MSGbuf, MSGbuf2);
        return -1;
    }
    else
    {
        return atoi(MSGbuf2);
    }
}
int findMax(char MSGbuf[])
{
    // Variables
    int i; // For loop index
    int max = 0; // Stores the value of the max number
    int temp = 0; // Stores each number while recreating them
    
    for(i = 0; i < strlen(MSGbuf) + 1; i++) // Scans through MSGbuf array
    {
        if(MSGbuf[i] == ' ' || i == strlen(MSGbuf)) // if not a number or end of MSGbuf
        {
            if(temp > max)
                max = temp;
            
            temp = 0; // reset
        }
        else
        {
            temp *= 10; // Shifts temp value
            temp += (MSGbuf[i] - '0'); 
        }
    }
    
    return max;
}


void sig_handler(int signum){
    // waits
}


void think_eat(int thisID, int eatingIterations, int randomPhilId, int thinkingIterations)
{
    // variables
    int err;
    int i; 
    int eatTime = rand()%5+1; // seconds per eat
    int thinkTime = rand()%5+1; // seconds per think
    int eatenCounter = 0; 
    int thinkingCounter = 0; 
    int cSocket; // client socket
    struct sockaddr_in cAddr; 
    char MSGbuf[BUFFLEN]; 
    signal(SIGALRM, sig_handler); 
    
    srand(time(0)/randomPhilId); // seed
    
    memset (&cAddr, 0, sizeof (struct sockaddr_in));
    cAddr.sin_family = AF_INET;
    cAddr.sin_port = htons(CPORT);
    cAddr.sin_addr.s_addr = inet_addr(IP_SERVER);
    
    cSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET
    if(cSocket == -1)
    {
        printf("Philosopher %d: cSocket to coordinator creation failed", randomPhilId);
        exit(12);
    }
    
    alarm(1);
    pause();
    
    err = connect(cSocket, (struct sockaddr *)&cAddr, sizeof(struct sockaddr_in));
    if (err == -1)
    {
        printf("Philosopher %d : connect to coordinator failed\n", randomPhilId);
        exit(13);
    }
    
    printf("Philosopher %d: connected to the coordinator\n", randomPhilId);
    
    sprintf(MSGbuf, "%d", thisID);
    err = send(cSocket, MSGbuf, BUFFLEN, 0);
    
    do { // Get the coordinator's OK
        err = recv (cSocket, MSGbuf, BUFFLEN, 0);
    } while (err == -1 && errno == EINTR);
    
    // Stores randomPhilId value
    sprintf(MSGbuf, "%d", randomPhilId);
    err = send(cSocket, MSGbuf, BUFFLEN, 0);
    
    do { // Get the coordinator's OK
        err = recv (cSocket, MSGbuf, BUFFLEN, 0);
    } while (err == -1 && errno == EINTR);
    
    while(eatenCounter < eatingIterations || thinkingCounter < thinkingIterations)
    {
        // Send EAT msg to coordinator
        strcpy(MSGbuf, "EAT");
        printf("Philosopher %d  is ready to eat\n", randomPhilId);
        err = send(cSocket, MSGbuf, BUFFLEN, 0);
        
        strcpy(MSGbuf, "empty");
    
        err = recv(cSocket, MSGbuf, BUFFLEN, 0); // err is 0 if no msg is recieved and coordinator ends
        if(err == -1 || err == 0)
            printf("Philosopher %d: Failed to recieve response msg from coordinator\n", randomPhilId);
        
        if(strcmp(MSGbuf, "OK") == 0) // if philosopher can enter eating
        {
            // Philosopher eat for N seconds
            printf("Philosopher %d  stopped eating for %d seconds\n", randomPhilId, eatTime);
            alarm(eatTime);
            pause();
            
            printf("Philosopher %d stopped eating\n", randomPhilId);
            strcpy(MSGbuf, "RELEASE");
            err = send(cSocket, MSGbuf, BUFFLEN, 0);
            eatenCounter++;
            
            printf("Philosopher %d  is thinking for %d seconds\n", randomPhilId, thinkTime);
            alarm(thinkTime);
            pause();
            
            printf("Philosopher %d stopped thinking\n", randomPhilId);
            thinkingCounter++;
        }
        else if(strcmp(MSGbuf, "NO") == 0) // if philosopher cannot eat
        {
            printf("Philosopher %d is thinking for %d seconds\n", randomPhilId, thinkTime);
            alarm(thinkTime);
            pause();
            
            printf("Philosopher %d stopped THINKING\n", randomPhilId);
            thinkingCounter++;
        }
        else // unexpected message
        {
            printf("Philosopher %d: recv() has returned an error : err: %d\n", randomPhilId, err);
            printf("Philosopher %d: recv() has returned an error : MSGbuf: %s\n", randomPhilId, MSGbuf);
            exit(14);
        }
    }
    
    strcpy(MSGbuf, "END");
    err = send(cSocket, MSGbuf, BUFFLEN, 0);
    
    close(cSocket); // Closes socket
    
    printf("Philosopher %d finished eating\n", randomPhilId);
    exit(0);
}


int main (int argc, char *argv[]) {
    // Obtain variables from argv[]
    int thisID = atoi(argv[1]); // ID of philosopher
    int randomPhilId = atoi(argv[2]); 
    int eatingIterations = atoi(argv[3]); // number of eating iterations
    int thinkingIterations = atoi(argv[4]); // number of thinking iterations


    Coordinator(thisID, randomPhilId);
    think_eat(thisID, eatingIterations, randomPhilId, thinkingIterations);
    
    return 0;
}
