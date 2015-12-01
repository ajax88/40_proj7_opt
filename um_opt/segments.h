/* segments.h
 * Created by Alex Jackson: ajacks06
 *            Claudia Mihm: cmihm01
 *            11/15/15
 * Interface for the memory segment abstraction of a UM
 */

#ifndef SEGMENTS_INCLUDED
#define SEGMENTS_INCLUDED

#include <stdint.h>

typedef struct MemSegments *MemSegments;

extern MemSegments mem_init();
extern void mem_free(MemSegments *memory);

extern uint32_t new_segment(MemSegments memory, int size);
extern void unmap_segment(MemSegments memory, uint32_t identifier);


extern uint32_t get_word(MemSegments memory, uint32_t identifier, int offset);
extern void write_word(MemSegments memory, uint32_t identifier, int offset, 
                uint32_t word);
extern uint32_t seg_size(MemSegments memory, uint32_t identifier);

#endif
