/*
  Group 1
  Created September 2016 by Ian Thomas with the help of Lanxin Ma.

  Using 32 bit addressing we will be able to access
  4294967296 decimal and 0x100000000 hex different addresses.
  With addresses, words, and instructions all being 32 bits each.

  The text segment will start at address 0x50000000
  The data segment will start at address 0x40000000
  The stack segment will start at address 0x30000000
  The kernal segment will start at address 0x60000000
*/

#include <stdint.h> // int32_t
#include <stdio.h> // pritntf, getline, fopen
#include <stdlib.h> // strtol
#include <string.h> // strtok, strcmp, strcspn

/* Typedefs and Defines*/
typedef int32_t mem_addr;
typedef int32_t mem_word;
typedef int32_t instruction;

#define MAX_SEG_SIZE 8888888
#define NUM_STRINGS 10
#define STRING_SIZE 100
#define TEXT_START ((mem_word) 0x50000000)
#define DATA_START ((mem_word) 0x40000000)
#define STACK_BOTTOM  ((mem_addr) 0x70000000)
#define KERNAL_START ((mem_word) 0x60000000)
#define DATA_TOP (TEXT_START -1)
#define TEXT_TOP (KERNAL_TOP -1)
#define KERNAL_TOP (((mem_word) (0x70000000)) -1)

/*Global Variables:  */
instruction* text_segment;
mem_word *data_segment;
mem_word *stack_segment;
mem_word *kernal_segment;
mem_addr stack_top = STACK_BOTTOM;
mem_addr data_limit;
int last_string = 0;
char strings[NUM_STRINGS][STRING_SIZE];

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
void push(mem_addr address);
void pop(mem_addr address);
void add();
void mult();

/*Main entry into simulator, one argument should be passed, the filename.*/
int main(int argc, char *argv[]){

    make_memory();
    parse_source_code(argv[1]);
    instruction instr;
    int usermode = 1;
    mem_word to_push = 0;
    mem_addr to_pop = 0;
    mem_addr pc = TEXT_START;
    mem_addr to_print;

    while(usermode){
        instr = read_inst(pc);
        pc += 1;
        switch(instr){
            case 0:
                to_push = (mem_word) read_inst(pc);
                push(to_push);
                pc += 1;
                break;
            case 1:
                to_pop = (mem_addr) read_inst(pc);
                pop(to_pop);
                pc += 1;
                break;
            case 2:
                add();
                break;
            case 3:
                mult();
                break;
            case 4:
                usermode = 0;
                printf("%d\n", stack_segment[(stack_top - STACK_BOTTOM)]);
                break;
            case 5:
                to_print = (mem_addr) read_inst(pc);
                if(to_print >= last_string || to_print < 0){
                    printf("Failed to print that string: Wrong Index");
                }
                printf("%s", strings[to_print]);
                pc += 1;
                break;

        }


    }
    free(data_segment);
    free(stack_segment);
    free(kernal_segment);
    free(text_segment);
    return 0;
}

/*Description: Creates memory for the memory segments.
  Params: NONE
  Returns: void
*/
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

/*Description: Loads the source code elements into simulated memory.
  Params: *filename -name of source code file
  Returns: void
*/
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
            //In data section of parse
                if(strcmp(line, ".data") == 0){
                     data_load = 1;
                     continue;
                }
            //Exit data section of parse
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

/*Description: Loads data elements from source into simulated data segment.
  Params: *token - essentially the line that was read
  Returns: void
*/
void load_data(char* token){
    char *address = NULL;
    char *value = NULL;
    mem_addr addr = 0;
    mem_word val = 0;

    address = strtok(token, " \t");
    //Checking for a data of type string, if so add to strings
    if(strcmp(address, ".asciiz") == 0){
       address = strtok(NULL, "\"") ;
        strcpy(strings[last_string], address);
        last_string += 1;
    }
    else{
      value = strtok(NULL, " \t");
      addr = (mem_addr) strtol(address, (char **)NULL, 16);
      val = (mem_word) strtol(value, (char **)NULL, 16);
      write_mem(addr, val);
    }
}

