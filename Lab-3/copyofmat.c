#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

double *a;
double *b;
double *c;


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

void matrix_multiplication_without_thread(int *a,int* b,int *c){
    for(int i=0;i<100;i++){
        for(int j=0;j<100;j++){
            for(int k=0;k<100;k++){
                c[i*100+j]+=(a[i*100+k])*(b[k*100+j]);
            }
        }
    }
}


void matrix_multiplication_with_thread(void *arg){
    static int i=0;
    for(int j=0;j<100;j++){
        for(int k=0;k<100;k++){
            c[i*100+j]+=(a[i*100+k])*(b[k*100+j]);
        }
    }
    i++;
}

int main(int argc,char **argv){

    if(argc!=7){
        printf("Invalid. arguments expected 7");
        return -1;
    }

    a=(int *)malloc(10000*sizeof(int));
    b=(int *)malloc(10000*sizeof(int));
    c=(int *)calloc(10000,sizeof(int));

    for(int i=0;i<100;i++){
        for(int j=0;j<100;j++){
            a[i*100+j]=i*100+j;
            b[i*100+j]=i*100+j;
        }
    }

   matrix_multiplication_without_thread(a,b,c);

//    for(int i=0;i<100;i++)
//        printf("%d ",c[i]);
    /*
    pthread_t tid[100];

    for (int t = 0; t < 100; t++)
        pthread_create(&tid[t], NULL, matrix_multiplication_with_thread, (void *)& tid[t]);

    for (int t = 0; t < 100; t++)
        pthread_join(tid[t], NULL);
*/
    //for(int i=0;i<100;i++)
    //    printf("%d ",c[i]);
}