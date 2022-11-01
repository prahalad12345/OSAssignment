#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc,char **argv){
    if(argc==1){
        printf("Expected File Names\n");
        exit(-1);
    }
    char content[2000000];
    int fd,n;
    for(int i=1;i<argc;i++){
        fd = open(argv[i],O_RDONLY);
        if(fd==-1){
            printf("\nError:Valid FileName Required\n");
            exit(-1);
        }
        else{
            n = read(fd,content,2000000);
            n = write(1,content,n);
        }
        close(fd);
    }
    return 0;
}