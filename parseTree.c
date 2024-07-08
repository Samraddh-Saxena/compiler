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

struct parseTreeNode
{
    char *lexeme;
    int line_no;
    char *token_name;
    char *value; // used if its a number
    struct parseTreeNode *parent;
    int isLeafNode;
    char *nodeSymbol; // if not a terminal or leaf node
    struct parseTreeNode *childHead;
    struct parseTreeNode *next;
};
typedef struct parseTreeNode parseTreeNode;

parseTreeNode *initParseTreeNode(char *lexeme, int line_no, char *token_name, char *value, parseTreeNode *parent, char *nodeSym, int isLeaf)
{
    parseTreeNode *n1 = (parseTreeNode *)malloc(sizeof(parseTreeNode));
    n1->lexeme = lexeme;
    n1->isLeafNode = isLeaf;
    n1->line_no = line_no;
    n1->nodeSymbol = nodeSym;
    n1->parent = parent;
    n1->token_name = token_name;
    n1->value = value;
    n1->childHead = NULL;
    n1->next = NULL;
}
