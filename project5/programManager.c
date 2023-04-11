// programManger is a program that creates N child processes, where N is a constant defined in the header file. 
// Each child process represents a philosopher that will run the dining philosopher problem. 
// The program receives two command-line arguments: the number of eating cycles and the number of thinking 
// cycles each philosopher will have.

#include "header.h"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("programManager: Please enter the number of eating cycles and thinking cycles as a parameter and relaunch program\n");
        exit(1);
    }
    
    // Variables
    int err; 
    int pid;
    int i; 
    int z; // generated philosopher IDs
    int generateNum[N];
    char randIdString[12];
    int eatIterate = atoi(argv[1]); 
    char eatIterateString[12]; 
    int thinkIterate = atoi(argv[2]); 
    char thinkIterateString[12];
    char idStr[N]; 
    time_t t2;
    
    srand((int)time(&t2) % getpid()); // seeds the rand() to get philosopher IDs
    
    for (i = 0; i < N; i++)
    {
        z=rand()%1000+1; // Generates philosopher ID
        
        while(z == generateNum[0] || z == generateNum[1] || z == generateNum[2] || z == generateNum[3] || z == generateNum[4])
        {
            z=rand()%1000+1;
        }
        generateNum[i] = z; //  philosopher ID in the array
        
        pid = fork(); // parent and child process
        
        if (pid < 0) // if fork() fails to create process
        {
            printf("programManager: Fork failed");
            exit(2);
        }
        else if (pid == 0) // child process
        {
            sprintf(randIdString, "%d", z);
            sprintf(eatIterateString, "%d", eatIterate);
            sprintf(thinkIterateString, "%d", thinkIterate);
            sprintf(idStr, "%d", (i+1));

            err = execl("./philosopher", "philosopher", idStr, randIdString, eatIterateString, thinkIterateString, (char *)NULL);
            
            if(err == -1)
            {
                printf("programManager: execl didn't work\n");
                exit(3);
            }
        }
    }
}
