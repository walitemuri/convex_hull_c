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
void shortest_path(Point points[], int n, Point *best_path)
{
    // Initialize the best distance to the maximum possible distance
    double best_distance = MAX_DISTANCE;

    int *path = (int *)malloc(n * sizeof(int));
    bool *used = (bool *)calloc(n, sizeof(bool));

    // For each point as a starting point in the path
    for (int i = 0; i < n; i++)
    {
        // Set the first point in the path to the current point
        path[0] = i;

        // Mark the current point as visited
        used[i] = true;

        // Initialize the current index to 1
        int curr = 1;

        // For each remaining point in the path
        for (int j = 0; j < n - 1; j++)
        {
            // Initialize the current index and minimum distance to invalid values
            int curr_index = -1;
            double curr_min = MAX_DISTANCE;

            // For each unused point
            for (int k = 0; k < n; k++)
            {
                if (!used[k])
                {
                    // Compute the distance between the current point and the unused point
                    double curr_distance = distance(points[path[j]], points[k]);

                    // If the distance is less than the minimum distance found so far,
                    // update the minimum distance and current index
                    if (curr_distance < curr_min)
                    {
                        curr_min = curr_distance;
                        curr_index = k;
                    }
                }
            }

            // Add the closest unused point to the path and mark it as visited
            path[curr++] = curr_index;
            used[curr_index] = true;
        }

        // Compute the total distance of the current path
        double curr_distance = 0;
        for (int j = 1; j < n; j++)
        {
            curr_distance += distance(points[path[j - 1]], points[path[j]]);
        }

        // If the current path is shorter than the best path found so far,
        // update the best path and the best distance
        if (curr_distance < best_distance)
        {
            best_distance = curr_distance;
            for (int j = 0; j < n; j++)
            {
                best_path[j] = points[path[j]];
            }
        }

        // Reset the used array for the next starting point
        for (int j = 0; j < n; j++)
        {
            used[j] = false;
        }
    }
    free(path);
    free(used);
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
