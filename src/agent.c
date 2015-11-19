#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../include/matrix.h"
#include "../include/utils.h"
#include "../include/agent.h"
#include <unistd.h>
#define FILENAME "entrada.txt"

AGENT *_bebezao;
MATRIX *_grid;
int cols = 0, rows = 0, _trainings = 100;
float _default_value = 0.0;
float _alpha = 0.7, _gamma = 0.8, _eps = 0.2;
FILE *saida;

int main(int argc, char **argv){
	int i, j;

	saida= fopen("saida.txt", "wt");

	if(argc > 1)
		UTILS_parse_args(argc, argv, &_alpha, &_gamma, &_eps, &_trainings);

	UTILS_parse_parameters("entrada.txt", &rows, &cols, &_default_value);
	if(_default_value == -1){
		return 0;
	}
	printf("rows: %d cols: %d default value: %f\n", rows, cols, _default_value);

	_grid = MATRIX_new(rows, cols);

	UTILS_parse_grid_world("entrada.txt", _grid, _default_value);
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			printf("state: %c v: %0.2f ", _grid->matrix[i][j].state,
					_grid->matrix[i][j].value);
		}
		printf("\n");
	}

	_bebezao = AGENT_new(_grid->r * _grid->c, _grid->r -1, 0);
	for(i = 0;  i <  _trainings ; i++) {
		Q_learning(_bebezao, _grid, _alpha, _gamma, _eps, _default_value);
		fprintf(saida, "Fim da rodada \n\n");
		AGENT_reset(_bebezao, 0, _grid->r -1, 0);
	}
	fclose(saida);
	return 0;
}

void Q_learning(AGENT *agent, MATRIX *world, float alfa, float gamma, float epsilon, float default_value){
	int action, best_action, col, row;
	int state, new_state, iterator = 0;
	float reward = 0;
	while(reward == default_value || reward == 0)  {

		state = (agent->row) * world->c + (agent->col);
		iterator++;
		col = agent->col;
		row  = agent->row;
		reward = world->matrix[row][col].value;
		action = choose_action(agent, world, epsilon, default_value);
		//printf("acao: %d\n", action);

		AGENT_move(agent, world, action);
		//printf("nova posicao %d %d \n", agent->row, agent->col);
		new_state = (agent->row) * world->c + (agent->col);
		best_action = choose_best_action(agent, world, default_value);
		//printf("best action %d \n", best_action);
		agent->Q[state][action] = (1-alfa) * agent->Q[new_state][action] +
			alfa*(reward + gamma * agent->Q[new_state][best_action]);

        printf("Posicao: (%d, %d) Valor: %.2f\n", row, col,agent->Q[state][action]);
	sleep(0.5);
	}
	fprintf(saida, "Número de iterações: %d\n", iterator);
}

int choose_best_action(AGENT *agent, MATRIX *world, float default_value){
	int action = -1000, i, state;
	float max = 0.0;

	for(i=0; i<NOF_ACTIONS; i++){
		if(AGENT_move(agent, world, i) != -1){
			state = (agent->row) * world->c + (agent->col);

			if( agent->Q[state][i] >= max ||
					agent->Q[state][i] != 0.0){
				action = i;
				max = agent->Q[state][action];
				//printf("posicao gerada pelo best action %d %d \n", agent->row ,agent->col);
			}
			AGENT_unmove(agent, world, i);
		}
	}
	return action;
}

int choose_action(AGENT *agent, MATRIX *world, float epsilon, float default_value){
	int action = -1000, i, state;
	float max = 0.0;
	{
		for(i=0; i<NOF_ACTIONS; i++){
			if(AGENT_move(agent, world, i) != -1) {
				state = (agent->row) * world->c + (agent->col);
				//printf("move is valid -> %d\n", i);
				if( agent->Q[state][i] >= max ||
						agent->Q[state][i] != 0.0){
					action = i;
					max = agent->Q[state][action];
				}
				AGENT_unmove(agent, world, i);
			}
		}
	}
	float randi;
	if ( (randi = RAND) < epsilon) {
		switch(action) {
			case LEFT:
			case RIGHT:
				if(RAND <= 0.5)
					action = UP;	
				else
					action = DOWN;
				break;
			case UP:
			case DOWN:
				if(RAND <= 0.5)
					action = LEFT;	
				else
					action = RIGHT;
				break;

		}
		if(AGENT_move(agent, world, action) == -1)
			return STAY;

		AGENT_unmove(agent, world, action);

	}

	return action;
}

