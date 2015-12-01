/* um_abs.c
 * Created by Alex Jackson
 * 	      Claudia Mihm 
 * 	11/19/15
 * Implementation for the UM abstraction
 */


#include <stdio.h>
#include <stdlib.h>
#include "um_abs.h"
#include "segments.h"
#include "bitpack.h" 
#include "inttypes.h" 
#include "assert.h" 

#define NUM_REGS 8

struct UM_T {
        MemSegments memory;
        uint32_t *regs;
        uint32_t prog_counter;
};

typedef struct {
        uint32_t rega, regb, regc, opcode;
        int val;
} wordData;

enum Registers{ r0 = 0, r1, r2, r3, r4, r5, r6, r7 };
enum Opcodes{ CMOV = 0, SEGLD, SEGS, ADD, MULT, DIV, BITAND, HALT, SEGM, SEGU,
              COUT, CIN, PROGLD, VALLD };

/*********************** HELPER FUNCTION PROTOTYPES ***************************/
extern wordData decode_instruction(UM_T um); 
extern int call_op(UM_T um, wordData curr_word);
extern int execute_intruction(UM_T um);


/*********************** UM INSTRUCTION PROTOTYPES ****************************/
extern void cond_mov(UM_T um, wordData curr_word);  
extern void seg_load(UM_T um, wordData curr_word);  
extern void seg_store(UM_T um, wordData curr_word);  
extern void add(UM_T um, wordData curr_word);  
extern void multiply(UM_T um, wordData curr_word);  
extern void divide(UM_T um, wordData curr_word);  
extern void bit_and(UM_T um, wordData curr_word);  
extern int halt(); // simply returns 1 to stop further processes  
extern void seg_map(UM_T um, wordData curr_word);  
extern void seg_unmap(UM_T um, wordData curr_word);  
extern void output(UM_T um, wordData curr_word);  
extern void input(UM_T um, wordData curr_word);  
extern void load_prog(UM_T um, wordData curr_word);  
extern void load_val(UM_T um, wordData curr_word);  


/*********************** USER/INTERFACE FUNCTIONS *****************************/


extern UM_T um_init(MemSegments loaded_mem)
{
	UM_T um = malloc(sizeof(*um));
	assert(um != NULL);
	um->regs = calloc(NUM_REGS, sizeof(uint32_t));
	assert(um->regs != NULL);
	um->memory = loaded_mem;
	um->prog_counter = 0;
	return um; 
}
extern void um_run(UM_T um)
{
	int is_halt = 0;
	while(!is_halt) {
		is_halt = execute_intruction(um);
		um->prog_counter += 1;
	}
}
extern void um_free(UM_T *um)
{
	unmap_segment((*um)->memory, 0);
	free((*um)->regs);
	mem_free(&((*um)->memory));
	free(*um);
	return;
}
       
/*********************** HELPER FUNCTIONS *************************************/


extern wordData decode_instruction(UM_T um) 
{
	wordData data;
	uint32_t word = get_word(um->memory, 0, um->prog_counter);
	/* initialize-different operations may not use all variables of data */
	data.opcode = Bitpack_getu(word, 4, 28);
	
	data.val = 0;
	data.rega = 0;
	data.regb = 0;
	data.regc = 0;
	
	if (data.opcode == VALLD) {
		data.rega = Bitpack_getu(word, 3, 25);
		data.val = Bitpack_gets(word, 25, 0);
	}
	else {
		data.regc = Bitpack_getu(word, 3, 0);
		data.regb = Bitpack_getu(word, 3, 3);
		data.rega = Bitpack_getu(word, 3, 6);
	}

	return data;
}

