#ifndef __matrix__
#define __matrix__

typedef struct grid {
        char state;
        float value;
} GRID;

typedef struct grid_matrix {
        GRID **matrix;
        int r, c;
} MATRIX;

MATRIX *MATRIX_new(int rows, int cols);

void MATRIX_free(MATRIX *matrix);

int MATRIX_out_of_bounds(MATRIX *matrix, int x, int y);

#endif
