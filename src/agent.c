#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/matrix.h"
#include "../include/utils.h"
#include "../include/agent.h"

int main(){

	int cols = 0, rows = 0;
	float default_value = 0.0;
	int i,j;
	char filename[12] = "entrada.txt";
	MATRIX *matrix;


	UTILS_parse_parameters(filename, &rows, &cols, &default_value);
	if(default_value == -1){
		return 0;
	}
	printf("rows: %d cols: %d default value: %f\n", rows, cols, default_value);

	matrix = MATRIX_new(rows, cols);

	UTILS_parse_grid_world(filename, matrix, default_value);
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			printf("state: %c v: %0.2f ", matrix->matrix[i][j].state,
				       	matrix->matrix[i][j].value);
		}
		printf("\n");
	}

	return 0;
}
