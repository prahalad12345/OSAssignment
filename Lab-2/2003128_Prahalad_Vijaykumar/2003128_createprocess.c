#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include <unistd.h>

int main(){
    fork();
    fork();
    execlp("./a.out","./a.out",NULL);
}