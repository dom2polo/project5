// The program initializes an array of chopsticks and a queue for the philosophers. It also keeps track of which philosopher has finished eating, 
// and which philosopher is currently in the queue using an array.

// There are several functions defined, including "enqueue," which adds a philosopher to the queue if they are not already in the queue. 
// The "dequeue" function removes the first philosopher from the queue and checks if they can eat with the available chopsticks. If they can eat, 
// they are given the chopsticks and removed from the queue. The "shiftLeft" function shifts all the philosophers in the queue to the left by one index. 
// There are also some debugging functions for printing out the contents of the queue and the chopstick array.Each thread represents a philosopher and has 
// its own client socket. The thread obtains the philosopher's ID and randomPhilId value and then enters a loop to simulate eating. 

#include "header.h" // Includes shared header which contains all of the header files


int chopstick[] = {1, 1, 1, 1, 1}; 
int queuedPhil[N]; 
int queuedPhilEnd = 0; 
int exitedQueue[] = {0, 0, 0, 0, 0}; 
int EnqueueCS = 0; 
int DequeueCS = 0; 
int philFinished = 0; 
int randomPhilId[N];

void enqueue(int newNum) // enqueue a philosopher to the queue
{
    int i; // For loop index
    
    for(i = 0; i < N; i++) // For loop used to check full queue
    {       
        if(newNum == queuedPhil[i]) // Occurs if newNum is already in queue
            break;
        
        if(i == N-1) // Occurs if newNum isn't already in queue
        {
            // Adds newNum to the queue
            printf("Coordinator: Philosopher %d in eating queue\n", randomPhilId[newNum-1]);
            queuedPhil[queuedPhilEnd++] = newNum;
        }
    }
}


void shiftLeft()
{
    int i; // For loop index
    
    for(i = 0; i < N; i++) // Shifts all of the philosophers in the queue left one spot
    {
        if(i != 4) // Occurs if index isn't the end of the array
            queuedPhil[i] = queuedPhil[i+1]; // Shifts philosopher left one index
        else
            queuedPhil[i] = 0; // Occurs if the index is the end of the array
    }
    queuedPhilEnd--; // Decrements the number of queued philosophers
}


void dequeue()
{
    int thisID; // Holds id of philosopher dequeued from the queue
    
    if(queuedPhilEnd == 0) // Returns if there are no philosophers in the queue
        return;
    
    while(queuedPhil[0] == -1) // Checks if a philosopher finished execution but is still in the queue
        shiftLeft(); // Shifts all philosophers in the queue left by 1 index
    
    if(queuedPhilEnd == 0) // Returns if there are no philosophers in the queue
        return;
        
    thisID = queuedPhil[0]; // Sets thisID to first philosopher in queue
    
    if(chopstick[thisID%N] == 1 && chopstick[(thisID+1)%N] == 1 && philFinished < 4) // Checks if the first philosopher can eat
    {
        // Tells user philosopher can eat and gives them chopsticks
        printf("Coordinator: Philosopher %d - was selected eat next\n", randomPhilId[thisID-1]);
        chopstick[thisID%N] = 0;
        chopstick[(thisID+1)%N] = 0;
        
        shiftLeft(); // Shifts all philosophers in the queue left by 1 index
        
        exitedQueue[thisID-1] = 1;
    }
}


void printChopsticks() // A function used for debugging
{
    int chopSum = 0; // Used to check if chopsticks are still odd
    int i; // For loop index
    
    // Prints out the contents of chopstick array & counts sum
    printf("Chopsticks: [");
    for(i = 0; i < N; i++)
    {
        printf("%d ", chopstick[i]);
        if(chopstick[i] == 1)
            chopSum++;
    }
    printf("]\n");
    
    if((chopSum % 2) == 0) // Checks if there is an even number of chopsticks in the array
        printf("CHOPSTICKS ARE EVEN\n\n\n\n\n");
}


