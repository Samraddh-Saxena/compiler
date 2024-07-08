/*
Group 19 Compiler Construction Submission
	Rajat Payghan (2021A7PS2218P)
	Rudra Goyal (2021A7PS0708P)
	Samraddh Saxena (2021A7PS1455P)
	Aditya Patel (2021A7PS2433P)
	Bhagwati Iyer (2021A7PS0685P)
	Jai Bothra (2021A7PS0015P)
*/



#ifndef LEXERH
#define LEXERH

#include <stdio.h>
#include <stdlib.h>
// SYMBOLTABLE.C Structs and Functions

#define TSIZE 1000

struct bucketNode
{
    char *key;
    char *val;
    struct bucketNode *next;
};
struct symbolTable
{
    int currSize;
    int totalSize;
    struct bucketNode *table[TSIZE];
};
typedef struct bucketNode bucketNode;
typedef struct symbolTable symTable;

// Important Functions
bucketNode *initbucketNode(char *key, char *val);
int hashing_function(const char *str);
char *lookupST(char *key, symTable *map);
void insertST(char *key, char *val, symTable *map);
symTable *initsymbolTable();

// LEXER1.C Structs and Functions

#define L_size 10000

typedef struct lexeme
{
    char *lexe;
    char *token;
    int line_no;
} lexeme;
typedef struct lex_header
{
    int size;
    lexeme *arr[L_size];
} lex_header;

// Important Functions
int getStream(FILE *fp);
char read(int *end, FILE *fp);
void removeComments(FILE *fp);

// Error and Lexeme Handlers
char *ret_lexeme_error(int beg, int end);
char *ret_lexeme(int beg, int end);
lexeme *new_lex(char *c, char *t, int lineno);
lex_header *create_Larray();
void add_lex(lex_header *head, lexeme *l);

// Utilities
void concatenate_string(char *s, int j, char *s1);
int isAlpha(char c);
// void print(FILE* fp);
void print(FILE *fp, symTable *map, lex_header *lex_list);
// void removeComments(FILE *fp);

#endif