extern int call_op(UM_T um, wordData curr_word)
{
	if (curr_word.opcode == CMOV)
		cond_mov(um, curr_word);
	else if (curr_word.opcode == SEGLD)
		seg_load(um, curr_word);
	else if (curr_word.opcode == SEGS)
		seg_store(um, curr_word);
	else if (curr_word.opcode == ADD)
		add(um, curr_word);
	else if (curr_word.opcode == MULT)
		multiply(um, curr_word);
	else if (curr_word.opcode == DIV)
		divide(um, curr_word);
	else if (curr_word.opcode == BITAND)
		bit_and(um, curr_word);
	else if (curr_word.opcode == HALT)
		return halt(); /* return 1 */
	else if (curr_word.opcode == SEGM)
		seg_map(um, curr_word);
	else if (curr_word.opcode == SEGU)
		seg_unmap(um, curr_word);
	else if (curr_word.opcode == COUT)
		output(um, curr_word);
	else if (curr_word.opcode == CIN)
		input(um, curr_word);
	else if (curr_word.opcode == PROGLD)
		load_prog(um, curr_word);
	else if (curr_word.opcode == VALLD)
		load_val(um, curr_word);
	else 
		return 1;
	return 0;
}

extern int execute_intruction(UM_T um)
{
	int is_halt = 0;
	wordData curr_word = decode_instruction(um);
	is_halt = call_op(um, curr_word);
	return is_halt; 
}


/*********************** UM INSTRUCTIONS **************************************/


extern void cond_mov(UM_T um, wordData curr_word)  
{
	if (um->regs[curr_word.regc] != 0)
		um->regs[curr_word.rega] = um->regs[curr_word.regb];
	return;
}

extern void seg_load(UM_T um, wordData curr_word)
{
	um->regs[curr_word.rega] = get_word(um->memory,
		um->regs[curr_word.regb], um->regs[curr_word.regc]);
	return;
}
extern void seg_store(UM_T um, wordData curr_word)
{
	write_word(um->memory, um->regs[curr_word.rega],
		um->regs[curr_word.regb], um->regs[curr_word.regc]);
	return;
}
extern void add(UM_T um, wordData curr_word)  
{
	um->regs[curr_word.rega] = um->regs[curr_word.regb] +
			um->regs[curr_word.regc];
	return;
}
extern void multiply(UM_T um, wordData curr_word)  
{
	um->regs[curr_word.rega] = um->regs[curr_word.regb] *
			um->regs[curr_word.regc];
	return;
}
extern void divide(UM_T um, wordData curr_word)  
{
	um->regs[curr_word.rega] = um->regs[curr_word.regb] /
			um->regs[curr_word.regc];
	return;
}
extern void bit_and(UM_T um, wordData curr_word)  
{

	um->regs[curr_word.rega] = ~(um->regs[curr_word.regb] &
		um->regs[curr_word.regc]);
	return;
}
extern int halt() 
{
	return 1;
}
extern void seg_map(UM_T um, wordData curr_word)  
{
	um->regs[curr_word.regb] = 
			new_segment(um->memory, um->regs[curr_word.regc]);
	return;
}
extern void seg_unmap(UM_T um, wordData curr_word)  
{
	unmap_segment(um->memory, um->regs[curr_word.regc]);
	return;
}
extern void output(UM_T um, wordData curr_word)  
{
	uint32_t c = um->regs[curr_word.regc];
	assert(c <= 255);

	putchar(c);
	return;
}
extern void input(UM_T um, wordData curr_word)  
{
	char c = getchar();
	if (c == EOF) /* if end of input, load regc with all 1's */
		um->regs[curr_word.regc] = ~(uint32_t)0;
	else 
		um->regs[curr_word.regc] = c;
	return;
}
extern void load_prog(UM_T um, wordData curr_word)  
{

	um->prog_counter = um->regs[curr_word.regc] - 1;

	if (um->regs[curr_word.regb] == 0)
		return;
	else {
		uint32_t program_size, i, seg0_id;
		program_size = seg_size(um->memory, um->regs[curr_word.regb]);
		unmap_segment(um->memory, 0);
		seg0_id = new_segment(um->memory, program_size); /* will be zero
		as unmapped ids get mapped in LIFO order */

		for (i = 0; i < program_size; i++) {
			write_word(um->memory, seg0_id, i, 
				get_word(um->memory, um->regs[curr_word.regb],
									    i));
		}
	}
	return;
}
extern void load_val(UM_T um, wordData curr_word)  
{
	um->regs[curr_word.rega] = curr_word.val;
	return;
}

#undef NUM_REGS
