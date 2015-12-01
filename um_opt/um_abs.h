/* um_abs.h
 * Created by Alex Jackson
 * 	      Claudia Mihm
 * 	      11/19/15
 * Interface for the UM abstraction
 */

#ifndef UM_ABS_INCLUDED
#define UM_ABS_INCLUDED

#include "segments.h"

typedef struct UM_T *UM_T;

extern UM_T um_init(MemSegments loaded_mem);
extern void um_run(UM_T um);
extern void um_free(UM_T *um); 


#endif 
