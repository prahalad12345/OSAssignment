#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
double **m1;
double **m2;
double **result;
int n_m1,m_m1,n_m2,m_m2;
int iter=0;
pthread_mutex_t lock;

void a_input_array(FILE *file){
    fscanf(file,"%d %d",&n_m1,&m_m1);
    m1 = (double**)malloc(n_m1 * sizeof(double*));
    for (int index=0;index<n_m1;++index)
    {
        m1[index] = (double*)malloc(m_m1 * sizeof(double));
    }
    char line[500];   

    fgets(line,sizeof(line),file) ;
    fgets(line,sizeof(line),file) ;
    for(int i=0; i<n_m1; i++) {
        fgets(line,sizeof(line),file) ;
        fgets(line,sizeof(line),file) ;
        for(int j=0; j<m_m1; j++) {
            fscanf(file, "%lf", &m1[i][j]);
        }   
    }
}

void b_input_array(FILE *file){
    fscanf(file,"%d %d",&n_m2,&m_m2);
    m2=(double**)malloc(n_m2*sizeof(double*));
    for (int index=0;index<n_m2;++index)
    {
        m2[index] = (double*)malloc(m_m2 * sizeof(double));
    }
    char line [100];
    
    for(int i=0; i<n_m2; i++) {
        fgets(line,sizeof(line),file) ;
        fgets(line,sizeof(line),file) ;
        for(int j=0; j<m_m2; j++) {
            fscanf(file, "%lf", &m2[i][j]);
        }
    }
}

void matrix_multiplication_with_thread(void *arg){
    pthread_mutex_lock(&lock);
    int i=iter++;

    for(int j=0;j<m_m2;j++){
        for(int k=0;k<m_m1;k++){
            result[i][j]+=m1[i][k]*m2[k][j];
        }
    }
    pthread_mutex_unlock(&lock);
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
        else if(strcmp(argv[i],"-p")==0){
            allcheck+=100;
            threads=atoi(argv[i+1]);
        }
    }

    
    if(m_m1!=n_m2){
        printf("Matrix multiplication cant be applied\n");
        return -1;
    }
    
    result = (double**)malloc(n_m1 * sizeof(double*));
    for (int index=0;index<n_m1;++index)
    {
        result[index] = (double*)malloc(m_m2 * sizeof(double));
    }
    pthread_t tid[threads];

    clock_t begin = clock();
    pthread_mutex_init(&lock, NULL);
    for (int t = 0; t < n_m1; t++){
        int *p;
        int rem=t%threads;
        pthread_create(&tid[rem], NULL, matrix_multiplication_with_thread, (void *)&p);
    }
    for (int t = 0; t < threads; t++)
        pthread_join(tid[t], NULL);
    pthread_mutex_destroy(&lock);
    clock_t end=clock();

    for(int i=0; i<n_m1; i++) {
        for(int j=0; j<m_m2; j++) {
            printf("%lf ", result[i][j]);
        }
        printf("\n");
    }

    printf("%ld second",(end-begin));
}