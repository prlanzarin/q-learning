#include <stdio.h>
#include <stdlib.h>
#include "../include/matrix.h"
#include "../include/utils.h"
#include "../include/agent.h"
#include <string.h>

int states_parse(char *buffer, MATRIX *matrix, int row);
int values_parse(char *buffer, MATRIX *matrix, int row, float default_value);

/* Parse the number of rows that the matrix in 'filename' has */

int UTILS_parse_parameters(char *filename, int *rows, int *cols, 
		float *def_value) {
	FILE *file;
	if((file = fopen(filename,"r")) == NULL) {
		fprintf(stderr, "ERROR: could not open %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if(fscanf(file, "L=%d\n", rows) && fscanf(file, "C=%d\n", cols) &&
			fscanf(file, "D=%f\n", def_value)) {
		fclose(file);
		return 0;
	}
	else {
		fclose(file);
		return -1;
	}

}

/* Parses the grid world from 'filename' to 'matrix' of type MATRIX */
int UTILS_parse_grid_world(char *filename, MATRIX *matrix, float default_value) {
	int row_count = 0;
	char *buffer, line[256];
	FILE *file;
	size_t line_size = sizeof(char) * (2 * matrix->c) + 1;

	if((file = fopen(filename,"r")) == NULL){
		fprintf(stderr, "ERROR: could not open %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fgets(line, sizeof(line), file);
	fgets(line, sizeof(line), file);
	fgets(line, sizeof(line), file);

	buffer = (char *) malloc(line_size);

	while((fgets(buffer, line_size, file)) != NULL && row_count < matrix->r) {
		/* grid processing */
		states_parse(buffer, matrix, row_count);
		row_count++;
	}
	row_count = 0;

	while((fgets(buffer, line_size, file)) != NULL && row_count < matrix->r) {
		/* values processing */
		values_parse(buffer, matrix, row_count, default_value);
		row_count++;
	}

	fclose(file);
	return 0;
}
/* Writes matrix 'mat' (of type MATRIX) to file 'filename' */
int UTILS_write_matrix(char *filename, MATRIX *mat) {
	int i, j;
	FILE *file;

	if((file = fopen(filename, "w+")) == NULL) {
		fprintf(stderr, "ERROR: could not open %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fprintf(file, "L=%d\nC=%d\n", mat->r, mat->c);
	for(i = 0; i < mat->r; i++) {
		for(j = 0; j < mat->c; j++) {
			fprintf(file, "%c", mat->matrix[i][j].state);
			if(j < mat->c - 1) // last column checking, no spaces
				fprintf(file, " ");
		}
		fprintf(file, "\n");
	}
	for(i = 0; i < mat->r; i++) {
		for(j = 0; j < mat->c; j++) {
			fprintf(file, "%f", mat->matrix[i][j].value);
			if(j < mat->c - 1) // last column checking, no spaces
				fprintf(file, " ");
		}
		if(i < mat->r - 1) // last row checking, no line end
			fprintf(file, "\n");
	}
	return 0;
}

/* Auxiliary subroutine to parse a 'row' of grid states in a safe way */
int states_parse(char *buffer, MATRIX *matrix, int row) {
	char *delim = " ";
	char *tok_buf;
	int col_count = 0;

	tok_buf = strtok(buffer, delim);
	while (tok_buf != NULL) {
		matrix->matrix[row][col_count].state = *tok_buf;
		col_count++;
		tok_buf = strtok(NULL, delim);
	}
	return 0;
}

/* Auxiliary subroutine to parse a 'row' of grid states in a safe way */
int values_parse(char *buffer, MATRIX *matrix, int row, float default_value) {
	char *delim = " ";
	char *tok_buf;
	int col_count = 0;

	tok_buf = strtok(buffer, delim);
	while (tok_buf != NULL) {
		if(*tok_buf == 'D'){
			matrix->matrix[row][col_count].value = default_value;
		}
		else
			matrix->matrix[row][col_count].value = atof(tok_buf);
		col_count++;
		tok_buf = strtok(NULL, delim);
	}
	return 0;
}
