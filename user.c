//child
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>

// shared memory key and size of shared memory.. size of an int(4 bytes)
#define SHMKEY 321800
#define BUFF_SZ	sizeof ( int )

void childMemory();


int main(int argc, char* argv[]) 
{
	
	return 0;
}

void childMemory()
{
  //int getValue;	

  int shmid = shmget (SHMKEY,BUFF_SZ , 0711 | IPC_CREAT );
  if (shmid == -1)
   {
    printf("Worker: Error in shmget..\n");
    return 1;
   }
  int *cint = ( shmat (shmid, NULL, 0) );
 
}