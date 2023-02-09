#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ROWS 10000

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

long long *integerArray(char *fileName)
{
    long long *arrayOfInts = malloc(sizeof(long long) * ROWS * 5);
    if (arrayOfInts == NULL)
    {
        printf("Error: Couldn't allocate memory for array\n");
        exit(-1);
    }

    FILE *f = fopen(fileName, "r");
    if (f == NULL)
    {
        free(arrayOfInts);
        printf("Error: Couldn't open file '%s'\n", fileName);
        exit(-1);
    }

    unsigned int i = 0;
    while (fscanf(f, "%lld", &arrayOfInts[i]) == 1)
    {
        i++;
    }

    fclose(f);
    return arrayOfInts;
}

int main(int argc, char *argv[])
{
    long long *A;
    A = integerArray("q1_data.dat");
    clock_t begin = clock();
    long count = countInversionsBrute(A, ROWS * 5);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Number of Inversions: %ld\nTime Spent (Brute): %.3lfs\n", count, time_spent);
    sleep(1);
    printf("\n");
    begin = clock();
    count = countInversionsDiv(A, ROWS * 5);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Number of Inversions: %ld\nTime Spent (DIV/CONQ): %.3lfs\n", count, time_spent);
    free(A);
    return 0;
}
