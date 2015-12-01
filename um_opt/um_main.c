/* um_main.c 
 * Created by Alex Jackson
 * 	      Claudia Mihm
 * 	11/19/15
 * Main file for the UM - uses the UM abstraction and a built-in program loader
 * to run .um binaries
 */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "segments.h"
#include "um_abs.h"
#include "bitpack.h" 
#include "inttypes.h" 

#define WORDSIZE sizeof(uint32_t)

/*************************** PROGRAM LOADER PROTOTYPES ***********************/
static MemSegments load_binary(char *filename);

int main(int argc, char *argv[])
{
	if (argc != 2){
		fprintf(stderr, "Usage: program %s takes 1 argument, e.g\n"
		"%s 'filename'\n", argv[0], argv[0]);
		exit(EXIT_FAILURE);	
	}

	MemSegments initial_mem = load_binary(argv[1]);
	UM_T um = um_init(initial_mem);
	um_run(um);
	um_free(&um);
	return 0;
}

/************************** PROGRAM LOADER FUNCTIONS *************************/
static MemSegments load_binary(char *filename)
{
	int num_words, segzero_id, i, j, temp_char;
	int most_lsb = (WORDSIZE * 8) - 8;
	uint32_t temp_word;
	struct stat file_data;

	if(stat(filename, &file_data) < 0) {
		fprintf(stderr, "File %s is invalid\n", filename);
		exit(EXIT_FAILURE);
	}

	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "File %s is invalid\n", filename);
		exit(EXIT_FAILURE);
	}

	num_words = file_data.st_size / WORDSIZE ;		
	MemSegments memory = mem_init();
	segzero_id = new_segment(memory, num_words); /* on first call this 
						will always be 0 */ 
	
	for (i = 0; i < num_words; i++) {
		temp_word = 0;
		/* pack 4 bytes (chars) into one 32-bit word */
		for (j = 0; j < (int)WORDSIZE; j++) {
			temp_char = getc(fp);
			temp_word = Bitpack_newu(temp_word, 8,
			(most_lsb - j * 8), temp_char);
		}
		write_word(memory, segzero_id, i, temp_word);
	}

	fclose(fp);
	return memory;
}




