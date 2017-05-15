#include <stddef.h>
#include <float.h>
#include "Signal.h"
#include "dynamicTimeWarping.h"

#include <stdio.h>

typedef struct vector_t{
	double* components;
	size_t size;
} vector;

static double** newMatrix(const size_t, const size_t);
static size_t min2size_t(const size_t, const size_t);
static size_t max2size_t(const size_t, const size_t);
static double min3doubles(const double, const double, const double);
static void freeMatrix(double**, const size_t);
static double meanOneNormDistance(const vector*, const vector*);

/** ------------------------------------------------------------------------ *
 * Compute the dynamic time warp distance between two signals subject to a
 * locality constraint
 *
 * PARAMETERS
 * signal1       A pointer to the first Signal object.
 * signal2       A pointer to the second Signal object.
 * locality      The maximum shift between the matching
 *
 * RETURN
 * dtw           The DTW score for this pair of signals.
 *               If the locality constraint prevent the dtw from computing
 *               the score, return DBL_MAX.
 * ------------------------------------------------------------------------- */
double dtw(Signal* s1, Signal* s2, size_t locality){
	// Checking that locality doesn't prevent calculation result
	size_t maxSize = max2size_t(s1->size, s2->size);
	size_t minSize = min2size_t(s1->size, s2->size);
	
	// Cap the locality at maxSize to prevent underflow in maxSize - locality
	locality = locality > maxSize ? maxSize : locality;
	if(maxSize - locality > minSize)
		return DBL_MAX;

	// Build the vectors of 13 coefficients to be compared
	if(s1->n_coef != s2->n_coef)
		return DBL_MAX;
	double c1[s1->n_coef];
	double c2[s2->n_coef];
	
	// Initializing cost matrix
	size_t height = s1->size + 1;
	size_t width = s2->size + 1;
	double** DTWcosts = newMatrix(height, width);
	for(size_t i = 0; i < height; i++)
		for(size_t j = 0; j < width; j++)
			DTWcosts[i][j] = DBL_MAX;
	DTWcosts[0][0] = 0;

	for(size_t i = 1; i < height; i++)
	{
		for(size_t j = max2size_t(1, i < locality ? 0 : i-locality); j < min2size_t(width, i+locality); j++)
		{
			for(size_t k = 0; k < s1->n_coef; k++)
			{
				c1[k] = s1->mfcc[k][i-1];
				c2[k] = s2->mfcc[k][j-1];
			}
			vector v1 = {c1, s1->n_coef};
			vector v2 = {c2, s2->n_coef};
			// The base cost to associate two data points = the distance between them
			double baseCost = meanOneNormDistance(&v1, &v2);
			// Actual cost is base cost + minimal previous cost to get there
			DTWcosts[i][j] = baseCost + min3doubles(DTWcosts[i-1][j],
													DTWcosts[i][j-1],
													DTWcosts[i-1][j-1]);

		}
	}

	double totalDistance;
	printf("Calculated total distance: %lf\n", totalDistance = DTWcosts[height-1][width-1]);
	freeMatrix(DTWcosts, height);
	return totalDistance;// TODO
}

/** ------------------------------------------------------------------------ *
 * Return the maximum of two unsigned values
 *
 * PARAMETERS
 * a             The first value
 * b             The second value
 *
 * RETURN
 *               The maximum of the two values
 * ------------------------------------------------------------------------- */
static size_t max2size_t(const size_t a, const size_t b){
	return a > b ? a : b;
}

/** ------------------------------------------------------------------------ *
 * Return the minimum of two unsigned values
 *
 * PARAMETERS
 * a             The first value
 * b             The second value
 *
 * RETURN
 *               The minimum of the two values
 * ------------------------------------------------------------------------- */
static size_t min2size_t(const size_t a, const size_t b){
	return a < b ? a : b;
}

/** ------------------------------------------------------------------------ *
 * Return the minimum of three double values
 *
 * PARAMETERS
 * a             The first value
 * b             The second value
 * c             The third value
 *
 * RETURN
 *               The minimum of the three values
 * ------------------------------------------------------------------------- */
static double min3doubles(const double a, const double b, const double c){
	double minAB = (a < b ? a : b);
	return minAB < c ? minAB : c;
}
/** ------------------------------------------------------------------------ *
 * Computes the mean one norm distance between two vectors
 * (the sum of the distances of their components divided by the number of components)
 *
 * PARAMETERS
 * v1       A pointer to the first vector
 * v2       A pointer to the second vector
 *
 * RETURN
 *          The mean one norm distance between the two vectors
 *          or -1 if the vectors are of different sizes
 * ------------------------------------------------------------------------- */
static double meanOneNormDistance(const vector* v1, const vector* v2){
	if(v1->size != v2->size)
		return -1;

	double sumOfDists = 0;
	for(size_t i = 0; i < v1->size; i++){
		sumOfDists += abs(v1->components[i] - v2->components[i]);
	}

	return (sumOfDists/v1->size);
}

/** ------------------------------------------------------------------------ *
 * Create a height * width matrix
 *
 * PARAMETERS
 * height        The number of lines
 * width         The number of columns
 * locality      The maximum shift between the matching
 *
 * RETURN
 * matrix        A new matrix
 * ------------------------------------------------------------------------- */
static double** newMatrix(const size_t height, const size_t width){
	double** matrix = malloc(sizeof(double*)*height);
	if(!matrix)
		return NULL;
	size_t i;
	bool error = false;
	for(i = 0; i<height && !error; i++){
		matrix[i] = malloc(sizeof(double)*width);
		if(!matrix[i])
			error = true;
	}
	if(error){
		while(i>0){
			i--;
			free(matrix[i]);
		}
		free(matrix);
		return NULL;
	}

	return matrix;
}

/** ------------------------------------------------------------------------ *
 * Free the matrix
 *
 * PARAMETERS
 * matrix      The matrix to free
 *
 * ------------------------------------------------------------------------- */
static void freeMatrix(double** matrix, const size_t height){
	if(!matrix)
		return;
	for(size_t i = 0; i<height; i++)
		if(matrix[i])
			free(matrix[i]);
	free(matrix);
}