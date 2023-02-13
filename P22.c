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

Point findLeftMost(Point *points, int count)
{
    Point leftMost = points[0];
    for (int i = 1; i < count; i++)
    {
        if (points[i].x < leftMost.x)
        {
            leftMost = points[i];
        }
    }
    return leftMost;
}

Point findRightMost(Point *points, int count)
{
    Point rightMost = points[0];
    for (int i = 1; i < count; i++)
    {
        if (points[i].x > rightMost.x)
        {
            rightMost = points[i];
        }
    }
    return rightMost;
}

double crossProduct(Point A, Point B, Point C)
{
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

int upperHull(Point *points, Point *convexHull, int count, Point A, Point B, int quickIndex)
{
    int maxIndex = 0;
    double maxDistance = 0;

    for (int i = 0; i < count; i++)
    {
        double distance = crossProduct(A, B, points[i]);
        if (distance > maxDistance)
        {
            maxDistance = distance;
            maxIndex = i;
        }
    }

    if (maxDistance > 0)
    {
        convexHull[quickIndex++] = points[maxIndex];

        quickIndex = upperHull(points, convexHull, count, A, points[maxIndex], quickIndex);
        quickIndex = upperHull(points, convexHull, count, points[maxIndex], B, quickIndex);
    }

    return quickIndex;
}

int lowerHull(Point *points, Point *convexHull, int count, Point A, Point B, int quickIndex)
{
    int maxIndex = 0;
    double maxDistance = 0;

    for (int i = 0; i < count; i++)
    {
        double distance = crossProduct(A, B, points[i]);
        if (distance < maxDistance)
        {
            maxDistance = distance;
            maxIndex = i;
        }
    }

    if (maxDistance < 0)
    {
        convexHull[quickIndex++] = points[maxIndex];

        quickIndex = lowerHull(points, convexHull, count, A, points[maxIndex], quickIndex);
        quickIndex = lowerHull(points, convexHull, count, points[maxIndex], B, quickIndex);
    }

    return quickIndex;
}

int quickHull(Point *points, Point *convexHull, int count)
{
    Point leftMost = findLeftMost(points, count);
    Point rightMost = findRightMost(points, count);

    int quickIndex = 0;
    convexHull[quickIndex++] = leftMost;
    convexHull[quickIndex++] = rightMost;

    // Recursively find the upper hull
    quickIndex = upperHull(points, convexHull, count, leftMost, rightMost, quickIndex);

    // Recursively find the lower hull
    quickIndex = lowerHull(points, convexHull, count, leftMost, rightMost, quickIndex);

    return quickIndex;
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
    Point *convexHull = malloc(sizeof(Point) * 300);
    int quickCount;
    struct timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);
    quickCount = quickHull(points, convexHull, MAX_POINTS);
    clock_gettime(CLOCK_REALTIME, &end);

    printf("\n================ Convex hull ==================\n");
    for (int i = 0; i < quickCount; i++)
    {
        printf("x-value: %.4lf, y-value: %.4lf\n", convexHull[i].x, convexHull[i].y);
    }

    printf("\nTotal Points: %d\n", quickCount);
    Point *bestPath = malloc(sizeof(Point) * 100);
    printf("\nShortest Path:\n");

    shortest_path(convexHull, quickCount, bestPath);

    for (int i = 0; i < quickCount; i++)
    {
        printf("(%lf, %lf)\n", bestPath[i].x, bestPath[i].y);
    }

    double elapsed = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Elapsed time: %lf seconds\n", elapsed);

    free(bestPath);
    free(points);
    free(convexHull);
}
