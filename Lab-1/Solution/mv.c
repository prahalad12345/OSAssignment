#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc,char **argv){
    if(argc<3){
        printf("Parameters aren't enough\n");
        return -1;
    }

    DIR *p;
    p=opendir(argv[argc-1]);
    if(p==NULL){
        perror("can't find directory");
        return -1;
    }
    int fd;
    for(int i=1;i<argc-1;i++){
        fd = open(argv[i],O_RDONLY);
        if(fd==-1){
            printf("\nError:Valid FileName Required\n");
            return -1;
        }
        else{
            char *newfile;
            newfile=malloc(strlen(argv[i])+strlen(argv[argc-1])+1);
            strcpy(newfile,argv[argc-1]);

            strcat(newfile,argv[i]);

            if(rename(argv[1],newfile)==-1){
                printf("Syntax Error .");
            }
            else{
                ;
            }
        }

    }
}