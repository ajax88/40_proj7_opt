/* segments_test.c
 *      Created by Alex Jackson: ajacks06
 *            Claudia Mihm: cmihm01
 *            11/15/15
 *      Test file for the memory segment abstraction 
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "assert.h"
#include "segments.h"

const int MEM_CAP = 10;

void test_expand();
void use_unmapped_id();
void use_outofbounds_id();
void double_unmap();
void invalid_unmap();
void make_seg_size_zero();
void outofbounds_offset();
int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;
	MemSegments mem = mem_init();
	uint32_t ids[100];
        for (uint32_t i = 0; i < 100; i++) {
                ids[i] = new_segment(mem, i);
                for (uint32_t j = 0; j < i; j++)
                        write_word(mem, ids[i], j, j);
        }
        for (uint32_t i = 0; i < 100; i++) {
                for (uint32_t j = 0; j < i; j++)
                        fprintf(stderr, "num is %d\n", 
                                (int)get_word(mem, ids[i], j));
        }



        //test_expand();
        //use_unmapped_id();
        //use_outofbounds_id();   
        //double_unmap();
        //invalid_unmap();
        //make_seg_size_zero();
        //outofbounds_offset();
        return 0;
}

/* create a new segment of memory when there are no available unmapped 
 * segment identifiers */
void test_expand()
{
        uint32_t identifier;
        MemSegments mem = mem_init();
        for(int i = 0; i < MEM_CAP; i++){
                identifier = new_segment(mem, 1);
        }       
        identifier = new_segment(mem,1);
        fprintf(stdout, "identifier: %"PRId32"\n",identifier);
        mem_free(&mem);
}

/* try to use an unmapped segment identifier to access data (should 
 * result in a program crash of some kind) */
void use_unmapped_id()
{
        MemSegments mem = mem_init();
        uint32_t id1 = new_segment(mem,1);
        uint32_t id2 = id1 + 1;
        uint32_t word = get_word(mem,id2, 0);
        fprintf(stdout, "word: %" PRId32 "\n", word);
        mem_free(&mem);
}

/*
 *  using an invalid segment identifier (e.g one that is out of the 
 *  bounds of the array of pointers to segments) */

void use_outofbounds_id()
{
        MemSegments mem = mem_init();
        uint32_t id = MEM_CAP;
        get_word(mem, id,0);
        mem_free(&mem);
}

/* try to unmap a segment using an identifier that is already unmapped
 * or is not valid */

void double_unmap()
{
        MemSegments mem = mem_init();
        uint32_t id = new_segment(mem,1);
        uint32_t word = ~0;
        write_word(mem, id, 0, word);
        unmap_segment(mem, id);
        unmap_segment(mem, id);
        mem_free(&mem);
}               

void invalid_unmap()
{
        MemSegments mem = mem_init();
        uint32_t id = MEM_CAP;
        unmap_segment(mem, id);
        mem_free(&mem);
}

/* try to create a new memory segment with size 0 */
void make_seg_size_zero()
{
        MemSegments mem = mem_init();
        uint32_t id = new_segment(mem, 0);
        (void) id;
        mem_free(&mem);
}       
/* try to get or write a word using a correct segment identifier 
 * but an offset that is out of the bounds of the segment of memory */
void outofbounds_offset()
{
        MemSegments mem = mem_init();
        uint32_t id = new_segment(mem,2);
        uint32_t word = ~0;
        write_word(mem, id, 0, word);
        word = get_word(mem, id, 2);
        mem_free(&mem);
}       
