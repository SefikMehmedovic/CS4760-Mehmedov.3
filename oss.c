#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>

// page ~534 for semaphores in book 
// 
// shared memory key and size of shared memory.. size of an int(4 bytes)
#define SHMKEYA 321800
#define SHMKEYB 321801
#define BUFF_SZ	sizeof ( int )


void signalHandler(int sig);
void cleanUp();

sem_t *semaphore;
pid_t *pcpids;

int shmidA, shmidB;

int main(int argc, char* argv[])
{
 
  pid_t pid = 0;
  
  //catch CTRL+C and clean up memory
  signal(SIGTERM, signalHandler);

  //semaphore---------
 
   semaphore = sem_open ("sem_name", IPC_CREAT | IPC_EXCL, 0777 , 1);
  
  if (semaphore == SEM_FAILED) {
		printf("\nERROR: OSS semaphore :\n");
     
     exit(EXIT_FAILURE);
    }
  
  //end semaphore-------

  int input, s, l, t; // s total number of children 5 is default.. l is logfile... t is time default is 2
  FILE *filePointer;
	char *fileName = "log.txt";
  
  
	while ((input = getopt (argc, argv, "hs:l:t:")) != -1)
	
	{
	 switch(input)
	 {
	case 'h':

		printf("\nTo run the program use ./oss\n");
		printf("-h is for help and all options\n ");
	    	printf("-s x where x is the max number of user process spawned (default 5)\n");
       		printf("-l filename is the name of the log file\n");
        	printf("-t z is where z is the time in seconds when master will terminate itself");
		printf(" and all children(default 2 seconds)\n");
       		printf("Example: ./oss -s 10 (runs oss with 10 child process)\n");
    	break;


	case 's':
       		 s = atoi(optarg);
            if(s > 20)
            {
              printf("\nS can not be greater than 20. S was set to 20\n");
              s = 20;
            }
	   else if(s == 0)
		{
     printf("\nS is set to default value of 5\n");
		 	s = 5;
		}
        	 printf("case s: %d\n", s);
     	break;
    
	case 'l':
		printf("case l input file");
	break;

	case 't':
		t = atoi(optarg);
		printf("case t: %d",t);
		if(t == 0 || t <= 0)
		{
       printf("\nT value is set to default value of 2\n");
			t = 2;
		}
	break;

   	case '?':
  	 fprintf(stderr, "Unknown option %c\n", optopt);
 	exit(1);   	
	break;
    default:
      //exit program incorrect options
     // exit(1);
    return 1;
	
	 }
	
	} 

// variables for shared memory
// Clock[0]  is seconds clock[1] is nanoseconds
// shmMsg is the msg containing child PID seconds and nanoseconds
  int *clock;
  int *shmMsg;
  
  //create sharedMemory --------------------------
  
  shmidA = shmget (SHMKEYA,BUFF_SZ , 0777 | IPC_CREAT );
   if (shmidA < 0)
   {
     printf("OSS:shmid Error in shmgetA \n");
     exit(1);
   } 
  //get shared memoryB ID B and check for errors
   shmidB = shmget(SHMKEYB,BUFF_SZ, 0777 | IPC_CREAT );
	if(shmidB < 0)
	{
	printf("OSS:shmidB Error in shmgetB \n ");
	exit(1);
	}
 
   // attach to memory
   clock = shmat(shmidA, NULL, 0);
   shmMsg = shmat(shmidB, NULL, 0);
   
//   char * paddr = ( char * ) (shmat (shmidA, NULL, 0) );
//   
//   int * pint = (int *) (paddr);
  
  
  
  clock[0] = 0; //seconds
  clock[1] = 0; //nanoseconds
  shmMsg[0] = 0; //user PID 
  shmMsg[1] = 0; //seconds
  shmMsg[2] = 0; //nanoseconds
  	
//------ end sharedMemory-----------------------

//open file to write-------

filePointer = fopen(fileName, "w");
fprintf(filePointer,"Starting Log--------\n");

if(filePointer == NULL)
{  
  printf("\nError: opening file\n");
  exit(1);
}
// end write file ------

pid_t (*cpids)[s] = malloc(sizeof *cpids);
pcpids = cpids;

int kidLimit = 100; //limit of max process allowed

//fork child process
int i = 0;
for(i; i < s; i++){
		(*cpids)[i] = fork();
		if ((*cpids)[i] < 0) { 
			printf("Fork failed!\n");
			exit(1);
		}
		else if ((*cpids)[i] == 0){
			
			execl("./user", "user", NULL); 
			printf("execl() failed!\n");
			exit(1);
		}
	}
//-- time vars and time calculation

time_t startTime, endTime;

startTime = time(NULL);
endTime = startTime + t;
int hundredK = 100000;
int maxNano = 1000000000;

while((startTime < endTime) && (s != kidLimit ))
{ //start while loop
  int temp;

  startTime = time(NULL);
  clock[1]+= hundredK;
  
  if(clock[1] == maxNano)
  {
    clock[1] = 0;
    clock[0] ++;
  }
  
  if(shmMsg[0] != 0 || shmMsg[1] != 0 || shmMsg[2] !=0)
  {  
    int i;
    for(i=0; i<s; i++)
    {
      if((*cpids)[i] == shmMsg[0])
      {
        temp = i;
      }
    }
    
    printf("OSS: Child %d is terminating at my time %d.%d | because it reached %d.%d\n in user\n", shmMsg[0], clock[0], clock[1], shmMsg[1], shmMsg[2]);
		 fprintf(filePointer,"OSS: Child %d is terminating at my time %d.%d | because it reached %d.%d in user\n", shmMsg[0], clock[0], clock[1], shmMsg[1], shmMsg[2]);
      
      
  }
} //end while loop 

fclose(filePointer); //close write file
cleanUp(); //call clean up at the end
return 0; // main
} // main end

//------signal handler for CTRL+C
void signalHandler(int sig)
{
  if( sig == SIGINT)
  {
    printf("OSS: CTRL+C Caught: Memory Clean Up");
    cleanUp();
  }
}

void cleanUp()
{
  printf("\nClean up started....\n");
  shmctl(shmidA, IPC_RMID, NULL);
  shmctl(shmidB, IPC_RMID, NULL);
  free(pcpids);
  sem_close(semaphore);
  
}