void printQueue() // A function used for debugging
{
    int i; // For loop index
    
    // Prints out the contents of the queue
    printf("Queue Contents: [");
    for(i = 0; i < N; i++)
    {
        printf("%d", queuedPhil[i]);
        if(i != (N-1))
        {
            printf(" ");
        }
    }
    printf("]\n");
}


void *philThread(void *c)
{
    int cSocket; // The client Socket of the philosopher
    int err; // Stores the ernno value
    char MSGbuf[BUFFLEN]; // A C string that is a buffer for msgs
    int thisID; // The ID value of the philosopher run by the thread
    int i, j; // For loop indexes
    
    cSocket = *((int *)(c)); // Obtains the client socket value from main
    
    // Get the philosopher's thisID value
    do {
        err = recv (cSocket, MSGbuf, BUFFLEN, 0);
    } while (err == -1 && errno == EINTR);
    
    thisID = atoi(MSGbuf);
    
    strcpy(MSGbuf, "thisID received");
    err = send(cSocket, MSGbuf, BUFFLEN, 0);
    
    // Get the philosopher's randomPhilId value
    do {
            err = recv (cSocket, MSGbuf, BUFFLEN, 0);
    } while (err == -1 && errno == EINTR);
    
    randomPhilId[thisID-1] = atoi(MSGbuf);
    
    strcpy(MSGbuf, "randomPhilId received");
    err = send(cSocket, MSGbuf, BUFFLEN, 0);
    
    while(1) // Runs while philosopher hasn't finished eating
    {
        // If global flag for exitedQueue is 1, then send OK msg
        if(exitedQueue[thisID-1] == 1 || philFinished == 4)
        {
            strcpy(MSGbuf, "OK");
            err = send(cSocket, MSGbuf, BUFFLEN, 0);
            exitedQueue[thisID-1] = 0;
        }
        
        // Get the philosopher's EAT, RELEASE, or END msg
        do {
                err = recv (cSocket, MSGbuf, BUFFLEN, 0);
        } while (err == -1 && errno == EINTR);
        
        //printf("Coordinator: Received Philosopher[%d] %s msg\n", thisID, MSGbuf); // Debug message
        
        if(strcmp(MSGbuf, "EAT") == 0) // Occurs if MSG was EAT
        {
            if(exitedQueue[thisID-1] == 1) // Occurs if msg is accidently sent after philosopher ends
            {
                
            }
            else if(chopstick[thisID%N] == 1 && chopstick[(thisID+1)%N] == 1) // Occurs if philosopher can eat
            {
                // Gives philosopher the chopsticks
                chopstick[thisID%N] = 0;
                chopstick[(thisID+1)%N] = 0;
                
                // printChopsticks(); // Used for debugging
                
                // Sends msg that philosopher can eat
                printf("Coordinator allowed Philosopher %d to eat\n", randomPhilId[thisID-1]);
                strcpy(MSGbuf, "OK");
                err = send(cSocket, MSGbuf, BUFFLEN, 0);
            }
            else // Occurs if philosopher cannot eat
            {
                if(EnqueueCS != 0) // Forces a philosopher to wait for 1 second until enqueue() access as CS
                {
                    while(EnqueueCS != 0) // Has philosopher wait 1 second since other philosopher is in critical enqeue section
                    {
                        //printf("Coordinator: Philosopher[%d] - EnqueueCS != 0\n\n\n\n\n\n", thisID); debug msg
                        sleep(1); // Couldn't get alarm() to work
                    }
                }
                EnqueueCS = 1; // Starts enqueue() mutual exclusion
                
                // Enqueues philosopher and msgs philosopher that it cannot eat
                enqueue(thisID);
                strcpy(MSGbuf, "NO");
                err = send(cSocket, MSGbuf, BUFFLEN, 0);
                if(err == -1)
                {
                    printf("Coordinator: Philosopher[%d] failed to send() no msg to philosopher\n\n", thisID);
                    exit(19);
                }
                
                EnqueueCS = 0; // Ends enqueue() mutual exclusion
            }
        }
        else if(strcmp(MSGbuf, "END") == 0) // Occurs if msg was END
        {
            for(j = 0; j < N; j++) // Checks through queuedPhil array to ensure that thisID isn't in queue
            {
                if(queuedPhil[j] == thisID)
                    queuedPhil[j] = -1;
            }
            
            // make sure chopstick is not stolen
            if(chopstick[(thisID-1)%N] == 1 && chopstick[thisID%N] == 0 && chopstick[(thisID+1)%N] == 0 && chopstick[(thisID+2)%N] == 1)
            {
                chopstick[thisID%N] = 1;
                chopstick[(thisID+1)%N] = 1;
            }
            if(chopstick[(thisID-1)%N] == 0 && chopstick[thisID%N] == 0 && chopstick[(thisID+1)%N] == 0 && chopstick[(thisID+2)%N] == 1)
            {
                chopstick[thisID%N] = 1;
                chopstick[(thisID+1)%N] = 1;
            }
            if(chopstick[(thisID-1)%N] == 1 && chopstick[thisID%N] == 0 && chopstick[(thisID+1)%N] == 0 && chopstick[(thisID+2)%N] == 0)
            {
                chopstick[thisID%N] = 1;
                chopstick[(thisID+1)%N] = 1;
            }


            break; // end loop
        }
        else if(strcmp(MSGbuf, "RELEASE") == 0) // if msg is released
        {
            // Release chopsticks
            chopstick[thisID%N] = 1;
            chopstick[(thisID+1)%N] = 1;
            
            if(DequeueCS != 0) // wait for 1 second
                {
                    while(DequeueCS != 0)
                    {
                        sleep(1); 
                    }
                }
            DequeueCS = 1;
            dequeue();
            DequeueCS = 0; 
        }
        else // if MSG was unknown
        {
            printf("Coordinator: Philosopher[%d] sent an error msg\n", thisID);
            exit(20);
        }
    }
    
    philFinished++;
    close(cSocket); // Closes Socket
}


