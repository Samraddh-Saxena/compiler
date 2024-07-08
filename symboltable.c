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
#include <string.h>
#include <stdlib.h>
#define TSIZE 1000
// #include "lexer.h"

bucketNode *initbucketNode(char *key, char *val)
{
    bucketNode *node = (bucketNode *)malloc(sizeof(bucketNode));
    node->key = (char *)malloc(strlen(key));
    strcpy(node->key, key);
    // node->key=key;
    node->val = val;
    node->next = NULL;
    return node;
}
int hashing_function(const char *str)
{
    int hash, i;
    for (hash = i = 0; str[i] != '\0'; i++)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return (abs(hash) % TSIZE);
}
char *lookupST(char *key, symTable *map)
{
    // returns NULL if not found else returns the token
    int index = hashing_function(key);
    bucketNode *head = map->table[index];
    while (head != NULL)
    {
        if (strcmp(head->key, key) == 0)
        {
            return head->val;
        }
        head = head->next;
    }
    return NULL;
}

void insertST(char *key, char *val, symTable *map)
{
    int index = hashing_function(key);
    bucketNode *node = initbucketNode(key, val);
    if (lookupST(key, map) != NULL)
        return;
    if (map->table[index] == NULL)
    {
        map->table[index] = node;
    }
    else
    {
        node->next = map->table[index];
        map->table[index] = node;
    }
    map->currSize++;
}

symTable *initsymbolTable()
{
    symTable *map = (symTable *)malloc(sizeof(symTable));
    map->currSize = 0;
    map->totalSize = TSIZE;
    for (int i = 0; i < TSIZE; i++)
    {
        map->table[i] = NULL;
    }
    insertST("<---", "TK_ASSIGNOP", map);
    insertST("%%", "TK_COMMENT", map);
    insertST("with", "TK_WITH", map);
    insertST("parameters", "TK_PARAMETERS", map);
    insertST("end", "TK_END", map);
    insertST("while", "TK_WHILE", map);
    insertST("union", "TK_UNION", map);
    insertST("endunion", "TK_ENDUNION", map);
    insertST("definetype", "TK_DEFINETYPE", map);
    insertST("as", "TK_AS", map);
    insertST("type", "TK_TYPE", map);
    insertST("_main", "TK_MAIN", map);
    insertST("global", "TK_GLOBAL", map);
    insertST("parameter", "TK_PARAMETER", map);
    insertST("list", "TK_LIST", map);
    insertST("[", "TK_SQL", map);
    insertST("]", "TK_SQR", map);
    insertST("input", "TK_INPUT", map);
    insertST("output", "TK_OUTPUT", map);
    insertST("int", "TK_INT", map);
    insertST("real", "TK_REAL", map);
    insertST(",", "TK_COMMA", map);
    insertST(";", "TK_SEM", map);
    insertST(":", "TK_COLON", map);
    insertST(".", "TK_DOT", map);
    insertST("endwhile", "TK_ENDWHILE", map);
    insertST("(", "TK_OP", map);
    insertST(")", "TK_CL", map);
    insertST("if", "TK_IF", map);
    insertST("then", "TK_THEN", map);
    insertST("endif", "TK_ENDIF", map);
    insertST("read", "TK_READ", map);
    insertST("write", "TK_WRITE", map);
    insertST("return", "TK_RETURN", map);
    insertST("+", "TK_PLUS", map);
    insertST("-", "TK_MINUS", map);
    insertST("*", "TK_MUL", map);
    insertST("/", "TK_DIV", map);
    insertST("call", "TK_CALL", map);
    insertST("record", "TK_RECORD", map);
    insertST("endrecord", "TK_ENDRECORD", map);
    insertST("else", "TK_ELSE", map);
    insertST("&&&", "TK_AND", map);
    insertST("@@@", "TK_OR", map);
    insertST("~", "TK_NOT", map);
    insertST("<", "TK_LT", map);
    insertST("<=", "TK_LE", map);
    insertST("==", "TK_EQ", map);
    insertST(">", "TK_GT", map);
    insertST(">=", "TK_GE", map);
    insertST("!=", "TK_NE", map);

    return map;
}
