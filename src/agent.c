#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../include/matrix.h"
#include "../include/utils.h"
#include "../include/agent.h"

#define FILENAME "entrada.txt"

AGENT *_bebezao;
MATRIX *_grid;
int cols = 0, rows = 0;
float default_value = 0.0;

int main(){

	int i,j;
	
	UTILS_parse_parameters(FILENAME, &rows, &cols, &default_value);
	if(default_value == -1){
		return 0;
	}
	printf("rows: %d cols: %d default value: %f\n", rows, cols, default_value);

	_grid = MATRIX_new(rows, cols);

	UTILS_parse_grid_world(FILENAME, _grid, default_value);
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			printf("state: %c v: %0.2f ", _grid->matrix[i][j].state,
					_grid->matrix[i][j].value);
		}
		printf("\n");
	}

	_bebezao = AGENT_new(_grid->r * _grid->c);
	AGENT_reset(_bebezao);

	return 0;
}
//TODO: colocar while(goal = 1, ou -1)
void Q_learning(AGENT *agent, MATRIX *world, int alfa, int gamma){
	int action;
	char state, new_state;
	float reward = 0, maxQ = 0;

	state = world->matrix[agent->posx][agent->posy].state;
	// recompensa estado atual
	reward = world->matrix[agent->posx][agent->posy].value;
	action = choose_action(agent, world, alfa);
	// depois de escolher a ação, as posições apontam para novo estado
	new_state = world->matrix[agent->posx][agent->posy].state;

	// maxQ = ... melhor recompensa das ações disponíveis no new_state

	//nao sei se o estado vai ser um char ou outra coisa
	agent->Q[state][action] = (1-alfa) * agent->Q[state][action] + alfa*(reward + gamma * maxQ );


}
int choose_action(AGENT *agent, MATRIX *world, int alfa){
	int action, i;
	float max = 0.0;
	if ( RAND < alfa){
		action = rand() % NOF_ACTIONS;
	}else{
		for(i=0; i<NOF_ACTIONS; i++){
			AGENT_move(agent, world, i);
			// TODO: we need to check if movement is invalid
			// (move returns -1)
			if(world->matrix[agent->posx][agent->posy].value > max){
				action = i;
				max = world->matrix[agent->posx][agent->posy].value;
			}
			AGENT_unmove(agent, world, i);
		}
	}
	return action;
}

/* agent allocator, sets x,y to (1, 1) and a zero-filled Q-table */
AGENT *AGENT_new(int nof_states) {
	int i;
	AGENT *new_ag = (AGENT *)malloc(sizeof(AGENT));
	if((new_ag->Q = malloc(nof_states * sizeof(float *))) == NULL)
		return NULL;
	for(i = 0; i < nof_states; i++) {
		if((new_ag->Q[i] = calloc(NOF_ACTIONS,  sizeof(float))) == NULL)
			return NULL;
	}

	new_ag->posx = new_ag->posy = 1;
	new_ag->nof_states = nof_states;
	return new_ag;
}

/* Returns -1 if action could not be performed (out of bounds), else it's
 * a successful <action> execution. Agent position is updated, as well as
 * the QTable should be
 */
int AGENT_move(AGENT *agent, MATRIX *world, int action) {
	switch(action) {
		case LEFT:
			if(AGENT_change_pos(agent, world, (agent->posx - 1),
						agent->posy) < 0)
				return -1; // invalid move
			
				break;
		case RIGHT:
			if(AGENT_change_pos(agent, world, (agent->posx + 1),
						agent->posy) < 0)
				return -1; // invalid move

				break;
		case UP:
			if(AGENT_change_pos(agent, world, agent->posx,
						(agent->posy + 1)) < 0)
				return -1; // invalid move

				break;
		case DOWN:
			if(AGENT_change_pos(agent, world, agent->posx,
						(agent->posy - 1)) < 0)
				return -1; // invalid move

				break;
	}
	return 0;
}

/*
 * Resets the agent move that was made by <action>. E.G.: if we want to go back
 * on a LEFT action, we should call AGENT_unmove(agent, world, LEFT)
 */
int AGENT_unmove(AGENT *agent, MATRIX *world, int action) {
	switch(action) {
		case LEFT:
			if(AGENT_change_pos(agent, world, (agent->posx + 1),
						agent->posy) < 0)
				return -1; // invalid move
			
				break;
		case RIGHT:
			if(AGENT_change_pos(agent, world, (agent->posx - 1),
						agent->posy) < 0)
				return -1; // invalid move

				break;
		case UP:
			if(AGENT_change_pos(agent, world, agent->posx,
						(agent->posy - 1)) < 0)
				return -1; // invalid move

				break;
		case DOWN:
			if(AGENT_change_pos(agent, world, agent->posx,
						(agent->posy + 1)) < 0)
				return -1; // invalid move

				break;
	}
	return 0;
}

/*
 * Returns -1 if next position (newx, newy) is invalid and does not alter the
 * agent's position.
 * If the movement is valid, update agent's position and returns 0
 */
int AGENT_change_pos(AGENT *agent, MATRIX *world, int newx, int newy) {
	// checking boundaries and walls
	if(MATRIX_out_of_bounds(world, newx, newy) ||
			AGENT_is_wall( world, newx, newy));
			return -1;

	// changing agent position
	agent->posx = newx; agent->posy = newy;
	
	return 0;
}

/* 
 * Checks if position (newx, newy) is a wall ('X' char). Returns 1 if so, else 0
 */
int AGENT_is_wall(MATRIX *world, int newx, int newy) {
	return (world->matrix[newx][newy].state == 'X');
}

/*
 * Resets agent's attributes (position to (1,1), QTable has all of its
 * values set to zero
 */
void AGENT_reset(AGENT *agent) {
	int i;
	agent->posx = agent->posy = 1;
	for(i = 0; i < agent->nof_states; i++)
			memset((void *) agent->Q[i], 0, 
					NOF_ACTIONS * sizeof(float));

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
