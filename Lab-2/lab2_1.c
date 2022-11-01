#include<stdio.h>
#include<sys/types.h>   //includes various data type declarations  
           			    //https://pubs.opengroup.org/onlinepubs/009696899/basedefs/sys/types.h.html
#include<sys/wait.h>    //includes declarations for waiting
						//https://pubs.opengroup.org/onlinepubs/009604599/basedefs/sys/wait.h.html
#include<stdlib.h>
#include<unistd.h>		//includes standard symbolic constants and types 
						//provides access to the POSIX  operating system API
						//https://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html

int main(){

int pid;
int status;

pid =fork();
printf("%d\n",pid);
if(pid>0){
  pid=wait(&status);
}

else{
execlp("ls"," ","-l","-a",NULL);
exit(0);
}



}
