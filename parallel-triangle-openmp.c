#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

int num_points; // Change this to the number of points in your dataset
int num_threads; // Number of threads to use in parallelization
int bins[100] = {0}; // Histogram bins for side lengths
double side_lengths[3]; // Side Lengths of each triangle
#define BOUNDS 101 // Change this to change the boundary box (Points generated within 0 to BOUNDS-1)
#define MAX_SIDE_LEN 20 // Max triangle side length

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

// Function to fill array with random points and initialize other variables
void generate_points( Point* points) {
    int i;
    unsigned int seed = time(NULL);

    for (i = 0; i < num_points; i++) {
		Point p;
		p.x = (double)(rand_r(&seed) % BOUNDS);
		p.y = (double)(rand_r(&seed) % BOUNDS);
		p.z = (double)(rand_r(&seed) % BOUNDS);
		points[i] = p;
    }
}

// Function to find triangles in given array of points
void find_triangles( Point* points) {
    
    omp_set_num_threads(num_threads);
    
    #pragma omp parallel reduction(+:bins)
    {
    int startPos = (omp_get_thread_num()) * (num_points / num_threads);
    int endPos = startPos + (num_points / num_threads);
    
    // Iterate through all combinations of three points
    #pragma omp for
    for (int i = startPos; i < endPos; i++) {
        for (int j = i + 1; j < num_points; j++) {
                side_lengths[0] = distance(points[i], points[j]);
                if( side_lengths[0] < MAX_SIDE_LEN ){
                    for (int k = j + 1; k < num_points; k++) {
                        side_lengths[0] = distance(points[i], points[j]);
                        side_lengths[1] = distance(points[j], points[k]);
                        side_lengths[2] = distance(points[k], points[i]);

                        // Check if all side lengths are less than MAX_SIDE_LEN
                        if (side_lengths[0] < MAX_SIDE_LEN && side_lengths[1] < MAX_SIDE_LEN && side_lengths[2] < MAX_SIDE_LEN) {
                            // Increment the histogram bins based on side lengths
                            for (int bin = 0; bin < 3; bin++) {
                                bins[((int)(side_lengths[bin] * 100)) % 100]++;
                            }
                        }
                    }
                }
        }
    }
    }
}

void print_results( double time_elapsed ){
    int triangle_count;
    for ( int i = 0; i < 100; i++) {
        triangle_count += bins[i];
        printf("Bin %d: %d\n", i, bins[i]);
    }
    printf("Count of Triangles: %d\n Time Elapsed (ms): %f\n", triangle_count/3, time_elapsed);
}

int main(int argc, char* argv[]) {
	struct timeval t1, t2;
	double elapsedTime;
    
    num_points = atoi(argv[1]); // Set Num Points
    num_threads = atoi(argv[2]);
    Point points[num_points]; // Holds the data for the 3-D points
    // Start Timer
    gettimeofday(&t1, NULL); // Start Timer

    // Call function to generate points
	generate_points( points );
    // Call function to find triangles
    find_triangles( points );
    
    // Stop Timer
	gettimeofday(&t2, NULL); // Stop Timer

    // Print results
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
    print_results( elapsedTime );

    return 0;
}
