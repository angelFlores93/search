#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "state.h"
#include "QueueList.h"


void a_star(int height, struct State initialState, struct State finalState){
	QueueList<struct State> frontier;
	QueueList<struct State> visited; 
	//printf("%i\n", finalState.valid);
	if (initialState.valid == 0 || finalState.valid == 0){
		printf("No solution found\n");
		return;
	}
	frontier.enqueue(initialState); 
	//frontier.display();
	int flag = 0; 
	int i, j, k; //k stackFrom
	int step = 1; 
	struct State aux;
	//frontier.display();
	while (!frontier.isEmpty()){
		//printf("fromtier not emty\n");
		aux = frontier.dequeue();
		if (compare(&aux, &finalState) == 1){
			flag = 1; 
			break;
		}

		visited.enqueue(aux);
		for (i = 0; i < aux.stacks; i++){
			for (j = 0; j < aux.height; j++){
				for (k = 0; k < aux.stacks; k++){
					//printf("can move %c to %i: %i\n", aux.s[i][j], k, can_move(&aux,j, i, k, aux.height));
					if (can_move(&aux,j, i, k, aux.height)){
						//printf("Moving %c to %i\n", aux.s[i][j], k );
						struct State new_state = get_new_state(&aux, &finalState, i, j, k);
						if (!visited.contains(new_state)){
							frontier.enqueue(new_state);
							//printf("added\n");
						}
					}
				}
			}
		}
		//printf("added\n");
		step++;
		//frontier.display();
		//printf("%i\n", step);
	}

	//frontier.enqueue(get_new_state(&initialState, &finalState, 1, 2, 0, 2));
	//frontier.enqueue(get_new_state(&initialState, &finalState, 1, 2, 2, 0));
	//frontier.enqueue(get_new_state(&initialState, &finalState, 0, 1, 2, 0));
	
	//frontier.display();
	//printf("finished\n");
	if (flag){
		printf("%i\n", aux.cost);
		for(i = 1; i < aux.moves.size(); i++) {
		    printf("(%i, %i)", aux.moves[i].stackFrom, aux.moves[i].stackTo);
		    if (i != aux.moves.size()-1){
		    	printf("; ");
		    }
		 } 
		
		//printf("(%i, %i)", aux.moves.get(0).stackFrom, aux.moves.get(0).stackTo);
	}else{
		printf("No solution found\n" );
	}
	
	frontier.clear();
}
int main (){
	int height; 
	char s1[1024]; 
	char s2[1024];
	scanf("%i %[^\n] %[^\n]",&height,  s1, s2);
	int stacks = get_num_stacks(s1);
	struct State initialState = parse_state(s1, height, stacks);
	struct State finalState = parse_state(s2, height, stacks);
	//printf("states parsed\n");
	//print_state(finalState);
	a_star(height, initialState, finalState);
	//printf("%i\n%s\n%s",height, initialState, finalState);
	return 0;
}