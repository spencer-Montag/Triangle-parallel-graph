#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUM_POINTS 1000 // Change this to the number of points in your dataset
#define BOUNDS 11 // Change this to change the boundary box (Points generated within 0 to BOUNDS-1)
#define MAX_SIDE_LEN 1 // Max triangle side length

// Define a structure for 3D points
typedef struct point {
    double x, y, z;
} Point;

// Function to calculate the Euclidean distance between two points
double distance( Point p1, Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    double dz = p1.z - p2.z;
    return sqrt((double)(dx * dx + dy * dy + dz * dz));
}

// Function to fill array with random points
void fill_points( Point* points) {
    int i;
    srand(time(0));

    for (i = 0; i < NUM_POINTS; i++) {
		Point p;
		p.x = (double)(rand() % BOUNDS);
		p.y = (double)(rand() % BOUNDS);
		p.z = (double)(rand() % BOUNDS);
		points[i] = p;
    }
}

int main() {
    int count = 0;
	
	// Initialize and fills points array with data
    Point points[NUM_POINTS];  // Assuming you have an array of 3D points
	fill_points( points );

    // Histogram bins for side lengths r12, r23, and r31
    int bins[100] = {0};  // Adjust the size as desired

    // Iterate through all combinations of three points
    for (int i = 0; i < NUM_POINTS; i++) {
        for (int j = i + 1; j < NUM_POINTS; j++) {
            for (int k = j + 1; k < NUM_POINTS; k++) {
                double side_lengths[3];
                side_lengths[0] = distance(points[i], points[j]);
                side_lengths[1] = distance(points[j], points[k]);
                side_lengths[2] = distance(points[k], points[i]);

                // Check if all side lengths are less than MAX_SIDE_LEN
                if (side_lengths[0] < MAX_SIDE_LEN && side_lengths[1] < MAX_SIDE_LEN && side_lengths[2] < MAX_SIDE_LEN) {
                    // Increment the histogram bins based on side lengths
                    count++;
                    for (int bin = 0; bin < 3; bin++) {
                        bins[((int)(side_lengths[bin] * 100)) % 100]++;
                    }
                }
            }
        }
    }

    // Print or use the histogram data as needed
    for ( int i = 0; i < 100; i++) {
        printf("Bin %d: %d\n", i, bins[i]);
    }
    printf("%d\n", count);

    return 0;
}
