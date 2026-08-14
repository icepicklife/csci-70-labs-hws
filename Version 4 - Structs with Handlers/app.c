#include <stdio.h>
#include "stack.h"

void application(){
    push(0, 'x');
    push(1, 'y');
    push(1, 'z');
    push(0, 'a');

    printf("Pushing to a out-of-bound arbitrary stack.");
    printf("Push B to stack number 20...");

    push(20, 'B');

    printf("Pop B to that stack number 20 assumming it successfully pushed...");

    char error_ans = pop(20);

    printf("\n");

    char ans = pop(1);
    printf("Popped from stack 0: %c\n", ans);
}

int main(){
    application();
    return 0;
}