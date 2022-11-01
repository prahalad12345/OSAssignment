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


int main(){
    //read access
    int readaccess;
    //path of the 
    char *path="/tmp/mypipe";
    //creating or touching a pipe
    mkfifo(path,0666);

    char *arr;
    arr=(char*)malloc(100);
    printf("Creating named pipe: /tmp/mypipe\n");
    while(1){
        readaccess=open(path,O_RDONLY);
        //allocate space for the string and read it as input
        arr=(char*)malloc(100);
        read(readaccess,arr,100);
        //print the string which was read through the pipe
        printf("Waiting for input...Got it: %s ",arr);
        //if exit is the string read then break out of the infinite loop
        if(strcmp("exit\n",arr)==0){
            printf("Exiting\n");
            break;
        }
        //close the reading file
        close(readaccess);
    }
}