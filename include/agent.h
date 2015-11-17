#ifndef __agent__
#define __agent__

#define DEFAULT_VALUE  1
#define NOF_ACTIONS 4

typedef enum action {
        LEFT,
        RIGHT,
        UP,
        DOWN
} ACTIONS;

typedef struct agent_st {
        float **Q;
        int posx, posy;
} AGENT;

AGENT *AGENT_new(int nof_states); 

void AGENT_free(AGENT *agent);

int AGENT_move(AGENT *agent, MATRIX *world, int action);

int AGENT_change_pos(AGENT *agent, MATRIX *world, int newx, int newy);

#endif
