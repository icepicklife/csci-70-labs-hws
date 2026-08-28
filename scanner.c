#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>

// initialize enums for object declaration of the states
typedef enum {
    A_state, // start state
    B_state, // gather digits from the NUM token
    E_state, // once encountered a single '=' character
} State;

// function to go through the contents of a file following the DFA
void scanOperationsFile(FILE *inputList, FILE *outputList) {

    int c;
    State currentState = A_state;

    // array storage of the string to do operations on
    char Opstring[256];
    int Op_index = 0;

    // loop until the end of the file has been reached
    while (1) {

        c = fgetc(inputList); // retrieve the character being iterated

        if (c == EOF) // once it reached the end of the file
        {

            if (currentState == B_state) // if it ends with a number, terminate the array and print the number
            {
                Opstring[Op_index] = '\0';
                fprintf(outputList, "NUM\t\t%s\n", Opstring);
            }

            else if (currentState == E_state) // following the DFA, it throws an error once nothing follows after a single '='
            {
                fprintf(outputList, "Lexical Error reading character \"=\"\n");
            }

            break;
        }

        // switch cases following the DFA states and transitions
        switch (currentState) {
            case A_state:

                if (isspace(c)) {

                    // if it is a space, tab, or new line: simply continue to the next read
                    currentState = A_state;

                } else if (isdigit(c)){

                    // start building the NUM op and move to state B
                    Opstring[Op_index++] = c;
                    currentState = B_state;

                } else if (c == '='){

                    // go to state e for checking of the complete ASSIGN token
                    currentState = E_state;

                } else if (c == '+'){
                    
                    // outputs PLUS token for +
                    fprintf(outputList, "PLUS\t+\n");
                    currentState = A_state;

                } else if ( c == '-') {

                    // outputs MINUS token for -
                    fprintf(outputList, "MINUS\t-\n");
                    currentState = A_state;

                } else {

                    // Any other character found will throw an error from A and terminate
                    fprintf(outputList, "Lexical Error reading character \"%c\"\n", c);
                    return;

                }
                break;

            case B_state:
                
                if (isdigit(c)) {

                    // build the overall number in the array buffer
                    if (Op_index < 255) {
                        Opstring[Op_index++] = c;
                    }

                } else {

                    // pushback token once the digits are complete -> complete token
                    Opstring[Op_index] = '\0';
                    fprintf(outputList, "NUM\t\t%s\n", Opstring);

                    // reset array index for next operation
                    Op_index = 0;

                    // push the terminating character of the state back to the stream for state A evaluation
                    ungetc(c, inputList);
                    currentState = A_state;

                }
                break;

            case E_state:
                
                if (c == '='){

                    // if the second character is also an '=', then complete the ASSIGN token
                    fprintf(outputList, "ASSIGN\t==\n");
                    currentState = A_state;

                } else {

                    // any other character after a single '=' is an error
                    fprintf(outputList, "Lexical Error reading character \"%c\"\n", c);

                    return; // terminate the stream according to sample I/O

                }
                break;

            default:
                break;
        }
    }
}

// helper function in following the specs where the "input" keyword 
// in the txt changes to "output" in the output txt filename
void generateOutputList(const char *inputName, char *outputName) {

    char *match = strstr(inputName, "input");
    if (match) {
        int prefix_len = match - inputName; // find the char index of the last character before "input"
        strncpy(outputName, inputName, prefix_len); // copy string up to that prefix
        outputName[prefix_len] = '\0';
        strcat(outputName, "output"); // add "output" to the prefix
        strcat(outputName, match + strlen("input")); // copy the rest after the keywords
    }

}

int main() {

    // initialize directory objects in a LINUX environment
    DIR *dir;
    struct dirent *ent;

    // Open the directory where the executable is being ran
    if ((dir = opendir(".")) != NULL) {

        // go through all the files in the directory
        while ((ent = readdir(dir)) != NULL) {

            // look for a txt that has input on their file name
            if (strstr(ent->d_name, "input") != NULL && strstr(ent->d_name, ".txt")) {

                // create the output file name
                char outputFilename[256];
                generateOutputList(ent->d_name, outputFilename);

                // initialize the read and write files
                FILE *inFile = fopen(ent->d_name, "r");
                FILE *outFile = fopen(outputFilename, "w");

                if (inFile && outFile) {

                    printf("Processing %s -> %s\n", ent->d_name, outputFilename);
                    scanOperationsFile(inFile, outFile); // Perform the operations from the input file, all print statements will go to the output file
                    fclose(inFile); // close input file
                    fclose(outFile); // close output file

                } else {
                    printf("Error opening files for %s\n", ent->d_name); // error fallback for file access
                }
            }
        }

        // close directory
        closedir(dir);

    } else {

        // error fallback for directory access
        perror("Could not open directory");
        return EXIT_FAILURE;

    }

    // terminate successfully
    return EXIT_SUCCESS;
}