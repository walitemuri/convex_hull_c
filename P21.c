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
