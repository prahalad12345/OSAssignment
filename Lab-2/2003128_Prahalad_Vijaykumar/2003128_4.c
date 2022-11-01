#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include <unistd.h>

int main(){
    int x=10;
    pid_t processid;
    int t=2;
    while(t--){
        processid=getpid();
        printf("%d process,  %p \n",processid,&x);
    }
    return 0;
}