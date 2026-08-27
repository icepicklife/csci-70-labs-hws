#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>

typedef enum {
    A_state,
    B_state,
    E_state,
} State;

void scanOperationsFile(FILE *inputList, FILE *outputList) {

    int c;
    State currentState = A_state;

    char Opstring[256];
    int Op_index = 0;

    while (1) {

        c = fgetc(inputList);

        if (c == EOF)
        {

            if (currentState == B_state)
            {
                Opstring[Op_index] = '\0';
                fprintf(outputList, "NUM\t%s\n", Opstring);
            }

            else if (currentState == E_state)
            {
                fprintf(outputList, "Lexical Error reading character \"=\"\n");
            }

            break;
        }

        switch (currentState) {
            case A_state:
                /* code */
                break;

            case B_state:
                /* code */
                break;

            case E_state:
                /* code */
                break;

            default:
                break;
        }
    }
}

void generateOutputList(const char *inputName, char *outputName) {

    char *match = strstr(inputName, "input");
    if (match) {
        int prefix_len = match - inputName;
        strncpy(outputName, inputName, prefix_len);
        outputName[prefix_len] = "\0";
        strcat(outputName, "output");
        strcat(outputName, match + strlen("input"));
    }

}

int main() {

    return EXIT_SUCCESS;
}