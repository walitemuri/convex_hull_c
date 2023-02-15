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

/* Helper Function which evaluates which side of the line the points is located */
bool IsOnSameSide(Point p1, Point p2, Point p3)
{
    return ((p3.y - p1.y) * (p2.x - p1.x) - (p3.x - p1.x) * (p2.y - p1.y)) < 0;
}

/*
Function: ReadPoitns
In: filename
Out: Ponint * points
Post: Returns address of points array containing all the points in file
*/
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

/*
Function: BruteHull
In: Point * points, unsigned int n, int * bruteCount
Out: Point *convexHull
Post: Returns an array of points containing the convex hull points
*/
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

/* Helper function in order to calculate distance using simple geometery rules */
double distance(Point a, Point b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

// This function finds the shortest path that connects all points in the input array
// Given a list of extreme points of the convex hull, find the shortest path
// from s1 to s2 along the hull
void shortest_path(Point s1, Point s2, Point *hull, int n)
{
    int i, j;
    double dist, min_dist = INFINITY;

    // Find the indices of s1 and s2 in the hull
    int i1 = -1, i2 = -1;
    for (i = 0; i < n; i++)
    {
        if (hull[i].x == s1.x && hull[i].y == s1.y)
            i1 = i;
        if (hull[i].x == s2.x && hull[i].y == s2.y)
            i2 = i;
    }

    // Allocate an array to store the shortest path points
    Point *path = (Point *)malloc(n * sizeof(Point));

    // Traverse the hull in both directions from s1 to s2
    int path_len = 0;
    i = i1;
    j = i2;
    while (i != j)
    {
        // Add the current point on the hull to the path
        path[path_len++] = hull[i];

        // Compute the length of the current segment of the hull
        dist = distance(hull[i], hull[(i + 1) % n]);

        // Check if the current segment is shorter than the current minimum
        if (dist < min_dist)
            min_dist = dist;

        // Move to the next point on the hull in the clockwise direction
        i = (i + 1) % n;
    }
    // Add the last point on the hull to the path
    path[path_len++] = hull[i];

    // Traverse the hull in the opposite direction from s2 to s1
    j = i2;
    while (j != i)
    {
        // Add the current point on the hull to the path
        path[path_len++] = hull[j];

        // Compute the length of the current segment of the hull
        dist = distance(hull[j], hull[(j + 1) % n]);

        // Check if the current segment is shorter than the current minimum
        if (dist < min_dist)
            min_dist = dist;

        // Move to the next point on the hull in the counterclockwise direction
        j = (j + n - 1) % n;
    }

    // Print the shortest path points
    printf("Shortest path from (%g, %g) to (%g, %g):\n", s1.x, s1.y, s2.x, s2.y);
    for (i = 0; i < path_len; i++)
        printf("(%g, %g)\n", path[i].x, path[i].y);
    printf("Length: %g\n", min_dist);
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

    Point p1;
    Point p2;

    printf("Enter s1 x and y seperated by space:\n");
    scanf(" %lf %lf", &p1.x, &p1.y);
    printf("Enter s2 x and y seperated by space:\n");
    scanf(" %lf %lf", &p2.x, &p2.y);

    shortest_path(p1, p2, convexHull, *bruteCount);
    double elapsed = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Elapsed time: %lf seconds\n", elapsed);

    free(bestPath);
    free(points);
    free(convexHull);
    free(bruteCount);
}
