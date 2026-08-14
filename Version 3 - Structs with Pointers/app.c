#include <stdio.h>
#include "stack.h"

void application(){
    struct stack s0, s1;
    initstack(&s0);
    initstack(&s1);

    push(&s0, 'x');
    push(&s1, 'y');
    push(&s1, 'z');
    push(&s0, 'a');

    s1.top = 0;
    s1.Store[0] = 'G';
    s1.top = 1;
    
    char ans = pop(&s1);
    printf("Popped from s1: %c \n", ans);
}

int main(){
    application();
    return 0;
}