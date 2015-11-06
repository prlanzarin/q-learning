#
# Makefile
#

CC=gcc
CFLAGS =-Wall # -O3 # soh ativar no release
DEBUG="-g" # TODO tratar como opção do usuário 
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
EXE = ./bin # TODO arrumar essa porcaria

all: utils.o matrix.o

utils.o: $(INC_DIR)/utils.h $(INC_DIR)/agent.h $(SRC_DIR)/utils.c matrix.o
	mkdir -p $(BIN_DIR) && $(CC) $(DEBUG) -c -o $(BIN_DIR)/utils.o $(SRC_DIR)/utils.c $(CFLAGS) 

matrix.o: $(INC_DIR)/matrix.h $(SRC_DIR)/matrix.c
	mkdir -p $(BIN_DIR) && $(CC) $(DEBUG) -c -o $(BIN_DIR)/matrix.o $(SRC_DIR)/matrix.c $(CFLAGS) 

clean:
	rm -rf $(BIN_DIR)/*.o $(EXE) $(SRC_DIR)/*~ $(INC_DIR)/*~ *~

rebuild: clean all
	
