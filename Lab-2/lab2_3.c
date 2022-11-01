/* ----------------------------------------------------------------- */
/* PROGRAM  fork-01.c                                                */
/*    This program illustrates the use of fork() and getpid() system */
/* calls.  Note that write() is used instead of printf() since the   */
/* latter is buffered while the former is not.                       */
/* ----------------------------------------------------------------- */

#include  <stdio.h>
#include  <string.h>
#include  <sys/types.h>
#include<stdlib.h>
#include <unistd.h>

#define   MAX_COUNT  200
#define   BUF_SIZE   100

void  main(void)
{
     pid_t  pid;
     int    i;
     char   buf[BUF_SIZE];
     int val=0;
     val=fork();
     pid = getpid();
     printf("%d\n",pid);
     sprintf(buf, "This line is from pid %d, value =%d\n", pid,val);
     val=fork();
     pid = getpid();
     printf("%d\n",pid);
     sprintf(buf, "This line is from pid %d, value =%d\n", pid,val);
     write(1, buf, strlen(buf));
     /*for (i = 1; i <= MAX_COUNT; i++) {
          sprintf(buf, "This line is from pid %d, value = %d\n", pid, i);
          write(1, buf, strlen(buf));
        // printf("%s",buf);
     } 
     */
}
