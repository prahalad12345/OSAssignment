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
//user1 starts conversation
//only one text message can be send by aperson per session

int main(){
    //write and read access
    int writeaccess,readaccess;
    char *path="/tmp/mypipe";
    //read write
    mkfifo(path,0666);

    char *arr;
    while(1){ 
        //allocate space for storing a string of size 100
        arr=(char*)malloc(100);
        //open a file to provide write access
        writeaccess=open(path,O_WRONLY);
        //accept user1 input
        printf("\nUser1:");
        //accept input of size max upto 100
        fgets(arr,100,stdin);
        //write it in the pipe
        write(writeaccess,arr,strlen(arr));
        //if exit is the text the break
        if(strcmp("exit\n",arr)==0){
            printf("Exiting\n");
            break;
        }
        //close the write access requested file
        close(writeaccess);

        //allocate the space again
        arr=(char*)malloc(100);
        readaccess = open(path, O_RDONLY);
        //wait until the input is recieved from the other end and read it
        //if it is exit the break or else just printit.
        read(readaccess, arr, 100);
         if(strcmp("exit\n",arr)==0){
            printf("Exiting\n");
            break;
        }
        printf("\nUser2:%s", arr);
        //closing fiile read access 
        close(readaccess);
    }
    return 0;
}