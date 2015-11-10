#ifndef __parser__
#define __parser__

#define FILEIN_1 "in1.txt"
#define FILEOUT "out.txt"
#define NOF_ARGS 2

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int UTILS_parse_rows(char *filename);
int UTILS_parse_cols(char *filename);
float UTILS_parse_default_value(char *filename);
int UTILS_parse_grid_world(char *filename, MATRIX *matrix, float default_value);
int UTILS_write_matrix(char *filename, MATRIX *mat);
int UTILS_write_arr_mat(char *filename, int *matrix, int r, int c);

#endif
