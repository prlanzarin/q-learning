#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/matrix.h"
#include "../include/utils.h"
#include "../include/agent.h"

int main(){

	int cols = 0, rows = 0;
	int i,j;
	char filename[12] = "entrada.txt";
	MATRIX *matrix;


	if(UTILS_parse_parameters(filename, &rows, &cols, &DEFAULT_VALUE) < 0){
		return 0;
	}
	printf("rows: %d cols: %d default value: %f\n", rows, cols, DEFAULT_VALUE);

	matrix = MATRIX_new(rows, cols);

	UTILS_parse_grid_world(filename, matrix, DEFAULT_VALUE);
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			printf("state: %c v: %0.2f ", matrix->matrix[i][j].state,
				       	matrix->matrix[i][j].value);
		}
		printf("\n");
	}

	return 0;
}
