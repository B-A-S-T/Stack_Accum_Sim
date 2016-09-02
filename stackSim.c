/*Using 32 bit addressing we will be able to access 4GB of data. 
  4294967296 decimal and 0x100000000 hex
  With addresses, words, and instructions all being 32 bits each
  we will have a total of 134217728 memory locations simulated using arrays.(index's).
  
  The text segment will start at address 0x50000000
  The data segment will start at address 0x40000000
  The stack segment will start at address 0x30000000
  The kernal segment will start at address 0x60000000

    Note not all address space is being use, but that is fine for the purpose of the assignment. Each segment is given 8388608 words.

  We can access each word in the segement when given its address by subtracting that address
    from the bottom most address and then right shifting that value 2, to simulate dividing
    by 2^2. segment[(currentAddress - botAdress) >> 2].

                    example: text_segment[(0x50000008 - 0x50000000) >> 2]
                        =   text_segement[(8) >> 2] = text_segment[2];
                        Address is converted so we can access it with our array.
*/



/* Typedefs and Defines*/
#include <stdint.h>
#include <stdio.h>

typedef int32_t mem_addr;
typedef int32_t mem_word;
typedef int32_t instruction;

#define MAX_SEG_SIZE 8388607
#define TEXT_START ((mem_word) 0x50000000)
#define DATA_START ((mem_word) 0x40000000)
#define STACK_BOTTOM  ((mem_word) 0x30000000) /*Grows Down*/
#define KERNAL_START ((mem_word) 0x60000000)


/*Global Variables:  */

instruction text_segment;
mem_word data_segment;
mem_word stack_segment;
mem_word kernal_segment;
mem_word stack_top;

/* Prototypes */

instruction* parse_source_code(char *filename);
void push(mem_word value);
void pop(mem_word value);
void add();
void mult();
void end();

int main(int argc, char *argv[]){


return 0;
}

