#ifndef STATE_H
#define STATE_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

struct Movement {
    int stackFrom;
    int stackTo;
};

struct State{
	int cost; 
	char **s;
	int heuristic;
	int unicost;
	int stacks; 
	int height;
	int valid;
	std::vector<struct Movement> moves;
};
void print_state(struct State s){
	int i, j; 
	for (i = 0; i < s.stacks; i++){
		for (j = 0; j < s.height; j++){
			
				printf("%c, ", s.s[i][j]);
			
		}
		printf("\n");
	}	
	printf("cost: %i\n", s.cost );
}
void free_state(struct State s){
	int i, j; 
	for (i = 0; i < s.stacks; i++){
		free(s.s[i]);
	}
	free(s.s);
}
int get_num_stacks(const char *state){
	int stacks = 0, i; 
	for (i = 0; i < strlen(state); i++){
		if (state[i] == ';'){
			stacks++;
		}
	}
	return stacks + 1; 
}
int compare(struct State *one, struct State *two){
	int i, j; 
	for (i = 0; i < (*one).stacks; i++){
		for (j = 0; j < (*one).height; j++){
			if ((*one).s[i][j] != (*two).s[i][j])
				return 0;
		}
	}
	return 1;
}
int get_cost(int stackFrom, int stackTo) {
	return 1+abs(stackFrom-stackTo);
}
int get_heuristic(const struct State *one, const struct State *final_state ){
	int i, j, k, n;
	char box; 
	int h = 0; 
	for (i = 0; i < (*one).stacks; i++){
		for (j = 0; j < (*one).height; j++){
			if ((*one).s[i][j] >= 'A' && (*one).s[i][j] <= 'Z'){
				box = (*one).s[i][j];
				for (k = 0; k < (*final_state).stacks; k++ ){
					for (n = 0; n<(*final_state).height; n++){
						if ((*final_state).s[k][n] == box){
							h += abs(k-i) + abs (j-n);
						}
					}
				}
			}
		}
	}
	return h;
	
}

struct State parse_state(const char *state, int height, int stacks){
	int i, j, index = 0; 
	struct State st; 
	st.s = new char*[stacks];
	for (i = 0; i < stacks; i++){
		st.s[i] = new char[height];
	}
	for (i = 0; i < stacks; i++){
		for (j = 0; j < height;j++){
			st.s[i][j] = '-';
		}
	}
	for (i = 0; i < stacks; i++){
		for (j = 0; state[index] != ';'; index++){
			if(j > height){
				st.valid = 0; 
				return st;
			}
			if (state[index] >= 'A' && state[index] <= 'Z' &&state[index] != 'X'){
				st.s[i][j++] = state[index];
			}
			if (index == (strlen(state) - 1)){
				break;
			}
		}
		index++;
	}
	st.stacks = stacks; 
	st.height = height;
	st.cost = 0;
	st.unicost = 0; 
	struct Movement m; 
	m.stackFrom = 0; 
	m.stackTo = 0; 
	st.moves.push_back(m);
	return st;
}
int can_move(struct State *state,int col, int stackFrom, int stackTo, int height){
	if ((*state).s[stackFrom][0] == '-') return 0; //if stackFrom is empty
	if ((*state).s[stackTo][height-1] != '-') return 0; //if stack to is full
	if ((col == height-1)) return 1;
	if ((*state).s[stackFrom][col+1] != '-') return 0;
	if (stackFrom == stackTo) return 0;
	if ((*state).s[stackFrom][col] == '-') return 0;
	return 1; 
}
int get_index_top(char ** s, int index, int max){
	int i; 
	for (i = 0; i < max; i++ ){
		if (s[index][i] == '-')
			return i;
	}
}
struct State get_new_state(struct State *state, struct State *final_state, int stackFrom, int colFrom, int stackTo){
	struct State s; 
	int i, j; 
	s.s = new char*[(*state).stacks];
	for (i = 0; i < (*state).stacks; i++){
		s.s[i] = new char[(*state).height];
	}
	for (i = 0; i < (*state).stacks; i++){
		for (j = 0; j < (*state).height; j++){
			s.s[i][j] = (*state).s[i][j];
		}
	}
	int topTo = get_index_top(s.s,stackTo,s.height);
	char temp = s.s[stackFrom][colFrom];
	s.s[stackFrom][colFrom] = s.s[stackTo][topTo];
	s.s[stackTo][topTo] = temp;
	s.height = (*state).height;
	s.stacks = (*state).stacks;
	s.unicost = get_cost(stackFrom, stackTo) + (*state).unicost;
	s.cost = get_heuristic(&s, final_state) + s.unicost;
	struct Movement m; 
	m.stackTo = stackTo; 
	m.stackFrom = stackFrom; 
	s.moves = (*state).moves;
	s.moves.push_back(m);
	return s; 
}

#endif