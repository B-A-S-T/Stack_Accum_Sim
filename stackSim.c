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
char *sourceTokens[NUM_OF_TOKENS];

/* Prototypes */

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
    
    parse_source_code("/home/ian/Stack_Accum_Sim/testing.txt");
    printf("The first token is:%s \n", sourceTokens[1]);
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
    size_t length = 0;
    ssize_t read;
    char *token = NULL;
    char *line = NULL;
    int tokenCounter = 0;
    fp = fopen("/home/ian/Stack_Accum_Sim/testing.txt", "r");
    if(fp == NULL){
       printf("Could not open file.\n");
        exit (1);
    }
    while((read = getline(&line, &length, fp)) != -1){
        token = strtok(line, " \t");
        while(token != NULL) {
            if(strcmp(token, "\n") != 0){
            printf("BNoops:%d    %s\n", length, token);
            sourceTokens[tokenCounter] = token;
            tokenCounter += 1;
            }
            token = strtok(NULL, " \t");
        }
    }

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