/* agent allocator, sets x,y to (0, 0) and a zero-filled Q-table */
AGENT *AGENT_new(int nof_states, int st_row, int st_col) {
	int i;
	AGENT *new_ag = (AGENT *)malloc(sizeof(AGENT));
	if((new_ag->Q = malloc(nof_states * sizeof(float *))) == NULL)
		return NULL;
	for(i = 0; i < nof_states; i++) {
		if((new_ag->Q[i] = calloc(NOF_ACTIONS,  sizeof(float))) == NULL)
			return NULL;
	}

	new_ag->row = st_row;
	new_ag->col = st_col;
	return new_ag;
}

/* Returns -1 if action could not be performed (out of bounds), else it's
 * a successful <action> execution. Agent position is updated, as well as
 * the QTable should be
 */
int AGENT_move(AGENT *agent, MATRIX *world, int action) {
	switch(action) {
		case LEFT:
			if(AGENT_change_pos(agent, world, (agent->col - 1),
						agent->row) < 0)
				return -1; // invalid move


			break;
		case RIGHT:
			if(AGENT_change_pos(agent, world, (agent->col + 1),
						agent->row) < 0)
				return -1; // invalid move

			break;
		case UP:
			if(AGENT_change_pos(agent, world, agent->col,
						(agent->row - 1)) < 0)
				return -1; // invalid move
			break;
		case DOWN:
			if(AGENT_change_pos(agent, world, agent->col,
						(agent->row + 1)) < 0)
				return -1; // invalid move
			break;
		case STAY:
			break;

	}
	return 0;
}

int AGENT_unmove(AGENT *agent, MATRIX *world, int action) {
	switch(action) {
		case LEFT:
			if(AGENT_change_pos(agent, world, (agent->col + 1),
						agent->row) < 0)
				return -1; // invalid move

			break;
		case RIGHT:
			if(AGENT_change_pos(agent, world, (agent->col - 1),
						agent->row) < 0)
				return -1; // invalid move
			break;
		case UP:
			if(AGENT_change_pos(agent, world, agent->col,
						(agent->row + 1)) < 0)
				return -1; // invalid move

			break;
		case DOWN:
			if(AGENT_change_pos(agent, world, agent->col,
						(agent->row - 1)) < 0)
				return -1; // invalid move
			break;
		case STAY:
			break;
	}
	return 0;
}


int AGENT_change_pos(AGENT *agent, MATRIX *world, int newx, int newy) {
	// checking boundaries and walls
	//printf("%d - %d\n", newy, newx );
	//printf("bordas %d \n",MATRIX_out_of_bounds(world, newx, newy ));
	//printf("parede %d \n",AGENT_is_wall( world, newx, newy) );
	if(MATRIX_out_of_bounds(world, newx, newy) ||
			AGENT_is_wall( world, newx, newy)){
		return -1;
	}
	// changing agent position
	agent->col = newx; agent->row = newy;

	return 0;
}

int AGENT_is_wall(MATRIX *world, int newx, int newy) {
	if(newx < 0 || newy < 0 || newx >= cols|| newy >= rows)
		return 0;
	return (world->matrix[newy][newx].state == 'X');
}

/*
 * Resets agent's attributes (position to (1,1), QTable has all of its
 * values set to zero if reset_table = 1)
 */
void AGENT_reset(AGENT *agent, int reset_table, int rs_row, int rs_col) {
	int i;
	agent->row = rs_row;
	agent->col = rs_col;
	if(reset_table) {
		for(i = 0; i < agent->nof_states; i++)
			memset((void *) agent->Q[i], 0,
					NOF_ACTIONS * sizeof(float));
	}
	return;
}

/* Agent deallocator */
void AGENT_free(AGENT *agent) {
	int i;
	for(i = 0; i < agent->nof_states; i++)
		free((void *) agent->Q[i]);
	free((void *) agent->Q);
	free((void *) agent);
}

