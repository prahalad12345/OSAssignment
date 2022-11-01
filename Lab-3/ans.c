#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<math.h>
#include<sys/time.h>
#include<string.h>
#include <sys/time.h>
#include<pthread.h>

// pthread_mutex_t lock;

double CTimer() 
{
	struct timeval tm;

	gettimeofday(&tm,NULL);
	return((double)tm.tv_sec + (double)(tm.tv_usec/1000000.0));
}

struct matrix_data
{
    float** A;
    int r1,c1;

    float** B;
    int r2,c2;

    int start;
    int end;
    float** result;
};

void* multiply_segments(void* input_data)
{
    // pthread_mutex_lock(&lock);

    // Compute for rows start to end in A
    struct matrix_data* given = (struct matrix_data*) input_data;

    int i, j, k;
    float sum;

    for (i=given->start; i<given->end; i++)
    {
        for (k=0; k<given->c2; k++)
        {
            sum = 0;
            for (j=0; j<given->c1; j++) sum = sum + (given->A[i][j] * given->B[j][k]);
            given->result[i][k] = sum;
        }
    }

    // pthread_mutex_unlock(&lock);
    return NULL;
}

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

void get_time(float** M1, int r1, int c1, float** M2, int r2, int c2, float** final_result, int thread_count)
{
    int start=0, i, j;

    // Get the divisions
    int* divisions = (int*) malloc(thread_count * sizeof(int));
    get_divisions(r1, thread_count, divisions);

    // Create some threads
    pthread_t* working_threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
    /*for (i=0; i<thread_count; i++)
    {
        pthread_t t;
        working_threads[i] = t;
    }*/

    // Split the threads
    for(i=0; i<thread_count; i++)
    {
        // Create a struct to pass the data
        struct matrix_data m_data;
        m_data.A = M1;
        m_data.r1 = r1;
        m_data.c1 = c1;

        m_data.B = M2;
        m_data.r2 = r2;
        m_data.c2 = c2;

        m_data.start = 0;
        m_data.end = start + divisions[i];
        m_data.result = final_result;

        pthread_create(&working_threads[i],NULL,multiply_segments,(void *) &m_data);
        start = start+divisions[i]; 
    }

    // Wait for all the threads to terminate
    for (i=0; i<thread_count; i++) pthread_join(working_threads[i],NULL);

    // Display the final result
    // printf("Threads = %d\n", thread_count);

    // printf("\n%d %d\n", r1, c2);
    // for(i=0; i<r1; i++)
    // {
    //     printf("# Row %d\n", i);
    //     for(j=0; j<c2; j++) printf("%f\n", final_result[i][j]);
    // }
}

int main(int argc, char **argv)
{
    // Initialize the variables
    int i, j, cross_check=0, r1, r2, c1, c2, sample_size=20;
    char file_name_1[256], file_name_2[256], parameter[10];
    double thread_timings[100], sample;
    FILE* fp;
    clock_t start_time, stop_time;
    
    // if (pthread_mutex_init(&lock, NULL) != 0)
    // {
    //     printf("Mutex not working\n");
    //     return 1;
    // }


    // Check the number of arguments
    if (argc < 5)
    {
        printf("Please enter:\nmy_matrix_multiply -a a_matrix_file.txt -b b_matrix_file.txt\n\n");
        return 0;
    }
    
    // Store the input parameters
    for (i=1; i<=3; i=i+2)
    {
        strcpy(parameter, argv[i]);
        if (strcmp(parameter, "-a") == 0) 
        {
            strcpy(file_name_1, argv[i+1]);
            cross_check = cross_check + 1;
        }

        else if (strcmp(parameter, "-b") == 0) 
        {
            strcpy(file_name_2, argv[i+1]);
            cross_check = cross_check + 2;
        }

        else
        {
            printf("Please enter:\nmy_matrix_multiply -a a_matrix_file.txt -b b_matrix_file.txt\n\n");
            return 0;
        }

    }

    if (cross_check != 3)
    {
        printf("Please enter:\nmy_matrix_multiply -a a_matrix_file.txt -b b_matrix_file.txt\n\n");
        return 0;  
    }


    // Read the matrices

    // Matrix 2
    // Initialize the variables
    char temp[50];    

    // Open the file
    fp = fopen(file_name_1, "r");
    if (fp == NULL)
    {
        printf("A file name is invalid\n");
        exit(0);
    }

    // Read the file row, col values
    fscanf(fp, "%d %d", &r1, &c1);

    // M1
    float** M1 = (float**) malloc(r1 * sizeof(float*));
    for (i=0; i<r1; i++) M1[i] = (float*) malloc(c1 * sizeof(float));

    // Read the matrix
    i=0;
    while (i<r1)
    {
        j=0;
        while (j<c1)
        {
            fscanf(fp, "%s", temp);
            if (temp[0] == '#')
            {
                // set the file pointer to the next line
                fgets(temp, 50, fp);
                continue;
            }

            M1[i][j] = atof(temp);
            j++;
        }
        i++;
    }

    fclose(fp);

    // Matrix 1
    // Initialize the variables

    // Open the file
    fp = fopen(file_name_2, "r");
    if (fp == NULL)
    {
        printf("A file name is invalid\n");
        exit(0);
    }

    // Read the file row, col values
    fscanf(fp, "%d %d", &r2, &c2);

    // M1
    float** M2 = (float**) malloc(r2 * sizeof(float*));
    for (i=0; i<r2; i++) M2[i] = (float*) malloc(c2 * sizeof(float));

    // Read the matrix
    i=0;
    while (i<r2)
    {
        j=0;
        while (j<c2)
        {
            fscanf(fp, "%s", temp);
            if (temp[0] == '#')
            {
                // set the file pointer to the next line
                fgets(temp, 50, fp);
                continue;
            }

            M2[i][j] = atof(temp);
            j++;
        }
        i++;
    }

    fclose(fp);

    // Result matrix
    float** final_result = (float**) malloc(r1 * sizeof(float*));
    for (i=0; i<r1; i++) final_result[i] = (float*) malloc(c2 * sizeof(float));

    // Make the output file
    fp = fopen("output.txt", "w");

    for (i=1; i<=100; i++) 
    {
        sample = 0;
        for (j=0; j<sample_size; j++)
        {
            // Start the timer
            start_time = clock();

            get_time(M1, r1, c1, M2, r2, c2, final_result, i);

            // Stop the timer
            stop_time = clock();

            // sample = sample + ((double) (stop_time - start_time))/CLOCKS_PER_SEC;

            printf("threads = %d, time = %f\n", i, ((double) (stop_time - start_time))/CLOCKS_PER_SEC);

            sample = sample + ((double) (stop_time - start_time))/CLOCKS_PER_SEC;
        }
        thread_timings[i] = sample/sample_size;
    }

    fprintf(fp, "Threads, time (in microseconds)\n");
    for (i=1; i<=100; i++) fprintf(fp, "%d,%f\n", i, thread_timings[i-1]);

    fclose(fp);

    return 0;
}