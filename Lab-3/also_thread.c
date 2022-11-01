#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<math.h>
#include<sys/time.h>
#include<string.h>
#include<pthread.h>

pthread_mutex_t lock;

// maximum size of matrix
#define MAX 1000

int A[MAX][MAX];
int B[MAX][MAX];
int Final[MAX][MAX];
int completed_rows = 0;


void get_divisions(int rows, int threads, int* divisions)
{
    int segment, surplus, i;

    surplus = (rows%threads);
    rows = rows - (rows%threads);
    segment = rows/threads;

    for (i=0; i<threads; i++)
    {
        if (surplus>0)
        {
            divisions[i] = segment+1;
            surplus--;
        }
        else divisions[i] = segment;
    }
}

void* multiply_matrices(void* bunch)
{
	int chunk = (int) bunch;
	int i, j, k;

    pthread_mutex_lock(&lock);
	int start = completed_rows;
	completed_rows = completed_rows+chunk;
	int stop = completed_rows;
    pthread_mutex_unlock(&lock);

	// For each row in the chunk
	for (i=start; i<stop; i++)
	{
		// For each column in the row of matrix 1
		for (k=0; k<MAX; k++)
		{
			// For each column of matrix 2
			for (j=0; j<MAX; j++)
			{
				Final[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void* multiply_matrices2()
{
	int i, j, k;

    pthread_mutex_lock(&lock);
	i = completed_rows;
	completed_rows++;
    pthread_mutex_unlock(&lock);

	// For each column in the row of matrix 1
	for (k=0; k<MAX; k++)
	{
		// For each column of matrix 2
		for (j=0; j<MAX; j++)
		{
			Final[i][j] += A[i][k] * B[k][j];
		}
	}
}
//851585
int main()
{
	int i,j;
	int num_threads = 500;
	int* segment_sizes = (int*) malloc(num_threads*sizeof(int));
	clock_t start, stop;

	// ================================================================================
	// Temporary decalrarion of matrices
	for (i=0; i<MAX; i++)
	{
		for (j=0; j<MAX; j++)
		{
			A[i][j] = 0;
			B[i][j] = 0;
		}	
	}

	for (i=0; i<MAX; i++)
	{
		A[i][i] = 1;
		B[i][i] = 1;	
	}
	// ================================================================================
	int time=0;
	int ii;
	// Create the threads
	//printf("1");
	for(int i=0;i<100;i++){
		pthread_t* working_threads = (pthread_t*) malloc(num_threads*sizeof(pthread_t));
		start = clock();
		// Multiply and wait
		for (ii=0; ii<MAX; ii++) pthread_create(&working_threads[ii%num_threads],NULL,multiply_matrices,NULL);
		for (ii=0; ii<num_threads; ii++) pthread_join(working_threads[ii],NULL);

		stop = clock();
		time+=(stop-start);
		printf("%d %d\n",i,time);
	}
	// for (i=0; i<MAX; i++)
	// {
	// 	for (j=0; j<MAX; j++) printf("%d ", Final[i][j]);	
	// 	printf("\n");
	// }
	time/=100;
	printf("Time = %lu\n", time);
}


// int main()
// {
// 	int i,j;
// 	int num_threads = 8;
// 	int* segment_sizes = (int*) malloc(num_threads*sizeof(int));
// 	clock_t start, stop;

// 	// Get the segment sizes
// 	get_divisions(MAX, num_threads, segment_sizes);

// 	// Temporary print
// 	for (i=0; i<num_threads; i++)
// 	{
// 		printf("%d ", segment_sizes[i]);
// 	}
// 	printf("\n");

// 	// ================================================================================
// 	// Temporary decalrarion of matrices
// 	for (i=0; i<MAX; i++)
// 	{
// 		for (j=0; j<MAX; j++)
// 		{
// 			A[i][j] = 0;
// 			B[i][j] = 0;
// 		}	
// 	}

// 	for (i=0; i<MAX; i++)
// 	{
// 		A[i][i] = 1;
// 		B[i][i] = 1;	
// 	}
// 	// ================================================================================
	
// 	// Create the threads
// 	pthread_t* working_threads = (pthread_t*) malloc(num_threads*sizeof(pthread_t));
	
// 	start = clock();

// 	// Multiply and wait
// 	for (i=0; i<num_threads; i++) pthread_create(&working_threads[i],NULL,multiply_matrices,(void *)segment_sizes[i]);
// 	stop = clock();
// 	for (i=0; i<num_threads; i++) pthread_join(working_threads[i],NULL);

// 	stop = clock();

// 	// for (i=0; i<MAX; i++)
// 	// {
// 	// 	for (j=0; j<MAX; j++) printf("%d ", Final[i][j]);	
// 	// 	printf("\n");
// 	// }

// 	printf("Time = %lu\n", stop-start);
// }
