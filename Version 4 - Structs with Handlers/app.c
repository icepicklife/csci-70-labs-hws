#include <stdio.h>
#include "stack.h"

void application(){
    push(0, 'x');
    push(1, 'y');
    push(1, 'z');
    push(0, 'a');
    char ans = pop(1);
    printf("Popped from stack 0: %c\n", ans);
}

int main(){
    application();
    return 0;
}