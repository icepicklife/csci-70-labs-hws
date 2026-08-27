#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>

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

                if (isspace(c)) {
                    currentState = A_state;
                } else if (isdigit(c)){

                    Opstring[Op_index++] = c;
                    currentState = B_state;

                } else if (c == '='){

                    currentState = E_state;

                } else if (c == '+'){
                    
                    fprintf(outputList, "PLUS\t+\n");
                    currentState = A_state;

                } else if ( c == '-') {

                    fprintf(outputList, "MINUS\t-\n");
                    currentState = A_state;

                } else {

                    fprintf(outputList, "Lexical Error reading character \"%c\"\n", c);
                    currentState = A_state;

                }
                break;

            case B_state:
                
                if (isdigit(c)) {

                    if (Op_index < 255) {
                        Opstring[Op_index++] = c;
                    }

                } else {

                    Opstring[Op_index] = '\0';
                    fprintf(outputList, "NUM\t%s\n", Opstring);

                    Op_index = 0;

                    ungetc(c, inputList);
                    currentState = A_state;

                }
                break;

            case E_state:
                
                if (c == '='){

                    fprintf(outputList, "ASSIGN\t==\n");
                    currentState = A_state;

                } else {

                    fprintf(outputList, "Lexical Error reading character \"%c\"\n", c);

                    ungetc(c, inputList);
                    currentState = A_state;

                }
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

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(".")) != NULL) {

        while ((ent = readdir(dir)) != NULL) {

            if (strstr(ent->d_name, "input") != NULL && strstr(ent->d_name, ".txt")) {

                char outputFilename[256];
                generateOutputList(ent->d_name, outputFilename);

                FILE *inFile = fopen(ent->d_name, "r");
                FILE *outFile = fopen(outputFilename, "w");

                if (inFile && outFile) {

                    printf("Processing %s -> %s\n", ent->d_name, outputFilename);
                    scanFile(inFile, outFile);
                    fclose(inFile);
                    fclose(outFile);

                } else {
                    printf("Error opening files for %s\n", ent->d_name);
                }
            }
        }

        closedir(dir);

    } else {

        perror("Could not open directory");
        return EXIT_FAILURE;
        
    }
    return EXIT_SUCCESS;
}