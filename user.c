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

//semaphore
sem_t *semaphore;
sem_t MUTEX;

int main(int argc, char* argv[]) 
{
  printf("CHILD\n");\
  
  
  //semaphore---------
  
  semaphore = sem_open("sem_name", O_CREAT | O_EXCL, 0777, MUTEX);
  //end Semaphore -------
  int *clock;
  int *shmMsg;

  int shmidA = shmget (SHMKEYA,BUFF_SZ , 0777 | IPC_CREAT );
  if (shmidA == -1)
   {
    printf("User: Error in shmgetA..\n");
    return 1;
   }
   
  int shmidB = shmget(SHMKEYB,BUFF_SZ, 0777 | IPC_CREAT ); 
	if(shmidB == -1)
	{
	printf("User: Error in shmgetB \n ");
	exit(1);
	}
   
    clock = shmat(shmidA, NULL, 0);
    shmMsg = shmat(shmidB, NULL, 0);	
    
	return 0;
 
}

