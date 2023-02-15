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

/*
Function: ReadPoints
In: char * filename
Out: Point * points
Post: Read points from file and populates array of points
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
Function: findLeftMost
In: Point * points, int count
Out: Point leftMost
Post: Iterates through the array of points to locate the
*/
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

/*
Function: findRightMost
In: Point * points, int count
Out: Point rightMost
Post: Iterates through the points and finds the rightmost point
*/
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

/* Helper function to calculate the cross product */
double crossProduct(Point A, Point B, Point C)
{
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

/* Recursive function desigend to find the points in the upper half of the plane */
int upperHull(Point *points, Point *convexHull, int count, Point A, Point B, int quickIndex)
{
    int maxIndex = 0;
    double maxDistance = 0;

    // Finding max distance point from the line
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

/* Recursive function desigend to find the points in the lower half of the plane */
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

/*
Function: quickHull
In: Point * points, Point * convexHull, int count
Out: int quickIndex
Post: Finds the convex hull by recursively finding the upper and lower hulls
*/
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

/* Helper function to calculate distance */
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
/* Main function */
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

    Point p1;
    Point p2;

    printf("Enter s1 x and y seperated by space:\n");
    scanf(" %lf %lf", &p1.x, &p1.y);
    printf("Enter s2 x and y seperated by space:\n");
    scanf(" %lf %lf", &p2.x, &p2.y);

    shortest_path(p1, p2, convexHull, quickCount);

    double elapsed = end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Elapsed time: %lf seconds\n", elapsed);

    free(bestPath);
    free(points);
    free(convexHull);
}
