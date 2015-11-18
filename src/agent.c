#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
//TODO: colocar while(goal = 1, ou -1)
void Q_learning(AGENT *agent, MATRIX *world, int alfa, int gamma){
    int action;
    char state, new_state;
    float reward = 0, maxQ = 0;
    agent->posx = 1;
    agent->posy = 1;

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
            if(world->matrix[agent->posx][agent->posy].value > max){
                    action = i;
                    max = world->matrix[agent->posx][agent->posy].value;
            }
        }
    }
    return action;
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
    //verificar parede?
	return -1;
}

