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
#include "hashmap.c"

struct lexeme
{
    char *lexeme;
};

typedef struct grammar_rule_node
{
    struct lexeme *lex;
    struct grammar_rule_node *next;
} grammar;

struct GRAMMAR
{
    struct grammar_rule_node **LIST;
    int size;
    hashmap *hash_nterminals, *hash_terminals;
};

struct containerNode
{
    int index;
    struct containerNode *next;
};
typedef struct containerNode containerNode;

containerNode *fillNode(int ind)
{
    containerNode *n = (containerNode *)malloc(sizeof(containerNode));
    n->index = ind;
    n->next = NULL;
    return n;
}

containerNode **workOnGram(struct GRAMMAR *g, int non_terminals)
{
    containerNode **array = (containerNode *)malloc(sizeof(containerNode) * non_terminals);
    for (int i = 0; i < non_terminals; i++)
        array[i] = NULL;
    int num = g->size;
    for (int i = 0; i < num; i++)
    {
        grammar *head = g->LIST[i];
        while (head != NULL)
        {
            if (lookup(head->lex, g->hash_nterminals) != -1)
            {
                containerNode *node = fillNode(i);
                node->next = array[lookup(head->lex, g->hash_nterminals)];
                array[lookup(head->lex, g->hash_nterminals)] = node;
            }
            head = head->next;
        }
    }
}

containerNode **workOnGramFirst(struct GRAMMAR *g, int non_terminals)
{
    containerNode **array = (containerNode *)malloc(sizeof(containerNode) * non_terminals);
    for (int i = 0; i < non_terminals; i++)
        array[i] = NULL;
    int num = g->size;
    for (int i = 0; i < num; i++)
    {
        grammar *head = g->LIST[i];
        if (lookup(head->lex, g->hash_nterminals) != -1)
        {
            containerNode *node = fillNode(i);
            node->next = array[lookup(head->lex, g->hash_nterminals)];
            array[lookup(head->lex, g->hash_nterminals)] = node;
        }
    }
}