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

//code for writer
int main(){
    int writeaccess;
    //creating or touching a pipe

    char *path="/tmp/mypipe";

    //printing an output
    printf("Opening named pipe: /tmp/mypipe\n");
    mkfifo(path,0666);
    //creating a pipe of 666 giving access of read and write
    char *arr;
    while(1){ 
        //allocate a string of size 100.
        arr=(char*)malloc(100);
        //allowing only writing access
        writeaccess=open(path,O_WRONLY);
        printf("Enter Input:");
        //acceptin input
        fgets(arr,100,stdin);
        printf("Writing buffer to pipe...done\n");
        //write to file
        write(writeaccess,arr,strlen(arr));
        //if exit then exit out of the file
        if(strcmp("exit\n",arr)==0){
            printf("Exiting\n");
            break;
        }
        //close the file 
        close(writeaccess);
    }
}