/* A readers/writers program for multiple readers and multiple writers  */
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/sem.h>

#define BUF_SIZE 2           /* logical size of buffer */
#define SHARED_MEM_SIZE (BUF_SIZE)*sizeof(int) /* size of shared memory */

#define buf_used 0     /* semaphore array index to check buffer elts used */
#define buf_space 1    /* semaphore array index to check buffer elts empty */

#define buffer1 2
#define signal 3

int sem_create(int num_semaphores)
{  /* procedure to create specified number of semaphores */
   int semid;

   /* create new semaphore set of semaphores */
   if ((semid = semget (IPC_PRIVATE, num_semaphores, IPC_CREAT | 0600)) < 0)
     {  perror ("error in creating semaphore");/* 0600 = read/alter by user */
        exit (1);
     }
   return semid;
}

void sem_init(int semid, int index, int value)
{  /* procedure to initialize specified semaphore to given value */
   if (semctl (semid, index, SETVAL, value) < 0)
     {  perror ("error in initializing first semaphore");
        exit (1);
     }
}

void P(int semid, int index)
{/* procedure to perform a P or wait operation on a semaphore of given index */
  struct sembuf sops[1];  /* only one semaphore operation to be executed */
   
   sops[0].sem_num = index;/* define operation on semaphore with given index */
   sops[0].sem_op  = -1;   /* subtract 1 to value for P operation */
   sops[0].sem_flg = 0;    /* type "man semop" in shell window for details */

   if (semop (semid, sops, 1) == -1)
     {  perror ("error in semaphore operation");
        exit (1);
     }
}

void V(int semid, int index)
{/* procedure to perform a V or signal operation on semaphore of given index */
   struct sembuf sops[1];  /* define operation on semaphore with given index */

   sops[0].sem_num = index;/* define operation on semaphore with given index */
   sops[0].sem_op  = 1;    /* add 1 to value for V operation */
   sops[0].sem_flg = 0;    /* type "man semop" in shell window for details */

   if (semop (semid, sops, 1) == -1)
     {  perror ("error in semaphore operation");
        exit (1);
     }
}


int main(){
    pid_t pid;          /* variable to record process id of child */

    /* shared memory elements */
    caddr_t shared_memory;   /* shared memory base address */


    /* semaphore elements */
    int semid;       /* identifier for a semaphore set */


    /* set up shared memory segment */
    shared_memory=mmap(0, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, 
                                MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (shared_memory == (caddr_t) -1)
        { perror ("error in mmap while allocating shared memory\n");
        exit (1);
        }
    int *buffer = (int*) shared_memory;
    buffer[0]=1;//initializing the values in the shared memory
    buffer[1]=1;
    /* set up pointers to appropriate places in shared memory segment */
    /* initial starting points */
    int proc[3]={0};
    /* create and initialize semaphores */
    semid = sem_create(5);
    sem_init(semid, buf_used, 1);//buf_used is for getting the signal for the first child process
    sem_init(semid, buf_space, 0);//buf_space is to initializing the value access for the second child process
    //the signal will be sent by the first child process
    sem_init(semid, buffer1, 0);
    //if the value is not divisible by 2 then the signal is sent to process 3
    sem_init(semid, signal, 0);//signal is the process for which the second process must wait this is for waiting until the second process is active.
    //this only affects the beginning of the code because the child process takes time to create
    pid=fork();
    if(pid==0){
        for(int i=1;i<=1000;i++){
            P(semid,buf_used);//signal if greater than 0 moves in 
            buffer[0]=i;
            V(semid,buf_space);//sends signal for the second process
        }
        buffer[0]=0;
        exit(0);
    }   
    else{
        proc[0]=pid;
    }

    pid=fork();
    if(pid==0){
        while(1){
            P(semid,buf_space);
            if(buffer[0]==0){
                buffer[1]=0;
                V(semid,buffer1);
                exit(0);
            }
            int val=buffer[0];
            V(semid,buf_used);
            if(val%2!=0){
                buffer[1]=val;
                V(semid,buffer1);//sends signal for the third child process to perform its action
                P(semid,signal);//it waits until the signal is recieved from third process.
            }   
        }
        
    }
    else 
        proc[1]=pid;

    pid=fork();
    if(pid==0){
        while(1){
            P(semid,buffer1);//once the signal is recieved moves ahead
            V(semid,signal);//sends signal for the second process to continue its work
            int val=buffer[1];

            if(val==0){
                exit(0);
            }
            
            //check if it is divisible 3
            if(val%3!=0){
                printf("%d\n",val);
            }
        }
    }
    else {
        proc[2]=pid;
    }
    //wait for the parent process to finish its execution of child
    for (int p_count = 0; p_count<3; p_count++)
     waitpid(proc[p_count], NULL, 0);
    //remove the sempahore
     if (semctl (semid, 0, IPC_RMID) < 0)   
     {  perror ("error in removing semaphore from the system"); 
        exit (1); 
     }
}