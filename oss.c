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

// page ~534 for semaphores in book 
// 
// shared memory key and size of shared memory.. size of an int(4 bytes)
#define SHMKEY 321800
#define SHMKEYB 321801
#define BUFF_SZ	sizeof ( int )

//shared memory function
void sharedMemory(int n, int s);
void signalHandler(int sig);

int main(int argc, char* argv[])
{
  	int input, s, l, t; // s total number of children 5 is default.. l is logfile... t is time default i 2
	char *fileName = "log.out";

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
		if(t == 0)
		{
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
  	



	return 0;
}

void sharedMemory(int n, int s)
{
    //  printf("sharedmemory: \n");
    int count = 0;
  // get shared memory segment ID.. not 100% of this 
   int shmid = shmget (SHMKEY,BUFF_SZ , 0711 | IPC_CREAT );
   if (shmid == -1)
   {
     printf("OSS:shmid Error in shmget \n");
     exit(1);
   } 
  
  int shmidB = shmget(SHMKEYB,BUFF_SZ, 0711 | IPC_CREAT );
	if(shmidB == -1)
	{
	printf("OSS:shmidB Error in shmget\n ");
	exit(1);
	}
   
   char * paddr = ( char * ) (shmat (shmid, NULL, 0) );
   int * pint = (int *) (paddr);
}

void signalHandler(int sig)
{
  if( sig == SIGINT)
  {
    printf("OSS: CTRL+C Caught: Memory Clean Up");
  }
}