int main(int argc, char *argv[])
{
    // Variables
    int i; // For loop index
    int err;
    int cSocket[N]; // client socket values
    int cSocLen[N]; // client socket length values
    int sSocket; // server socket
    struct sockaddr_in sAddr;
    struct sockaddr_in cAddr[N];
    pthread_t tid[N]; // values for all threads
    
    printf("\nCoordinator: philosopher %d has started a new server\n\n", atoi(argv[0]));
    
    // Create's the Coordinator's server
    sSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(sSocket == -1)
    {
        printf("Coordinator: socket creation failed");
        exit(15);
    }
    
    memset(&sAddr, 0, sizeof (struct sockaddr_in));
    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(CPORT);
    sAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    err = bind (sSocket, (struct sockaddr*)&sAddr, sizeof (struct sockaddr_in));
    if(err == -1)
    {
        printf("Coordinator: Bind address to socket failed");
        exit(16);
    }
    
    err = listen(sSocket, N);
    if(err == -1)
    {
        printf("Coordinator: Listen failed");
        exit(17);
    }
    
    for(i = 0; i < N; i++)
    {
        cSocket[i] = accept(sSocket, (struct sockaddr*)&cAddr[i], &cSocLen[i]);
        if(cSocket[i] == -1)
        {
            printf("Coordinator: Accept failed");
            exit(18);
        }
    }
    
    printf("\nCoordinator: All philosophers have connected\n\n");
    
    // Creates all Philosopher threads
    for(i = 0; i < N; i++)
        err = pthread_create(&tid[i], NULL, philThread, &cSocket[i]);
    
    // Waits for all Philosopher threads to complete
    for(i = 0; i < N; i++)
        pthread_join(tid[i], NULL);
    
    // Announce the end of execution
    printf("Coordinator: All philosophers have finished eating\n");
    close(sSocket); // Close server socket
    exit(0);
    
    return 0;
}
