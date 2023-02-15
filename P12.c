#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 10000

/*
Function: countInversionsDiv
In: long long * A, unsigned int n
Out: long long rightInv + leftInv +mergeInv
Post: Returns number of Inversions in array using Divide and Conquer
*/
long long countInversionsDiv(long long *A, unsigned int n)
{
    if (n <= 1)
        return 0;

    unsigned int mid = n / 2;
    long long leftInv = countInversionsDiv(A, mid);
    long long rightInv = countInversionsDiv(A + mid, n - mid);

    long long mergeInv = 0;
    unsigned int i = 0, j = mid, k = 0;
    long long *temp = (long long *)malloc(n * sizeof(long long));

    while (i < mid && j < n)
    {
        if (A[i] <= A[j])
        {
            temp[k++] = A[i++];
        }
        else
        {
            temp[k++] = A[j++];
            mergeInv += mid - i;
        }
    }

    while (i < mid)
    {
        temp[k++] = A[i++];
    }
    while (j < n)
    {
        temp[k++] = A[j++];
    }

    for (unsigned int l = 0; l < n; l++)
    {
        A[l] = temp[l];
    }

    free(temp);
    return leftInv + rightInv + mergeInv;
}

/*
Function: numsArray
In: char * fileName
Out: long long * arrayOfNums
Post: Returns array of numbers read from the file.
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

/* Main function */
int main(int argc, char *argv[])
{
    long long *A;
    A = numsArray("data_A2_Q1.txt");
    struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);
    long numOfInversions = countInversionsDiv(A, ROWS * 5);
    printf("Number of inversions using Div/Conq: %ld\n", numOfInversions);
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Elapsed time: %lf seconds\n", elapsed);
    free(A);
    return 0;
}