/*Description: Loads source code instructions into simulated text segment.
  Params: *index - program counter incremented for next instruction entry
  Returns: void
*/
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

/*Description: Gets the op code for an instruction for easier switch statement.
  Params: *instr - instruction that was parsed from source file
  Returns: instruction - opcode number or address for an instruction
*/
instruction get_opCode(char *instr){
    if(strcmp(instr, "PUSH") == 0) return 0;
    else if (strcmp(instr, "POP") == 0) return 1;
    else if (strcmp(instr, "ADD") == 0) return 2;
    else if (strcmp(instr, "MULT") == 0) return 3;
    else if (strcmp(instr, "END") == 0) return 4;
    else if (strcmp(instr, "PRINT") == 0) return 5;
    else return (instruction) strtol(instr, (char **)NULL, 16);
}

/* For each of the reads and writes we first check to see if the address
    is less than its address top boundary and then the bottom.*/
/*Description: Read memory from the data_segment.
      Params: address - address to read from
      Returns: mem_word - returns the contents
*/
mem_word read_mem(mem_addr address){
    if((address <  DATA_TOP) && (address >= DATA_START))
        return data_segment[(address - DATA_START)];
    else{
        printf("We couldn't access that address check addresses avaliable\n");
        return NULL;
    }
}

/*Description: Read memory from the text_segment.
      Params: address - address to read from
      Returns: insrtuction - returns the contents
*/
instruction read_inst(mem_addr address){
    if((address <  TEXT_TOP) && (address >= TEXT_START)){
        return text_segment[(address - TEXT_START)];
}
    else{
        printf("We couldn't access that address check addresses avaliable\n");
        return NULL;
    }
}

/*Description: Writes memory to the text_segment.
      Params: address - address to read from, instr- instruction to write
      Returns: void
*/
void write_instr(mem_addr address, instruction instr){

    if((address <  TEXT_TOP) && (address >= TEXT_START))
        text_segment[(address - TEXT_START)] = instr;
    else{
        printf("We couldn't access that address check address avaliable\n");
        return NULL;
    }
}

/*Description: Writes memory to the data_segment.
      Params: address - address to read from, instr- data to write
      Returns: void
*/
void write_mem(mem_addr address, mem_word value){
    if((address < DATA_TOP) && (address >= DATA_START))
        data_segment[(address - DATA_START)] = value;
    else{
        printf("We couldn't access that address check addresses avaliable\n");
        return NULL;
    }
}

/*Description: Push a value on the stack at a memory address.
      Params: address - address of value to push
      Returns: void
*/
void push(mem_addr address){
        mem_word value = read_mem(address);
        stack_top += 1;
        stack_segment[(stack_top - STACK_BOTTOM)] = value;
}

/*Description: Pop the top value of the stack into an address.
      Params: address - address of value to pop into
      Returns: void
*/
void pop(mem_addr address){
    if(address == -1){
        stack_top -= 1;
        return NULL;
    }
    if(stack_top == STACK_BOTTOM){
        printf("Trying to POP empty stack:EXITING");
        exit(1);
    }
    mem_word value = stack_segment[(stack_top - STACK_BOTTOM)];
    stack_top -= 1;
    write_mem(address, value);
}

/*Description: Adds top two values on the stack, pops them off, then pushes the
                result back on.
      Params: NONE
      Returns: void
*/
void add(){
    mem_word added_value = stack_segment[(stack_top - STACK_BOTTOM)]
            + stack_segment[(stack_top - STACK_BOTTOM - 1)];
    pop(-1);
    stack_segment[stack_top - STACK_BOTTOM] = added_value;

}

/*Description: Multiplies top two values on the stack, pops them off, then pushes the
                result back on.
      Params: NONE
      Returns: void
*/
void mult(){
    mem_word added_value = stack_segment[(stack_top - STACK_BOTTOM)]
            * stack_segment[(stack_top - STACK_BOTTOM - 1)];
    pop(-1);
    stack_segment[stack_top - STACK_BOTTOM] = added_value;

}
