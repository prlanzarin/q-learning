#ifndef __agent__
#define __agent__

#include <time.h>
#define DEFAULT_VALUE  1
#define NOF_ACTIONS 4
#define RAND ((float)(rand())/(float)(RAND_MAX) )
typedef enum action {
        LEFT,
        RIGHT,
        UP,
        DOWN
} ACTIONS;

typedef struct agent_st {
        float **Q;
        int nof_states;
        int col, row;
} AGENT;

AGENT *AGENT_new(int nof_states);

void AGENT_free(AGENT *agent);

int AGENT_move(AGENT *agent, MATRIX *world, int action);

int AGENT_unmove(AGENT *agent, MATRIX *world, int action);

int choose_action(AGENT *agent, MATRIX *world, float alfa, float default_value);

int AGENT_change_pos(AGENT *agent, MATRIX *world, int newx, int newy);

int AGENT_is_wall(MATRIX *world, int newx, int newy);

int choose_best_action(AGENT *agent, MATRIX *world, float default_value);

void Q_learning(AGENT *agent, MATRIX *world, float alfa, float gamma, float epsilon, float default_value);

void AGENT_reset(AGENT *agent);
#endif
