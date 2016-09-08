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
mem_word* read_mem(mem_addr address);
instruction* read_inst(mem_addr address);
void write_instr(mem_addr address, instruction instr);
void write_mem(mem_addr address, mem_word value);
//mem_word load(mem_addr address);
//void push(mem_word value);
//void pop(mem_word value);
//void add();
//void mult();
//void end();

int main(int argc, char *argv[]){
    
    make_memory();
    parse_source_code("/home/ian/Stack_Accum_Sim/testing.txt");
    printf("The first token is:%08x \n", data_segment[0]);
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
    char *token = NULL;
    char *line = NULL;
    int tokenCounter = 0;
    fp = fopen("/home/ian/Documents/Stack_Accum_Sim/testing.txt", "r");

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
                    //load_text(token, text_index);
                }
/*
            printf("Token string: %s\n", token);
            source_tokens[tokenCounter] = *(mem_word*) token;
            printf("Token after: %08x\n", source_tokens[tokenCounter]);
            tokenCounter += 1;
*/
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
    printf("Address: %s, Value: %s\n", address, value);

    addr = (mem_addr) strtol(address, (char **)NULL, 16);
    val = (mem_word) strtol(value, (char **)NULL, 16);
    
    write_mem(addr, val);
}

mem_word get_op_code(char* token){
    
    



}


/* For each of the reads and writes we first check to see if the address
    is less than its address top boundary and then the bottom.*/
mem_word* read_mem(mem_addr address){
    if((address <  DATA_TOP) && (address >= DATA_START))
        return &data_segment[(address - DATA_START)];
    else{
        printf("We couldn't access that address check addresses avaliable\n");
        return NULL;
    }
}

instruction* read_inst(mem_addr address){
    if((address <  TEXT_TOP) && (address >= TEXT_START))
        return &text_segment[(address - TEXT_START)];
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












