#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MEMSIZE    80 // maximum size of the char array
#define LINESIZE  100 // maximum size of the input line

char memory[MEMSIZE]; // the char array containing the memory
char input[LINESIZE]; // handles input from stdin or file

int kanta = 1; // whether or not to continue

// used to return fixed size arrays
struct array {
    char arr[LINESIZE];
    int size;
};

// gets all characters which are not spaces or nulls
struct array get_part(int starting_index) {
    struct array ret;
    ret.size = 0;
    for (int i = starting_index; input[i] != ' ' && input[i] != '\0'; i += 1) {
        ret.arr[i-starting_index] = input[i];
        ret.size += 1;
    }
    for (int i = ret.size; i < LINESIZE && ret.arr[i] != '\0';) {
        ret.arr[i] = ' ';
        i += 1;
    }
    return ret;
}

// actually allocates memory
void add_memory(char to_add_char, int to_add_int, int index) {
    if (index + to_add_int > MEMSIZE) {
        return;
    }
    for (int i = index; i < index + to_add_int; i += 1) {
        memory[i] = to_add_char;
    }
}

// counts how many dots are in a free block
int free_block_count(int index) {
    int ret = 0;
    while (memory[index + ret] == '.' && index + ret < MEMSIZE) {
        ret += 1;
    }
    return ret;
}

// allocates using first fit algorithm
void alloc_first(char to_add_char, int to_add_int) {
    for (int i = 0; i < MEMSIZE; i += 1) {
        if (memory[i] == '.') {
            int count = free_block_count(i);
            if (count >= to_add_int) {
                add_memory(to_add_char, to_add_int, i);
                return;
            }
            i += count - 1;
        }
    }
    printf("error: illegal add in '%s'\n", input);
}

// allocates using best fit algorithm
void alloc_best(char to_add_char, int to_add_int) {
    int index = -1;
    int smallest = INT_MAX;
    for (int i = 0; i < MEMSIZE; i += 1) {
        if (memory[i] == '.') {
            int count = free_block_count(i);
            if (count >= to_add_int && count < smallest) {
                smallest = count;
                index = i;
            }
            i += count - 1;
        }
    }
    if (index == -1) {
        printf("error: illegal add in '%s'\n", input);
    }
    add_memory(to_add_char, to_add_int, index);
}

// allocates using worst fit algorithm
void alloc_worst(char to_add_char, int to_add_int) {
    int index = -1;
    int largest = INT_MIN;
    for (int i = 0; i < MEMSIZE; i += 1) {
        if (memory[i] == '.') {
            int count = free_block_count(i);
            if (count >= to_add_int && count > largest) {
                largest = count;
                index = i;
            }
            i += count - 1;
        }
    }
    if (index == -1) {
        printf("error: illegal add in '%s'\n", input);
    }
    add_memory(to_add_char, to_add_int, index);
}

// chooses which fit algorithm to use
void alloc() {
    if (strcspn(input, "\0") <= 6) {
        printf("error: incomplete command in '%s'\n", input);
        return;
    }
    struct array to_add = get_part(4);
    char to_add_char = input[2];
    int to_add_int = atoi(to_add.arr);
    if (4 + to_add.size + 1 > (int) strcspn(input, "\0")) {
        printf("error: illegal command '%s'\n", input);
        return;
    }
    switch(input[4 + to_add.size + 1]) {
        case 'F' :
            alloc_first(to_add_char, to_add_int);
            break;
            
        case 'B' :
            alloc_best(to_add_char, to_add_int);
            break;

        case 'W' :
            alloc_worst(to_add_char, to_add_int);
            break;

        default :
            printf("error: illegal fit algorithm '%c' in '%s'\n", input[4 + to_add.size + 1], input);
    }
}

// frees the memory that corresponds to the char
void dealloc(char to_del_char) {
    for (int i = 0; i < MEMSIZE; i += 1) {
        if (memory[i] == to_del_char) {
            memory[i] = '.';
        }
    }
}

// finds which memory to free
void free_memory() {
    if (strcspn(input, "\0") <= 2) {
        printf("error: incomplete command in '%s'\n", input);
        return;
    }
    char to_del_char = get_part(2).arr[0];
    dealloc(to_del_char);
}

// compacts the list
void compact() {
    int back = 0;
    char temp;
    for (int i = 0; i < MEMSIZE && back < MEMSIZE;) {
        while (memory[back] == '.') {
            back += 1;
        }
        if (back < MEMSIZE) {
            temp = memory[back];
            memory[back] = '!';
            memory[i] = temp;
            back += 1;
            i += 1;
        }
    }
    dealloc('!');
}

// function declaration
void file_read();

// chooses the right command
void info_dump() {
    if (input[0] == '\0') {
        printf("error: unspecified command\n");
        return;
    }
    if (input[1] != ' ' && input[1] != '\0') {
        printf("error: illegal command in '%s'\n", input);
        return;
    }
    switch(input[0]) {
        case 'A' :
            alloc();
            break;
            
        case 'F' :
            free_memory();
            break;

        case 'S' :
            printf("%s\n", memory);
            break;

        case 'R' :
            file_read();
            break;
            
        case 'C' :
            compact();
            break;

        case 'E' :
            kanta = 0;
            break;

        default :
            printf("error: illegal command '%c' in '%s'\n", input[0], input);
    }
}

// reads a file and executes whatever is in there
void file_read() {
    if (strcspn(input, "\0") <= 2) {
        printf("error: unspecified file in '%s'\n", input);
        return;
    }
    FILE *file;
    struct array filename = get_part(2);
    file = fopen(filename.arr, "r");
    if (file == NULL) {
        printf("error: illegal file '%s' in '%s'\n", filename.arr, input);
        return;
    }
    while (kanta && fgets(input, LINESIZE, file) != NULL) {
        input[strcspn(input, "\n")] = 0;
        info_dump();
    }
    fclose(file);
}

// the main function, does its thing until kanta becomes 0
int main() {
    printf("welcome to the contiguous memory allocator, space cowboy\n");
    for (int i = 0; i < MEMSIZE; i += 1) {
        memory[i] = '.';
    }
    while (kanta == 1) {
        fgets(input, LINESIZE, stdin);
        fflush(stdin);
        input[strcspn(input, "\n")] = 0;
        info_dump();
    }
    printf("see you, space cowboy\n");
    return 0;
}

