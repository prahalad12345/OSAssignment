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
    //read and write access
    int readaccess,writeaccess;
    char *path="/tmp/mypipe";
    //making pipe
    mkfifo(path,0666);

    char *arr;
    arr=(char*)malloc(100);
    //arr is the string which acts as input and output
    while(1){
        //opening file to read
        readaccess=open(path,O_RDONLY);
        arr=(char*)malloc(100);
        //extrcting the input in the pipe and read it 
        read(readaccess,arr,100);
        //print the string 
        printf("\nUser1:%s",arr);
        if(strcmp("exit\n",arr)==0){
            //if exit the break
            printf("Exiting\n");
            break;
        }
        close(readaccess);
        arr=(char*)malloc(100);
        //write a file 
        writeaccess=open(path,O_WRONLY);
        printf("\nUser2:");
        //get the input from the user2 
        fgets(arr,100,stdin);
        write(writeaccess,arr,strlen(arr));
        //write on the output file and exit when the string exit is obtained as input
        if(strcmp("exit\n",arr)==0){
            printf("Exiting\n");
            break;
        }
        close(writeaccess);
    }
    return 0;
}