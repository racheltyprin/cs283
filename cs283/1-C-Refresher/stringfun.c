#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
void reverse_string(char *, int str_len);
void word_print(char *, int str_len);
int string_replace(char *, int len, char *, char *);


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    char *src = user_str;
    char *dest = buff;
    int count = 0;
    int in_whitespace = 0;

    while (*src) {
        if (count >= len) {
            return -1;
        }

        if (isspace(*src)) {
            if (!in_whitespace) {
                *dest++ = ' ';
                count++;
                in_whitespace = 1;
            }
        } else {
            *dest++ = *src;
            count++;
            in_whitespace = 0;
        }
        src++;
    }

    for (int count = 0; count < len; count++) {
        *dest++ = '.';
        count++;
    }

    return count;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int count = 0;
    int in_word = 0;

    for (int i = 0; i < str_len; i++) {
        if (!isspace(buff[i]) && buff[i] != '.') {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }

    return count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
int string_replace(char *buff, int len, char *target, char *replacement) {
    char *match = NULL;
    int target_len = 0, replacement_len = 0;
    char *temp = buff;

    while (target[target_len] != '\0') target_len++;
    while (replacement[replacement_len] != '\0') replacement_len++;

    // Find occurrence of target in buff
    while (*temp != '\0' && temp < buff + len) {
        if (strncmp(temp, target, target_len) == 0) {
            match = temp;
            break;
        }
        temp++;
    }

    if (!match) {
        printf("Target string not found.\n");
        return -1;
    }

    int extra_chars = replacement_len - target_len;
    if ((buff + len - match) < replacement_len) {
        printf("Error: Replacement is too long\n");
        return -2; 
    }

    if (extra_chars > 0) {
        char *end = buff + len - 1;
        char *src = buff + len - extra_chars - 1;
        while (src >= match + target_len) {
            *end = *src;
            end--;
            src--;
        }
    }
    for (int i = 0; i < replacement_len; i++) {
        match[i] = replacement[i];
    }

    printf("Modified String: ");
    for (char *p = buff; p < buff + len && *p != '.'; p++) {
        putchar(*p);
    }
    putchar('\n');

    return 0;
}

void word_print(char *buff, int str_len) {
    int word_num = 1;
    char *start = buff;

    printf("Printing\n----------\n");
    for (int i = 0; i < str_len; i++) {
        if (isspace(buff[i]) || i == str_len - 1) {
            if (i == str_len - 1 && !isspace(buff[i])) {
                i++;
            }
            printf("%d. ", word_num++);
            for (char *p = start; p < buff + i; p++) {
                putchar(*p);
            }
            printf(" (%ld)\n", buff + i - start);
            start = buff + i + 1;
        }
    }
}

void reverse_string(char *buff, int str_len) {
    char *start = buff;
    char *end = buff + str_len - 1;

    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }

    printf("Reversed String: ");
    for (int i = 0; i < str_len; i++) {
        putchar(buff[i]);
    }
    putchar('\n');
}


int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if argv[1] does not exist?
    //      This ensures that argc is composed of the correct number of arguments, and checks that argv is a flag.
    //      if argv does not exist, this program would not be called to begin with.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      This statement checks the number of arguments and ensures there is a string provided in the command. Otherwise, the program exits.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3:
    buff = (char *)malloc(BUFFER_SZ);
    if (buff == NULL) {
        printf("Memory allocation failed\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
        case 'r':
            reverse_string(buff, user_str_len);
            break;
        case 'w':
            word_print(buff, user_str_len);
            break;
        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            free(buff);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          This is a good practice because providing the buffer and length can prevent buffer overflows and/or incorrect memory accessing.