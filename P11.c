/*
File Owned By: Wali Temuri 1183379
CIS - 2750 A2 1.1
Email: wtemuri@uoguelph.ca
Date Created: February 11, 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 10000

/*
Function: countInversionsBrute
In: long long * A, unsigned int n
Out: inv_count
Post: Counts number of inversions in array A and returns the number
*/
long countInversionsBrute(long long *A, unsigned int n)
{
    long inv_count = 0;
    unsigned int j = 0;
    for (unsigned int i = 0; i < n - 1; i++)
    {
        for (unsigned int j = i + 1; j < n; j++)
        {
            if (A[i] > A[j])
            {
                inv_count++;
            }
        }
    }

    return inv_count;
}

/*
Function: numsArray
In: char * fileName
Out: long long * arrayOfNums
Post: Alloactes and populates array by reading numbers contained in file
*/
long long *numsArray(char *fileName)
{
    long long *arrayOfNums = malloc(sizeof(long long) * ROWS * 5);
    if (arrayOfNums == NULL)
    {
        printf("Error: Couldn't allocate memory for array\n");
        exit(-1);
    }

    FILE *f = fopen(fileName, "r");
    if (f == NULL)
    {
        free(arrayOfNums);
        printf("Error: Couldn't open file '%s'\n", fileName);
        exit(-1);
    }

    unsigned int i = 0;
    while (fscanf(f, "%lld", &arrayOfNums[i]) == 1)
    {
        i++;
    }

    fclose(f);
    return arrayOfNums;
}

/* Main Function */
int main(int argc, char *argv[])
{
    long long *A;
    A = numsArray("data_A2_Q1.txt");
    struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);
    long numOfInversions = countInversionsBrute(A, ROWS * 5);
    printf("Number of inversions using Brute: %ld\n", numOfInversions);
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Elapsed time: %lf seconds\n", elapsed);
    free(A);
    return 0;
}
