//
// Created by Ahmed on 18-Aug-16.
//

#ifndef INTERPRETER_TERM4_STACK_POSTFIX_TO_INFIX_H
#define INTERPRETER_TERM4_STACK_POSTFIX_TO_INFIX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TYPE char
#define MAXSIZE 100

char postFix[100] = {'\0'};

typedef struct doubleStack {
    double data[MAXSIZE];
    int top, isInitialized;
} doubleStack;

void Reset2(doubleStack *s) {
    s->top = 0;
    s->isInitialized = 1;
}

void Push2(doubleStack *s, double value) {
    if (s->top == MAXSIZE || s->isInitialized != 1) {
        printf("Stack is full or not initialized ,Plz Reset Stack\n");
        return;
    }
    s->data[s->top++] = value;
}

double Pop2(doubleStack *s) {
    int Flag = 1;
    if (s->top <= 0) {
        printf("Stack is Empty\n");
        Flag = 0;
    }

    if (Flag)
        return s->data[--s->top];
    else
        return 0;
}

double Top2(doubleStack *s) {
    if (s->top <= 0) {
        printf("Stack is Empty\n");
        return 0;
    }
    return s->data[(s->top) - 1];
}

int isEmpty2(doubleStack *s) {
    if (s->top <= 0) {
        return 1;
    }
    return 0;
}

void Print2(doubleStack *s) {
    int i = 0;
    printf("Stack is : ");
    while (i < s->top) {
        printf("%lf ", s->data[i++]);
    }
    printf("\n");
}

typedef struct Stack {
    TYPE data[MAXSIZE];
    int top, isInitialized;
} Stack;

void Reset(Stack *s) {
    s->top = 0;
    s->isInitialized = 1;
}

void Push(Stack *s, TYPE value) {
    if (s->top == MAXSIZE || s->isInitialized != 1) {
        printf("Stack is full or not initialized ,Plz Reset Stack\n");
        return;
    }
    s->data[s->top++] = value;
}

TYPE Pop(Stack *s) {
    int Flag = 1;
    if (s->top <= 0) {
        printf("Stack is Empty\n");
        Flag = 0;
    }

    if (Flag)
        return s->data[--(s->top)];
    else
        return 0;
}

TYPE Top(Stack *s) {
    if (s->top <= 0) {
        printf("Stack is Empty\n");
        return 0;
    }
    return s->data[(s->top) - 1];
}

int isEmpty(Stack *s) {
    if (s->top <= 0) {
        return 1;
    }
    return 0;
}

Stack char_stack;
doubleStack stack2;

int Priorty(char A) {
    if (A == '*' || A == '/')
        return 2;
    if (A == '+' || A == '-')
        return 1;
    else
        return 0;
}

double evaluate_MultiDigitPostFix(char *postFix) {
    Reset2(&stack2);
    char tmpArray[10];
    char tmp[100];
    strcpy(tmp, postFix);
    int i = 0, j = 0, len = strlen(tmp);
    double First = 0, Second = 0;
    for (i = 0; i < len; i++) {

        if (tmp[i] == '.' || isdigit(tmp[i])) {
            while (tmp[i] != ' ' && i < len) {
                tmpArray[j++] = tmp[i++];
            }
            tmpArray[j] = '\0', j = 0;
            Push2(&stack2, atof(tmpArray));
        }

        if (tmp[i] == '+') {
            Push2(&stack2, Pop2(&stack2) + Pop2(&stack2));
        } else if (tmp[i] == '*') {
            Push2(&stack2, Pop2(&stack2) * Pop2(&stack2));
        } else if (tmp[i] == '-') {
            First = Pop2(&stack2), Second = Pop2(&stack2);
            Push2(&stack2, Second - First);
        } else if (tmp[i] == '/') {
            First = Pop2(&stack2), Second = Pop2(&stack2);
            Push2(&stack2, Second / First);
        }
    }

    return Pop2(&stack2);
}

char *inFix_To_postFix(char inFix[]) {

    for (int i = 0; i < 100; i++) {
        postFix[i] = '\0';
    }

    Reset(&char_stack);
    unsigned int i = 0, j = 0;
    char tmp = 'A';
    int digitFlag = 1;

    for (i = 0; i < strlen(inFix); i++) {

        if (isdigit(inFix[i])) {
            if (digitFlag == 0) {
                postFix[j++] = ' ';
            }
            postFix[j++] = inFix[i];
            digitFlag = 1;
        } else if (inFix[i] == '(') {
            Push(&char_stack, inFix[i]);
            digitFlag = 0;
        } else if (inFix[i] == ')') {
            while ((tmp = Pop(&char_stack)) != '(' && !isEmpty(&char_stack)) {
                postFix[j++] = ' ';
                postFix[j++] = tmp;
            }
            digitFlag = 0;
        } else if (inFix[i] == '*' || inFix[i] == '/' || inFix[i] == '+'
                   || inFix[i] == '-') {
            while (!isEmpty(&char_stack) && Top(&char_stack) != '('
                   && (Priorty(Top(&char_stack)) >= Priorty(inFix[i]))) {
                postFix[j++] = ' ';
                postFix[j++] = Pop(&char_stack);
            }
            digitFlag = 0;
            Push(&char_stack, (int) inFix[i]);
        } else if (inFix[i] == '.') {
            postFix[j++] = inFix[i];
        }
    }
    postFix[j++] = ' ';
    while (!isEmpty(&char_stack)) {
        postFix[j++] = (char) Pop(&char_stack);
        postFix[j++] = ' ';
    }
    postFix[j] = '\0';
    postFix[j + 1] = '\0';
    return postFix;
}

#endif //INTERPRETER_TERM4_STACK_POSTFIX_TO_INFIX_H
