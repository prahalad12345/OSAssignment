#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
double *a;
double *b;
double *c;
int an,am,bn,bm;
int iter=-1;
pthread_mutex_t lock;

void a_input_array(FILE *file){
    fscanf(file,"%d %d",&an,&am);
    int id=0;
    a=(double *)malloc(((an)*(am))*sizeof(double));
    int i=1;
    char line[500];   


    fgets(line,sizeof(line),file) ;
    fgets(line,sizeof(line),file) ;

    while (fscanf(file,"%lf",&a[id])) {
        if(id==(an)*(am))
            break;

        id++;
        i++;
        if(i%((am)+1)==0){
            i++;
            fgets(line,sizeof(line),file) ;
            fgets(line,sizeof(line),file) ;
        }
    }
}

void b_input_array(FILE *file){
    fscanf(file,"%d %d",&bn,&bm);
    int id=0;
    b=(double *)malloc(((bn)*(bm))*sizeof(double));
    int i=1;
    char line[500];   

    fgets(line,sizeof(line),file) ;
    fgets(line,sizeof(line),file) ;
    while (fscanf(file,"%lf",&b[id])) {
        if(id==(bn)*(bm))
            break;

        id++;
        i++;
        if(i%((bm)+1)==0){
            i++;
            fgets(line,sizeof(line),file) ;
            fgets(line,sizeof(line),file) ;
        }
    }
}

void matrix_multiplication_without_thread(){
    for(int i=0;i<an;i++){
        for(int j=0;j<bm;j++){
            for(int k=0;k<am;k++){
                c[i*bm+j]+=(a[i*am+k])*(b[k*bm+j]);
                
            }
        }
    }
}


void matrix_multiplication_with_thread(void *arg){
    pthread_mutex_lock(&lock);
    int id=++iter;
    pthread_mutex_unlock(&lock);
    for(int j=0;j<bm;j++){
        for(int k=0;k<am;k++){
            c[id*bm+j]+=(a[id*am+k])*(b[k*bm+j]);
        }
    }

}

int main(int argc,char **argv){

    if(argc!=7){
        printf("Invalid. arguments expected 7");
        return -1;
    }
    FILE* fp;
    FILE *fp2;
    int threads;
    int allcheck=0;
    for(int i=1;i<=6;i+=2){
        if(strcmp(argv[i],"-a")==0){
            allcheck++;
            fp=fopen(argv[i+1],"r");
            a_input_array(fp);
        }
        else if(strcmp(argv[i],"-b")==0){
            allcheck+=10;
            fp2=fopen(argv[i+1],"r");
            b_input_array(fp2);
        }
        else if(strcmp(argv[i],"-t")==0){
            allcheck+=100;
            threads=atoi(argv[i+1]);
        }
        else{
            allcheck+=9999;
        }
    }
    if(allcheck!=111){
        printf("Some Flags are irrelevant or extra flags are added\n");
        return -1;
    }
    
    if(am!=bn){
        printf("Matrix multiplication cant be applied\n");
        return -1;
    }
    c=(double *)calloc(an*bm,sizeof(double));
    /*
    for(int i=0;i<an*am;i++){
        if(i%am==0)
            printf("\n");
        printf("%lf ",a[i]);
    }
    */
    //printf("\n");
    /*
    for(int i=0;i<bn*bm;i++){
        if(i%bm==0)
            printf("\n");
        printf("%lf ",b[i]);
    }
    */
    
    clock_t begin = clock();
    matrix_multiplication_without_thread();
    clock_t end=clock();

    //printf("%ld",(end-begin));
    //printf("\n");
    double time=0;
    pthread_t tid[threads];
    for(int j=0;j<100;j++){
    begin = clock();
    pthread_mutex_init(&lock, NULL);
    for (int t = 0; t < an; t++){
        int *p;
        int rem=t%threads;
        pthread_create(&tid[rem], NULL, matrix_multiplication_with_thread, (void *)&p);
    }
    end=clock();
    time+=(end-begin);
   //printf("%f",time);
    for (int t = 0; t < threads; t++)
        pthread_join(tid[t], NULL);
    pthread_mutex_destroy(&lock);
    }
    time/=100;
    /*
    for(int i=0;i<an*bm;i++){
       if(i%bm==0)
            printf("\n");
        printf("%lf ",c[i]);
    }
    */
    printf("%f\n",time);
}