/*
File Owned By: Wali Temuri 1183379
CIS - A -
Email: wtemuri@uoguelph.ca
Date Created:
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_POINTS 30000

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

bool IsOnSameSide(Point p1, Point p2, Point p3)
{
    return ((p3.y - p1.y) * (p2.x - p1.x) - (p3.x - p1.x) * (p2.y - p1.y)) < 0;
}

Point *BruteHull(Point *points, unsigned int n)
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
                if (k == i || k == j)
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
                    convexHull = realloc(convexHull, (count + 1) * 2 * sizeof(Point));
                    convexHull[count] = points[i];
                    count++;
                }
                partofHull = false;
            }
        }
    }

    return convexHull;
}

int main(int argc, char *argv[])
{
    // Point *points = ReadPoints("data_A2_Q2.txt");
    Point *testPoints = malloc(sizeof(Point) * 17);

    testPoints[0].x = 4.0;
    testPoints[0].y = 5.0;

    testPoints[1].x = 5.0;
    testPoints[1].y = 4.0;

    testPoints[2].x = 5.0;
    testPoints[2].y = 3.0;

    testPoints[3].x = 3.85;
    testPoints[3].y = 1.93;

    testPoints[4].x = 1.0;
    testPoints[4].y = 5.0;

    testPoints[5].x = 0.64;
    testPoints[5].y = 1.21;

    testPoints[6].x = 2.66;
    testPoints[6].y = 1.17;

    testPoints[7].x = 2.51;
    testPoints[7].y = 5.1;

    testPoints[8].x = 0.78;
    testPoints[8].y = 3.25;

    testPoints[9].x = 2.76;
    testPoints[9].y = 3.27;

    testPoints[10].x = 3.68;
    testPoints[10].y = 3.42;

    testPoints[11].x = 2.54;
    testPoints[11].y = 2.34;

    testPoints[12].x = 2.21;
    testPoints[12].y = 3.96;

    testPoints[13].x = 1.70;
    testPoints[13].y = 2.66;

    testPoints[14].x = 4.76;
    testPoints[14].y = 1.74;

    testPoints[15].x = 0.67;
    testPoints[15].y = 2.23;

    testPoints[16].x = 2.01;
    testPoints[16].y = 1.82;

    Point *convexHull = BruteHull(testPoints, 17);

    for (int i = 0; i < 11; i++)
    {
        printf("%.4lf,%.4lf\n", convexHull[i].x, convexHull[i].y);
    }

    free(testPoints);
    free(convexHull);
    return 0;
}