
#include  <stdio.h>
#include  <string.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int pid;
    int status;

    pid =fork();

    if(pid>0){
        printf("%d",pid);
    }

    else{
        sleep(200);
        exit(0);
    }
}