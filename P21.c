#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define INF 1e9
#define MAX_POINTS 30000
#define MAX_DISTANCE 100000000.0

typedef struct Point
{
    double x, y;
} Point;

bool IsOnSameSide(Point p1, Point p2, Point p3)
{
    return ((p3.y - p1.y) * (p2.x - p1.x) - (p3.x - p1.x) * (p2.y - p1.y)) < 0;
}

int comparePoints(const void *a, const void *b)
{
    Point *p1 = (Point *)a;
    Point *p2 = (Point *)b;

    if (p1->x < p2->x)
        return -1;
    else if (p1->x > p2->x)
        return 1;
    else
        return 0;
}

Point *ReadPoints(char *filename)
{
    Point *points = (Point *)malloc(sizeof(Point) * MAX_POINTS);

    FILE *fptr = fopen(filename, "r");

    if (fptr == NULL)
    {
        printf("File Error\n");
        free(points);
        return NULL;
    }

    if (points == NULL)
    {
        fclose(fptr);
        printf("Error: Failed to allocate points array\n");
        return NULL;
    }

    for (unsigned int i = 0; i < MAX_POINTS; i++)
    {
        fscanf(fptr, "%lf %lf", &points[i].x, &points[i].y);
    }

    fclose(fptr);
    return points;
}

Point *BruteHull(Point *points, unsigned int n, int *bruteCount)
{
    Point *convexHull = NULL;

    unsigned int count = 0;
    bool partofHull = false;

    for (unsigned int i = 0; i < n; i++)
    {
        for (unsigned int j = 0; j < n; j++)
        {
            if (i == j)
                continue;

            Point p1 = points[i];
            Point p2 = points[j];

            for (unsigned int k = 0; k < n; k++)
            {
                if (k == i || k == j || (i == 0 && k < count))
                    continue;

                if (IsOnSameSide(p1, p2, points[k]))
                {
                    partofHull = true;
                }
                else
                {
                    partofHull = false;
                    break;
                }
            }
            if (partofHull)
            {
                if (count == 0)
                {
                    convexHull = (Point *)malloc((count + 1) * sizeof(Point));
                    convexHull[count] = points[i];
                    count++;
                }
                else
                {
                    convexHull = realloc(convexHull, (count + 1) * sizeof(Point));
                    convexHull[count] = points[i];
                    count++;
                }
                *bruteCount = count;
                partofHull = false;
            }
        }
    }

    return convexHull;
}

double distance(Point a, Point b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

void shortest_path(Point points[], int n, Point *best_path)
{
    double best_distance = MAX_DISTANCE;
    int *path = (int *)malloc(n * sizeof(int));
    bool *used = (bool *)calloc(n, sizeof(bool));

    for (int i = 0; i < n; i++)
    {
        path[0] = i;
        used[i] = true;
        int curr = 1;
        for (int j = 0; j < n - 1; j++)
        {
            int curr_index = -1;
            double curr_min = MAX_DISTANCE;
            for (int k = 0; k < n; k++)
            {
                if (!used[k])
                {
                    double curr_distance = distance(points[path[j]], points[k]);
                    if (curr_distance < curr_min)
                    {
                        curr_min = curr_distance;
                        curr_index = k;
                    }
                }
            }
            path[curr++] = curr_index;
            used[curr_index] = true;
        }
        double curr_distance = 0;
        for (int j = 1; j < n; j++)
        {
            curr_distance += distance(points[path[j - 1]], points[path[j]]);
        }
        if (curr_distance < best_distance)
        {
            best_distance = curr_distance;
            for (int j = 0; j < n; j++)
            {
                best_path[j] = points[path[j]];
            }
        }
        for (int j = 0; j < n; j++)
        {
            used[j] = false;
        }
    }
    free(path);
    free(used);
}

int main(int argc, char *argv[])
{
    Point *points = ReadPoints("data_A2_Q2.txt");
    int *bruteCount = malloc(sizeof(int));
    struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);
    Point *convexHull = BruteHull(points, MAX_POINTS, bruteCount);
    clock_gettime(CLOCK_REALTIME, &end);

    printf("\n================ Convex hull ==================\n");
    for (int i = 0; i < *bruteCount; i++)
    {
        printf("x-value: %.4lf, y-value: %.4lf\n", convexHull[i].x, convexHull[i].y);
    }

    printf("\nTotal Points: %d\n", *bruteCount);

    printf("\nShortest Path:\n");
    Point *bestPath = malloc(sizeof(Point) * 1000);

    shortest_path(convexHull, *bruteCount, bestPath);

    for (int i = 0; i < *bruteCount; i++)
    {
        printf("(%lf, %lf)\n", bestPath[i].x, bestPath[i].y);
    }
    double elapsed = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Elapsed time: %lf seconds\n", elapsed);

    free(bestPath);
    free(points);
    free(convexHull);
    free(bruteCount);
}
