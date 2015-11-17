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

AGENT *AGENT_new(int nof_states) {
	int i;
	AGENT *new_ag = (AGENT *)malloc(sizeof(AGENT));
	if((new_ag->Q = malloc(nof_states * sizeof(float *))) == NULL)
		return NULL;
	for(i = 0; i < nof_states; i++) {
		if((new_ag->Q[i] = malloc(NOF_ACTIONS * sizeof(float))) == NULL)
			return NULL;
	}
	return new_ag;
}

int AGENT_move(AGENT *agent, MATRIX *world, int action) {
	switch(action) {
		case LEFT:
			if(AGENT_change_pos(agent, world, (agent->posx--),
						(agent->posy)) < 0)
				// invalid move
			break;
		case RIGHT:
			if(AGENT_change_pos(agent, world, (agent->posx++),
						(agent->posy)) < 0)
				// invalid move
			break;
		case UP:
			if(AGENT_change_pos(agent, world, (agent->posx),
						(agent->posy++)) < 0)
				// invalid move
			break;
		case DOWN:
			if(AGENT_change_pos(agent, world, (agent->posx--),
						(agent->posy--)) < 0)
				// invalid move
			break;
	}
	return 0;
}

int AGENT_change_pos(AGENT *agent, MATRIX *world, int newx, int newy) {
	return -1;
}

