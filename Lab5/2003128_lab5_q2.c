/* A simple readers/writers program using a one-word shared memory. */
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/sem.h>

#define SIZE sizeof(int)   /* size of [int] integer */
#define BUF_SIZE 1           /* logical size of buffer */
#define SHARED_MEM_SIZE (BUF_SIZE+2)*sizeof(int) /* size of shared memory */
#define run_length 10  /* number of iterations in test run */

#define buf_used 0     /* semaphore array index to check buffer elts used */
#define buf_space 1    /* semaphore array index to check buffer elts empty */

int sem_init(void)
{  /* procedure to create and initialize semaphores and return semaphore id,
      assuming two semaphores defined in the given array of semaphores     */
   int semid;

   /* create new semaphore set of 2 semaphores */
   if ((semid = semget (IPC_PRIVATE, 2, IPC_CREAT | 0600)) < 0)  /*https://man7.org/linux/man-pages/man2/semget.2.html*/
     {  perror ("error in creating semaphore");/* 0600 = read/alter by user */
        exit (1);
     }

   /* initialization of semaphores */
   /* BUF_SIZE free spaces in empty buffer */
   if (semctl (semid, buf_space, SETVAL, BUF_SIZE) < 0) /*https://man7.org/linux/man-pages/man2/semctl.2.html*/
     {  perror ("error in initializing first semaphore");
        exit (1);
     }

   /* 0 items in empty buffer */
   if (semctl (semid, buf_used, SETVAL, 0) < 0) 
     {  perror ("error in initializing second semaphore");
        exit (1);
     }
   return semid;
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
  
   if (semop (semid, sops, 1) == BUF_SIZE)
     {  perror ("error in semaphore operation");
        exit (1);
     }
}

int main (void)
{  pid_t pid;               /* variable to record process id of child */
   caddr_t shared_memory;   /* shared memory base address */
   int i_parent, i_child;   /* index variables */
   int value;               /* value read by child */

   /* set up shared memory segment */
  shared_memory=mmap(0, SIZE, PROT_READ | PROT_WRITE, 
                              MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  if (shared_memory == (caddr_t) -1)
    { perror ("error in mmap while allocating shared memory\n");
      exit (1);
    }
    *shared_memory=-1;
  //clock_t t=clock();
   int semid = sem_init();

   if (-1 == (pid = fork())) /* check for error in spawning child process */
     { perror ("error in fork");  
       exit (1);
     }
   if (0 == pid)             
     { /* processing for child */
      //printf("%f\n",(clock()-t));
       printf ("The child process begins.\n");
       for (i_child = 0; i_child < run_length; i_child++)
         { 
       
             //if the value in shared memory isnt -1 then extract the value and break;
            P(semid, buf_used);
            value=*shared_memory;
            V(semid, buf_space); 
           
           //sleep(1);  /* wait for memory to be updated */
           //value = *shared_memory;
           printf ("Child's report:  current value = %2d\n", value);
         }
       printf ("The child is done\n");
     }
  else 
     { /* processing for parent */
      //printf("%f\n",(clock()-t));
       printf ("The parent process begins.\n");
       for (i_parent = 0; i_parent < run_length; i_parent++)
        { 
          P(semid, buf_space);
          *shared_memory = i_parent * i_parent;/* square into shared memory */
          V(semid, buf_used);
           printf ("Parent's report: current index = %2d\n", i_parent ); 
           //sleep(1);  /* wait for child to read value */
         }
       wait(&pid);
       printf ("The parent is done\n");
     }
   exit (0);
}
