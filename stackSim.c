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

#define DATA_TOP (TEXT_START -4)
#define TEXT_TOP (KERNAL_TOP -4)
#define KERNAL_TOP ((mem_word) (0x70000000 -4))

/*Global Variables:  */

instruction text_segment;
mem_word *data_segment;
mem_word *stack_segment;
mem_word *kernal_segment;
mem_addr stack_top;
mem_addr data_limit;

/* Prototypes */

instruction* parse_source_code(char *filename);
mem_word load(mem_addr address);
void push(mem_word value);
void pop(mem_word value);
void add();
void mult();
void end();

int main(int argc, char *argv[]){


return 0;
}


void make_memory(){

    data_segment = malloc(MAX_SEG_SIZE);
    if(data_segment == NULL)
        exit(1);

    stack_segment = malloc(MAX_SEG_SIZE);
    if(stack_segment == NULL)
        exit(1);
    kernal_segment = malloc(MAX_SEG_SIZE);
    if(kernal_segment == NULL)
        exit(1);
    text_segment = malloc(MAX_SEG_SIZE);
    if(text_segment == NULL) 
        exit(1);

}

instruction* parse_source_code(char *filename);{

    FILE *fp;
    char line[255];

    fp = fopen(filename, "r");
    
    if(fp == NULL){
       printf("Could not open file.\n");
        return 0; 
    }
    fscanf(fp, "%s", line);

}


mem_word read_mem(mem_addr address){
    if((address <  DATA_TOP) && (address >= DATA_START))
        return data_segment[(address - DATA_START) >> 2];
    else{
        printf("We couldn't access that address check addresses
                avaliable\n");
        return NULL;
    }
}

mem_word read_inst(mem_addr address){
    if((address <  TEXT_TOP) && (address >= TEXT_START))
        return text_segment[(address - TEXT_START) >> 2];
    else{
        printf("We couldn't access that address check addresses
                avaliable\n");
        return NULL;
    }
}

void write_instr(mem_addr address, instruction instr){

    if((address <  TEXT_TOP) && (address >= TEXT_START))
        text_segment[(address - TEXT_START) >> 2] = instr;
    else{
        printf("We couldn't access that address check addresses
                avaliable\n");
        return NULL;
    }
}


void write_mem(mem_addr address, mem_word value){
    if((address < DATA_TOP) && (address >= DATA_START))
        data_segment[(address - DATA_START) >> 2] = value;
    else{
        printf("We couldn't access that address check addresses
                avaliable\n");
        return NULL;
    }
}












