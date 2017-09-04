#include <stdlib.h>
#include <stdio.h>
#include "main_aux.h"

int main(/* int argc, char *argv[] */){
	ProgramState *prog_state;
	if(!(prog_state = create_program_state())){
		print_generic_message(MALLOC_FAILURE);
		return 0;
	}
	do {
		if(prog_state->indicators->run_state == GAME){
			if(!(fetch_and_exe_game(prog_state))){
				print_generic_message(MALLOC_FAILURE);
				break;
			}
		} else {
			if(!fetch_and_exe_settings(prog_state)){
				print_generic_message(MALLOC_FAILURE);
				break;
			}
		}
	} while (!prog_state->indicators->quit);

	destroy_program_state(prog_state);
	return 0;
}
