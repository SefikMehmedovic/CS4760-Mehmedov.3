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

// page ~534 for semaphores in book 
// 
// shared memory key and size of shared memory.. size of an int(4 bytes)
#define SHMKEY 321800
#define BUFF_SZ	sizeof ( int )

//shared memory function
//void sharedMemory(int n, int s);

int main(int argc, char* argv[])
{
  	int input, s, l, t; // s total number of children 5 is default.. l is logfile... t is time default i 2
	
	while ((input = getopt (argc, argv, "hs:l:t:")) != -1)
	
	{
	 switch(input)
	 {
	case 'h':

		printf("\nTo run the program use ./oss\n");
		printf("-h is for help and all options\n ");
	    	printf("-s x where x is the max number of user process spawned (default 5)\n");
       		printf("-l filename is the name of the log file");
        	printf("-t z is where z is the time in seconds when master will terminate itself");
		printf(" and all children(default 2 seconds)\n");
       		printf("Example: ./oss -s 10 (runs oss with 10 child process)\n");
    	break;


	case 's':
       		 s = atoi(optarg);
        	 printf("case s: %d\n", s);
     	break;
    
	case 'l':
		printf("case l input file");
	break;

	case 't':
		t = atoi(optarg);
		printf("case t: %d",t);
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
  
	return 0;
}
