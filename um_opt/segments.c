/* segments.h
 * Created by Alex Jackson: ajacks06
 *            Claudia Mihm: cmihm01
 *            11/15/15
 * Implementation for the memory segment abstraction of a UM
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "assert.h"
#include "segments.h"
#include "seq.h"
#include "uarray.h" 

#define INITIAL_SIZE 10
#define WORDSIZE sizeof(uint32_t)

struct MemSegments {
	uint32_t seg_size;
	Seq_T segments; 
	Seq_T unmapped_ids;
};

/************************** HELPER PROTOTYPES ***************************/
static void expand_ids(MemSegments memory);
static void push_ids(MemSegments memory, uint32_t prev_size, uint32_t new_size);

/************************* INTERFACE/USER FUNCTIONS *********************/


extern MemSegments mem_init()
{	
	MemSegments main_memory = malloc(sizeof(*main_memory));
	main_memory->seg_size = INITIAL_SIZE;
	main_memory->segments = Seq_new(INITIAL_SIZE);
	main_memory->unmapped_ids = Seq_new(INITIAL_SIZE);
	push_ids(main_memory, 0, main_memory->seg_size);	
        return main_memory;
}
extern void mem_free(MemSegments *memory)
{
	Seq_free(&((*memory)->segments));
	Seq_free(&((*memory)->unmapped_ids));
	free(*memory);	
        return;
}

extern uint32_t new_segment(MemSegments memory, int size)
{
	if (Seq_length(memory->unmapped_ids) == 0)
		expand_ids(memory);
	uint32_t id = (uint32_t)(uintptr_t)Seq_remlo(memory->unmapped_ids);

	UArray_T new_seg = UArray_new(size, WORDSIZE);
	Seq_put(memory->segments, (int)id, new_seg);			
        return id;
}
extern void unmap_segment(MemSegments memory, uint32_t identifier)
{
	UArray_T curr_seg = Seq_get(memory->segments, (int)identifier);
	UArray_free(&curr_seg);
	Seq_put(memory->segments, (int)identifier, NULL);
	Seq_addlo(memory->unmapped_ids, (void *)(uintptr_t)identifier);

        return;
}

extern uint32_t get_word(MemSegments memory, uint32_t identifier, int offset)
{
	UArray_T curr_seg = Seq_get(memory->segments, (int)identifier);
	return *(uint32_t *)UArray_at(curr_seg, offset);

}
extern void write_word(MemSegments memory, uint32_t identifier, int offset,
                uint32_t word)
{
	UArray_T curr_seg = Seq_get(memory->segments, (int)identifier);
	*(uint32_t *)UArray_at(curr_seg, offset) = word;
        return;
}

extern uint32_t seg_size(MemSegments memory, uint32_t identifier)
{
	UArray_T curr_seg = Seq_get(memory->segments, (int)identifier);
	return UArray_length(curr_seg);
}

/************************** HELPER FUNCTIONS ****************************/
static void expand_ids(MemSegments memory)
{
	uint32_t prev_size = memory->seg_size;
	memory->seg_size = prev_size * 2;
	push_ids(memory, prev_size, memory->seg_size);
}
static void push_ids(MemSegments memory, uint32_t prev_size, uint32_t new_size)
{
	uint32_t id = prev_size; 
	for (; id < new_size; id++){
		Seq_addhi(memory->unmapped_ids, (void *)(uintptr_t)id);
		Seq_addhi(memory->segments, NULL); // so it has space at the
						   // proper index
	}
	return;
}

#undef INITIAL_SIZE
#undef WORDSIZE
