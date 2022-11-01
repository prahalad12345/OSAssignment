#include<stdio.h>
#include<dirent.h>

bool touch(const char *filename) {
    int fd = open(filename, O_CREAT | S_IRUSR | S_IWUSR);

    if (fd == -1) {
        perror("Unable to touch file");
        return false;
    }

    close(fd);
    return true;
}

int main(){
char d[10];
DIR *p;
struct dirent *d1;
printf("Enter the directory name\n");
scanf("%s",d);

p=opendir(d);
 if(p==NULL){
  perror("can't find directory");
  exit(-1);
}

while(d1=readdir(p))
 printf("%s\n",d1->d_name);
}

