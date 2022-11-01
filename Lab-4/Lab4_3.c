#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSIZE	4  /* Size of the shared memory */

int main(){

    char c;
    int shmid;
    key_t key;
    int *shm;

    key = 5678; /* Some key to uniquely identify shared memory */

    /* Create Segment */ 

    if((shmid=shmget(key,SHMSIZE,IPC_CREAT | 0666))< 0){
        perror("shmget");
        exit(1);
    }

   /* Attach the segment to our data space */

    if((shm=shmat(shmid,NULL,0))==(int *) -1){
        perror("shmat");
        exit(0);
    }

  /* Now put something into shared memory */

    *shm=5;
     /* End with NULL termination */

  /* Wait until the other process changes the first character to '*' in the shared memory */
    printf("%d",*shm);

     exit(0);

}
        
