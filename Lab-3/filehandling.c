#include <stdio.h>
#include <stdlib.h> 

double *a;

void input_array(int *n,int *m,FILE *file){
    fscanf(file,"%d %d",n,m);
    printf("%d %d",*n,*m);
    int id=0;
    a=(double *)malloc(((*n)*(*m))*sizeof(double));
    int i=1;
    char line[500];   
    double val;
    printf("\n");
    fgets(line,sizeof(line),file) ;
    fgets(line,sizeof(line),file) ;
    printf("%s",line);
    while (fscanf(file,"%lf",&a[id])) {
        if(id==(*n)*(*m))
            break;
        id++;
        i++;
        if(i%((*m)+1)==0){
            i++;
            fgets(line,sizeof(line),file) ;
            fgets(line,sizeof(line),file) ;
        }
    }
}

int main(){
    char* s;
    FILE *file=fopen("matrix.txt","r");
    int n,m;
    input_array(&n,&m,file);
    for(int i=0;i<n*m;i++)
        printf("%lf \n",a[i]);
}
