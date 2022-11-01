/* A simple readers/writers program using a one-word shared memory. */
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
//#include <time.h>

#define SIZE sizeof(int)   /* size of [int] integer */
#define run_length 10       /* number of iterations in test run */

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
   if ((pid = fork()) < 0)  /* apply fork and check for error */
     { perror ("error in fork");  
       exit (1);
     }

   if (0 == pid)             
     { /* processing for child */
      //printf("%f\n",(clock()-t));
       printf ("The child process begins.\n");
       for (i_child = 0; i_child < run_length; i_child++)
         { 
           while(1){
             //if the value in shared memory isnt -1 then extract the value and break;
            if(*shared_memory!=-1){
              value=*shared_memory;
              break;
            }
           }
           //sleep(1);  /* wait for memory to be updated */
           //value = *shared_memory;
           printf ("Child's report:  current value = %2d\n", value);
           *shared_memory=-1;//update shared memory value to -1
         }
       printf ("The child is done\n");
     }
  else 
     { /* processing for parent */
      //printf("%f\n",(clock()-t));
       printf ("The parent process begins.\n");
       for (i_parent = 0; i_parent < run_length; i_parent++)
        { 
          while(1){
            //infinite loop
            //if value in shared memory is -1 then the value will be updated to square of parent
            if(*shared_memory==-1){
              *shared_memory = i_parent * i_parent;/* square into shared memory */
              break;
            }
          }
           printf ("Parent's report: current index = %2d\n", i_parent ); 
           //sleep(1);  /* wait for child to read value */
         }
       wait(&pid);
       printf ("The parent is done\n");
     }
   exit (0);
}
