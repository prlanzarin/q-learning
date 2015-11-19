#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/matrix.h"

/* Matrix allocator. New matrix of sizes rows x cols */
MATRIX *MATRIX_new(int rows, int cols) {
	int i;
	MATRIX *new_mat = (MATRIX *)malloc(sizeof(MATRIX));
	new_mat->r = rows;
	new_mat->c = cols;
	if((new_mat->matrix = malloc(rows * sizeof(GRID *))) == NULL)
		return NULL;
	for(i = 0; i < rows; i++) {
		if((new_mat->matrix[i] = malloc(cols * sizeof(GRID))) == NULL)
			return NULL;
	}
	return new_mat;
}

/* checks if pos (x, y) is out of grid bounds */
int MATRIX_out_of_bounds(MATRIX *matrix, int x, int y) {
	return ((x < 0) || (x > matrix->c) || (y < 0) || (y > matrix->r));
}

/* Matrix deallocator */
void MATRIX_free(MATRIX *matrix) {
	int i;
	for(i = 0; i < matrix->r; i++)
			free((void *) matrix->matrix[i]);
	free((void *) matrix->matrix);
	free((void *) matrix);
}
