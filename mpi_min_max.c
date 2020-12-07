#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define N 11

int findMin(int arr[], int size, int my_rank);
int findMax(int arr[], int size, int my_rank);

int main(int argc, char *argv[])
{
    int my_rank;
    int size;
    int p;
    MPI_Status status;

    int input[] = {45, 6, 7, 23, -12, 73, -21, 58, 99, 22, 45};
    int *sub_input;
    int size_per_proc;
    int sub_min;
    int sub_max;
    int *min_array;
    int *max_array;
    int min,max;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    size_per_proc = (int)ceil((double)N / p);
    if (my_rank == 0)
    {
        min_array = (int *)malloc(sizeof(int) * p);
        max_array = (int *)malloc(sizeof(int) * p);
    }

    sub_input = (int *)malloc(sizeof(int) * size_per_proc);

    MPI_Scatter(input, size_per_proc, MPI_INT,
                sub_input, size_per_proc, MPI_INT,
                0, MPI_COMM_WORLD);

    sub_min = findMin(sub_input, size_per_proc, my_rank);
    sub_max = findMax(sub_input, size_per_proc, my_rank);

    MPI_Gather(&sub_min,1,MPI_INT,
                min_array, 1, MPI_INT,
                0,MPI_COMM_WORLD);

    MPI_Gather(&sub_max,1,MPI_INT,
                max_array, 1, MPI_INT,
                0,MPI_COMM_WORLD);
    
    if(my_rank == 0)
    {
        min = findMin(min_array,p,-1);
        max = findMax(max_array,p,-1);

        printf("The max of array: %d\n",max);
        printf("The min of array: %d\n", min);
    }

    MPI_Finalize();
    return 0;
}

int findMin(int arr[], int size, int my_rank)
{
    int i, min, global_i;
    min = arr[0];

    for (i = 1; i < size; i++)
    {
        if (arr[i] < min)
            min = arr[i];


        // my_rank = -1 means that the arr is completely filled
        if(my_rank == -1) continue;
        // Check whether reached the last element of input
        // If reached, stop the loop
        global_i = (my_rank + 1) * i;
        if (global_i == N - 1)
            break;
    }
    return min;
}

int findMax(int arr[], int size, int my_rank)
{
    int i, max, global_i;
    max = arr[0];

    for (i = 1; i < size; i++)
    {
        if (arr[i] > max)
            max = arr[i];

        // my_rank = -1 means that the arr is completely filled
        if(my_rank == -1) continue;
        // Check whether reached the last element of input
        // If reached, stop the loop
        if(my_rank == -1) continue;
        global_i = (my_rank + 1) * i;
        if (global_i == N - 1)
            break;
    }
    return max;
}