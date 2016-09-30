# README
## Created by Alex Jackson: ajacks06
##           Claudia Mihm: cmihm01
##           11/24/15

### Purpose
This program emulates an performance optimized RISC-style and turing-complete universal machine that uses 8 registers and 14 basic instructions. 


*************************** PROGRAM ARCHITECTURE *******************************


* um_writer.c & write_helper.h *
- These files are an abstraction for the testing of the system. Becuase these
modules are irrelavent to the exported program, it is not worth going into 
detail about their functions. The assignment spec mentioned that we should be 
including all .c and .h files used for the project, so this is simply a 
notification of what the purpose of these modules are.

* segments_test.c *
- Simply a testing file for the memory segments abstraction. 

* segments.h (memory segments abstraction) *
- Contains a 'MemSegments' data structure which is given to the user as an 
incomplete struct. This is an abstraction for the memory of a universal machine
The secrets of the struct are hidden in the implementation:
it contains two Hanson sequences, one that holds the memory segments and one 
that holds all the unused segment ids. The struct also has a uint32_t variable
that hold the total number of mapped segments. This module allows a user to 
create a MemSegments object. With this object, the user can map and unmap memory
segments - with each memory segment, they may write and get words at any valid
index of a given segment. There are hidden functions within the implementation
that are used to help keep track of the used and unused segment ids.

* um_abs.h (um abstraction) *
- Contains a UM_T data structure which is used with the operations of this um
abstraction. It is given to the user as an incomplete struct - its secrets held 
within the implemenation are a MemSegments object, an array of 8 registers that 
each hold the size of a 'word', and a program counter which points to the next
instruction to be executed by the um at a given point in the process. The user 
must initialize a UM_T with a loaded MemSegment. This entails putting the 
instructions from a .um binary into segment0 of a MemSegment object. The user 
may then either free this um, or run the instructions from the .um binary with 
um_run. There are many secrets held within this module's implementation. There 
is a hidden helper function that decodes the information of an instruction and 
puts it into a hidden struct, 'wordData'. There is another helper function which
runs the appropriate operation with the given registers. There is a final helper
function which handles the control flow of the um, calling the decoding and 
operation calling functions while keeping track of the program_counter. Note 
that all the operations are also held within this module, as they need to know 
the secret of the 'wordData' data structure. 

* um_main.h (um running abstraction w/ program loader) *
- Contains the abstraction for the program loader - this is a module holding 
the main function which is responsible for reading in a .um binary and loading
its instructions into the zeroith segment of a MemSegments object. This module
then initializes a um with this MemSegments object and runs the um accordingly. 

****************************** RUNNING TIMES ***********************************
Our um runs midmark.um in 14.3 seconds. This binary has a file size of 
120440 bytes, which corresponds to 30110 4 byte instructions. With some simple 
alegbra, we can correlate this to a runtime of 23746 seconds for a process with
500,000 instructions. 

*********************************** UMTESTS ************************************
halt.um - tests halt by calling halt, and if the um does not stop its flow 
accordingly, it will print out an error message.

goto.um - test the prog_load jumping operation by calling prog_load to jump to 
a successful print out message. The control flow will continue to print out an
error message if this jump doesn't work. The load_val and output functions are 
also tested with this .um binary. 

add2.um - tests the output/input functions by calling input. Then it tests the 
add function by adding 2 to the inputted value and printing it out.

add2_mult2.um - test the multiply function by first doing the same operations as
the above add test, then multiplying that value by 2. 

divide2.um - divides an inputted value by 2 to test divide.

cj_zero.um - tests the conditional move function by calling it with rc == 0, so 
the reassignment should not happen. This allows for a jump to occur to a
successful print out statement.

cj_nonzero.um - tests the conditional move where rc != 0 so the reassignment 
does occur, which allows for a jump to occur to a successful print out statement

read_print.um - tests the output/input functions directly by calling input, then
outputting that value.

map_unmap.um - tests the map and unmapping segments by calling both functions 
and checking to see for a crash.

load_store.um - tests the seg_load and seg_store functions by storing a value 
into a nonzero segment (which requires using the map_seg function), loading the 
same value, then outputting it.

prog_load.um - tests the prog_loading function by first making a new segment. 
Instructions are then made by a helper function that uses stores the value of a 
reduced instruction (b/c the max load_val value is 25 bits), then unreducing it 
into the proper given instruction. Essentially, it stores the instructions 
necessary to print 'Hi' into the new segment. This segment is then loaded with 
prog_load and if the 'Hi' is printed, then the test was successful.

segment_check.um - tests map_segments by doing a stress test of mapping several 
hundred segments and unmapping every other segment. 

large_mapping.um - tests the load_prog function by doing a 
large jump over several failed print out statements




