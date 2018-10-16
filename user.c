//user
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>


// shared memory key and size of shared memory.. size of an int(4 bytes)
#define SHMKEYA 321800
#define SHMKEYB 321801
#define BUFF_SZ	sizeof ( int )

int shmidA, shmidB;

int main(int argc, char* argv[]) 
{
 //printf("\nCHILD\n");
 pid_t pid = (long)getpid();
  
 sem_t *semaphore = sem_open("sem_name", IPC_CREAT | IPC_EXCL, 0777, 1);
  //end Semaphore -------
  
  int *clock;
  int *shmMsg;

   shmidA = shmget (SHMKEYA,BUFF_SZ , 0777 | IPC_CREAT );
  if (shmidA == -1)
   {
    printf("User: Error in shmgetA..\n");
    return 1;
   }
   
   shmidB = shmget(SHMKEYB,BUFF_SZ, 0777 | IPC_CREAT ); 
	if(shmidB == -1)
	{
	printf("User: Error in shmgetB \n ");
	exit(1);
	}
   
    clock = shmat(shmidA, NULL, 0);
    shmMsg = shmat(shmidB, NULL, 0);	
    
   // generate random wait time
   
 srand(time(NULL));
 
  int stop = 1;	 
  int wait, begin, end;	
  
	begin = (clock[0] * 1000000000) + clock[1];
	wait = (rand() % 999999) + 1;
	end = begin + wait; 
 
  while(stop == 1) {
		// wait on the semaphore
		sem_wait(semaphore);
		
		
		// CRITICAL SECTION----------------
   
		// if end time check shmMsg is empty
		if(((clock[0] * 1000000000) + clock[1]) <= end)
    {
			if(shmMsg[0] != 0 || shmMsg[1] != 0 || shmMsg[2] != 0){
				// shmMsg is full, can't do anything
			}
			else {
				// set the loop to end and post message to shmMsg
				stop = 0;
				shmMsg[0] = pid;
				shmMsg[1] = clock[0];
				shmMsg[2] = clock[1];
			}
			
		}
		// CRITICAL SECTION END---------
		
		// let go of the semaphore
		sem_post(semaphore);
	}  
    
	return 0;
 
}



