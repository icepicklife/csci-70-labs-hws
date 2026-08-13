#include <stdio.h>
#define MAX 100

char Store[MAX];
int top = 0;

void push(char x){
    if (top < MAX )
        Store[top++] = x ;
    else
        printf("Stack full \n") ;
}

char pop(){
    if(top > 0)
        return Store[--top];
    else{
        printf("Stack empty \n") ;
        return 0; /* Make sure something is returned regardless */
    }
}

void application(){
    push('a');
    push('b');
    push('c');
    push('d');
    push('e');
    pop();
    pop();
    pop();
    pop();
    push('f');
    push('g');
    printf("Popped : %s \n", Store);
}

int main(){
    application();
    return 0;
}