/*
Group 19 Compiler Construction Submission
	Rajat Payghan (2021A7PS2218P)
	Rudra Goyal (2021A7PS0708P)
	Samraddh Saxena (2021A7PS1455P)
	Aditya Patel (2021A7PS2433P)
	Bhagwati Iyer (2021A7PS0685P)
	Jai Bothra (2021A7PS0015P)
*/


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "parseTree.c"

int size = 200;
struct stackEntry
{
    char *str;
    parseTreeNode *node;
    int bool;
};
typedef struct stackEntry stackEntry;
struct stack
{
    stackEntry **stackptr;
    int top;
};
typedef struct stack stack;

stack *initStack()
{
    stack *st = (stack *)malloc(sizeof(stack));
    st->stackptr = (stackEntry **)malloc(sizeof(stackEntry *) * size);
    st->top = -1;
    return st;
}
void iffull(stack *st)
{
    // printf("ifful called\n");
    size *= 2;
    st->stackptr = (stackEntry **)realloc(st->stackptr, sizeof(stackEntry *) * size);
}
void push(stack *st, char *string, int bool, parseTreeNode *n)
{
    stackEntry *entry = (stackEntry *)malloc(sizeof(stackEntry));
    entry->bool = bool;
    entry->str = string;
    entry->node = n;
    if (st->top >= size - 1)
        iffull(st);
    st->top++;
    st->stackptr[st->top] = entry;
}
void pop(stack *st)
{
    if (st->top == -1)
        printf("Stack is Empty");
    else
        free(st->stackptr[st->top]), st->top--;
}
stackEntry *peek(stack *st)
{
    if (st->top == -1)
    {
        return NULL;
    }
    return st->stackptr[st->top];
}
int cap(stack *st)
{
    return st->top + 1;
}