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
#include <stdlib.h>
#include <string.h>

typedef int32_t mem_addr;
typedef int32_t mem_word;
typedef int32_t instruction;

#define MAX_SEG_SIZE 33554428
#define TEXT_START ((mem_word) 0x50000000)
#define DATA_START ((mem_word) 0x40000000)
#define STACK_BOTTOM  ((mem_word) 0x30000000) /*Grows Down*/
#define KERNAL_START ((mem_word) 0x60000000)

#define NUM_OF_TOKENS 200
/* We subtract 1 mem_word(32 bits) from the address to get the top */

#define DATA_TOP (TEXT_START -1)
#define TEXT_TOP (KERNAL_TOP -1)
#define KERNAL_TOP (((mem_word) (0x70000000)) -1)

/*Global Variables:  */

instruction* text_segment;
mem_word *data_segment;
mem_word *stack_segment;
mem_word *kernal_segment;
mem_addr stack_top;
mem_addr data_limit;
mem_word *source_tokens[NUM_OF_TOKENS];

/* Prototypes */

void make_memory();
void parse_source_code(char *filename);
void load_data(char* token);
void load_text(char* token, int *index);
instruction get_opCode(char* instr);
mem_word read_mem(mem_addr address);
instruction read_inst(mem_addr address);
void write_instr(mem_addr address, instruction instr);
void write_mem(mem_addr address, mem_word value);
void add(mem_word* accum, mem_addr operand);
void mult(mem_word* accum, mem_addr operand);

int main(int argc, char *argv[]){
    
    make_memory();
    parse_source_code(argv[1]);
    mem_word accumulator;
    instruction instr;
    int usermode = 1;
    mem_word to_load = 0;
    mem_addr to_store = 0;
    mem_addr to_add = 0;
    mem_addr to_mult = 0; 
    mem_addr pc = TEXT_START;

    while(usermode){
        instr = read_inst(pc);
        pc += 1;
        switch(instr){
            case 0:
                to_load = (mem_word) read_inst(pc); 
                accumulator = read_mem(to_load);
                pc += 1;
                break;
            case 1:
                to_store = (mem_addr) read_inst(pc);
                write_mem(to_store, accumulator);
                pc += 1;
                break;
            case 2:
                to_add = (mem_addr) read_inst(pc);
                add(&accumulator, to_add);
                pc +=1;
                break;
            case 3:
                to_mult = (mem_addr) read_inst(pc);
                mult(&accumulator, to_mult);
                pc +=1;
                break;
            case 4:
                usermode = 0;
                printf("Value in Accumulator:  %d\n\n", accumulator);
                break;
        }
    }
    free(data_segment);
    free(stack_segment);
    free(kernal_segment);
    free(text_segment);
    return 0;
}

/* Each segment will have 33,554,431 bytes, divide this by

    4 and we get 8,388,607 indexes in each array*/

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

void parse_source_code(char *filename){
    FILE *fp;
    short data_load = 0;
    size_t length = 0;
    ssize_t read;
    int *text_index = 0;
    char *token = NULL;
    char *line = NULL;
    fp = fopen(filename, "r");

    if(fp == NULL){
       printf("Could not open file.\n");
        exit (1);
    }
    while((read = getline(&line, &length, fp)) != -1){
            if(strcmp(line, "\n") != 0){
            line[strcspn(line, "\n")] = 0;
                if(strcmp(line, ".data") == 0){
                     data_load = 1;
                     continue;
                }
                if(strcmp(line, ".text") == 0){
                     data_load = 0;
                     continue;
}               
                if(data_load == 1){
                    load_data(line);
                }
                else{
                    load_text(line, &text_index);
                }
            }
    }

}

load_data(char* token){
    char *address = NULL;
    char *value = NULL;
    mem_addr addr = 0; 
    mem_word val = 0;
    
    address = strtok(token, " \t");
    value = strtok(NULL, " \t");

    addr = (mem_addr) strtol(address, (char **)NULL, 16);
    val = (mem_word) strtol(value, (char **)NULL, 16);
    
    write_mem(addr, val);
}

void load_text(char* token, int *index){
    char *instr = strtok(token, " \t");
    instruction op_code = 0;

    while(instr != NULL){
        op_code = get_opCode(instr);
        mem_addr address = TEXT_START + *index;
        write_instr(address, op_code);
        *index += 1;
        instr = strtok(NULL, " \t");
    }
    
}

instruction get_opCode(char *instr){
    if(strcmp(instr, "LOAD") == 0) return 0;
    else if (strcmp(instr, "STO") == 0) return 1;
    else if (strcmp(instr, "ADD") == 0) return 2;
    else if (strcmp(instr, "MULT") == 0) return 3;
    else if (strcmp(instr, "END") == 0) return 4;
    else return (instruction) strtol(instr, (char **)NULL, 16);
}

/* For each of the reads and writes we first check to see if the address
    is less than its address top boundary and then the bottom.*/
mem_word read_mem(mem_addr address){
    if((address <  DATA_TOP) && (address >= DATA_START))
        return data_segment[(address - DATA_START)];
    else{
        printf("We couldn't access that address check addresses avaliable\n");
        return NULL;
    }
}

instruction read_inst(mem_addr address){
    if((address <  TEXT_TOP) && (address >= TEXT_START)){
        return text_segment[(address - TEXT_START)];
}
    else{
        printf("We couldn't access that address check addresses avaliable\n");
        return NULL;
    }
}

void write_instr(mem_addr address, instruction instr){

    if((address <  TEXT_TOP) && (address >= TEXT_START))
        text_segment[(address - TEXT_START)] = instr;
    else{
        printf("We couldn't access that address check address avaliable\n");
        return NULL;
    }
}


void write_mem(mem_addr address, mem_word value){
    if((address < DATA_TOP) && (address >= DATA_START))
        data_segment[(address - DATA_START)] = value;
    else{
        printf("We couldn't access that address check addresses avaliable\n");
        return NULL;
    }
}

void add(mem_word* accum, mem_addr operand){
    mem_word quotient = read_mem(operand);
    *accum = quotient + *accum;
}

void mult(mem_word* accum, mem_addr operand){

    mem_word product = read_mem(operand);
    *accum = product * (*accum);
}















