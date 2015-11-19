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
	Q_learning(_bebezao, _grid, 0.2, 0.8, default_value);

	return 0;
}

void Q_learning(AGENT *agent, MATRIX *world, int alfa, int gamma, float default_value){
	int action, x, y;
	char state, new_state, best_action;
	float reward = 0;

    while(reward != 1 || reward != -1){
        state = world->matrix[agent->posx-1][agent->posy-1].state;
        x = agent->posx;
        y = agent->posy;
        reward = world->matrix[x][y].value;
        action = choose_action(agent, world, alfa, default_value);
        printf("acao: %d\n", action);

        AGENT_move(agent, world, action);
        printf("nova posicao %d %d \n", agent->posx, agent->posy);
        new_state = world->matrix[agent->posx-1][agent->posy-1].state;
        best_action = choose_best_action(agent, world, default_value);
        printf("best action %d \n", best_action);
        //TODO: arrumar estado
        agent->Q[state][action] = (1-alfa) * agent->Q[0][action] + alfa*(reward + gamma * agent->Q[new_state][best_action]);

        printf("posicão: %d, %d - valor Q: %f \n",  x, y, agent->Q[state][action]);
    }
}

int choose_best_action(AGENT *agent, MATRIX *world, float default_value){
    int action = -1000, i;
    float max = default_value -0.1;
    for(i=0; i<NOF_ACTIONS; i++){
        if(AGENT_move(agent, world, i) != -1){
            if(world->matrix[agent->posx-1][agent->posy-1].value >= max || world->matrix[agent->posx-1][agent->posy-1].value != 0.0){
                action = i;
                max = world->matrix[agent->posx-1][agent->posy-1].value;
                //printf("posicao gerada pelo best action %d %d \n", agent->posx,agent->posy);
            }
            AGENT_unmove(agent, world, i);
        }
    }
    return action;
}

int choose_action(AGENT *agent, MATRIX *world, int alfa, float default_value){
	int action = -1000, i;
	float max = default_value -0.1;
	if ( RAND < alfa){
		action = rand() % NOF_ACTIONS;
	}else{
		for(i=0; i<NOF_ACTIONS; i++){
			if(AGENT_move(agent, world, i) != -1){
                if(world->matrix[agent->posx-1][agent->posy-1].value >= max
                   && world->matrix[agent->posx-1][agent->posy-1].value != 0.0){
                    action = i;
                    max = world->matrix[agent->posx-1][agent->posy-1].value;
                }
                AGENT_unmove(agent, world, i);
			}
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


int AGENT_change_pos(AGENT *agent, MATRIX *world, int newx, int newy) {
	// checking boundaries and walls
	if(MATRIX_out_of_bounds(world, newx, newy) ||
			AGENT_is_wall( world, newx, newy)){
			return -1;
			}
	// changing agent position
	agent->posx = newx; agent->posy = newy;

	return 0;
}

int AGENT_is_wall(MATRIX *world, int newx, int newy) {
    if(newx == 0 || newy ==0 )
        return 0;
	return (world->matrix[newx-1][newy-1].state == 'X');
}

/*
 * Resets agent's attributes (position to (1,1), QTable has all of its
 * values set to zero)
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

