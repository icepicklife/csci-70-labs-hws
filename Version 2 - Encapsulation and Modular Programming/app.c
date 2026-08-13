#include <stdio.h>
#include "stack.h"

void application(){
    push('x');
    push('y');
    push('z');
    char ans = pop();
    printf("Popped: %c\n", ans);
}

int main(){
    application();
    return 0;
}