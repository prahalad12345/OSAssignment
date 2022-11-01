#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include <unistd.h>

void killchild(){
    pid_t  pid = getpid();
    printf("Child Process:%d\n",pid);
    printf("Child is Terminated\n");
    exit(0); 
    //state of exit ranges from 0-255 and -1 if there is an interrupt
    //1 is to exit due to termination
    //0 is successful
}

int main(){
    int pid=fork();
    int status=0;
    if(pid>0){
        pid = wait(&status);
        int num=WEXITSTATUS(status);
        printf("State of Process Termination:%d\n",num);
    }
    else if(pid==0){
        killchild();
    }
    return 0;
}