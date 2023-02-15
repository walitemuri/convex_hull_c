## CIS-3490 A2

Wali Temuri, 1183379

## Description

### 1.1 - Brute Force Algorithm for Counting Inversions in Array

The file P11.c contains a brute force implementation for counting the number of inversions in a given array,
the algorithm has O(n^2) complexity.

Inversion Definition: every pair of (A[i], A[j]) for i < j.

#### Compilation

To compile the c file simply run the following command:

```
make
```

To run the program run:

```
./P11
```

### 1.2 - Divide and Conquer Algorithm for Counting Inversions in Array

The file P12.c contains a divide and conquer implementation for counting the number of inversions in a given array,
the algorithm has O(nlgn) complexity.

Inversion Definition: every pair of (A[i], A[j]) for i < j.

#### Compilation

To compile the c file simply run the following command:

```
make
```

To run the program run:

```
./P12
```

### 2.1 - Brute Force Algorithm to find Convex Points and Shortest Path

The file P21.c contains the brute force implementation to find the convex points in a given 2D plane of points,
then the shortest path is found using another brute force implementation.

The time complexity of the brute force convex point algorithm is O(n^3) and the shortest path algorithm is O(n!).

#### Compilation

To compile the program run:

```
make
```

To run the program run:

```
./P21
```

### 2.2 - Divide and Conquer Algorithm to find Convex Points and Shortest Path

The file P21.c contains the divide and conquer implementation to find the convex points in a given 2D plane of points,
then the shortest path is found using a brute force implementation.

The time complexity of the brute force convex point algorithm is O(nlgn) and the shortest path algorithm is O(n!).

#### Compilation

To compile the program run:

```
make
```

To run the program run:

```
./P22
```
