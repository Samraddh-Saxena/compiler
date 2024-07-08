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
#include "stack.c"
#include "hashmap.c"
#include "lexer.h"
// #include "parser.h"

int tokens = 58;
int max_gram_len = 8;
int non_tokens = 53;
// int line_no=8;

struct Node
{
    char value[100];
    struct Node *next;
};

struct Node *create_NODE(char *c)
{
    struct Node *N = (struct Node *)malloc(sizeof(struct Node));
    N->next = NULL;
    strcpy(N->value, c);
    return N;
};

struct List
{
    int Tot_Size;
    struct Node **LIST;
};

struct List *create_List()
{
    struct List *l = (struct List *)malloc(sizeof(struct List));
    l->LIST = (struct Node **)malloc(sizeof(struct Node *) * 1000);
    for (int i = 0; i < 1000; i++)
        l->LIST[i] = NULL;
    return l;
}
void Insert(char *s, struct Node **N, int i)
{
    i--;
    struct Node *temp = create_NODE(s);
    temp->next = N[i];
    N[i] = temp;
}

typedef struct first_follow_node
{
    char *token;
    struct first_follow_node *next;
} first_follow_node;

typedef struct first_node
{
    char *token;
    struct first_node *next;
    int rule;
} first_node;

void populate_node(first_follow_node *n1, char *value)
{
    n1->token = (char *)malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(n1->token, value);
    n1->next = NULL;
}
void populate_node_first(first_node *n1, char *value, int k)
{
    n1->token = (char *)malloc(sizeof(char) * (strlen(value) + 1));
    strcpy(n1->token, value);
    n1->rule = k;
    n1->next = NULL;
}

void firstAdder(first_follow_node **firstArr, char *val, int k)
{
    first_follow_node *node1 = (first_follow_node *)malloc(sizeof(first_follow_node));
    populate_node(node1, val);
    node1->next = firstArr[k];
    firstArr[k] = node1;
}

void firstFollow(first_node **firstArr, char *val, int k, int l)
{
    first_node *node1 = (first_node *)malloc(sizeof(first_node));
    populate_node_first(node1, val, l);
    node1->next = firstArr[k];
    firstArr[k] = node1;
    node1->rule;
}

void createParseTable(first_node **firstArr, hashmap *h2, first_follow_node **followArr, struct List *l, char *parseT[non_tokens][tokens][max_gram_len])
{
    for (int i = 0; i < non_tokens; i++)
    {
        first_node *fnode = firstArr[i];
        int isNull = 0;
        // printf("here %d\n",i);
        while (fnode != NULL)
        {
            int terminal = lookup(fnode->token, h2);
            if (terminal == -1)
            {
                fnode = fnode->next;
                continue;
            }
            int rule = fnode->rule;
            struct Node *temp = l->LIST[rule];
            int k = 0;
            // printf("here2\n");
            while ((temp != NULL) && (temp->next != NULL))
            {
                // printf("here3   ");
                if (strcmp(temp->value, "eps") == 0)
                {
                    isNull = 1;
                    break;
                }
                parseT[i][terminal][k] = (char *)malloc(strlen(temp->value) + 1);
                strcpy(parseT[i][terminal][k], temp->value);
                // if(i==43 && terminal==41){
                //     printf(" %s   rule %d  here\n",temp->value,rule);
                // }
                // printf("%s  ",temp->value);
                k++;
                temp = temp->next;
            }

            fnode = fnode->next;
        }
        // if(1){
        first_follow_node *u = followArr[i];
        while (u != NULL)
        {
            int ind = lookup(u->token, h2);
            if ((ind != -1) && (isNull == 1))
                parseT[i][ind][0] = NULL;
            else if ((parseT[i][ind][0] != NULL) && (strcmp(parseT[i][ind][0], "ERROR") == 0))
            {
                // printf("%s  here", parseT[i][ind][0]);
                // parseT[i][ind][0]=(char*)malloc(5);
                strcpy(parseT[i][ind][0], "syn");
            }

            u = u->next;
        }
        // }
    }
}

void printParseTree(parseTreeNode *root, char *outfile)
{
    FILE *outline = fopen(outfile, "w");
    printParseTreeRecursive(root, outline);
}

// void printParseTreeRecursive(parseTreeNode*root, FILE *outline){
//     // add file handle later
//     parseTreeNode*p=root->childHead;
//     while(p!=NULL && p->next!=NULL){
//         printParseTreeRecursive(p,outline);
//         p=p->next;
//     }
//     if(root->isLeafNode==1 && (root->token_name=="TK_NUM" || root->token_name=="TK_RNUM"))
//     fprintf(outline,"%-25s\t%-*d\t%-15s\t%-10s\t%-25s\t%-6s\t%s\n",root->lexeme,5,root->line_no,root->token_name,root->value,root->parent->nodeSymbol,"yes",root->nodeSymbol);
//     else if(root->isLeafNode==1)
//     fprintf(outline,"%-25s\t%-*d\t%-15s\t%-10s\t%-25s\t%-6s\t%s\n",root->lexeme,5,root->line_no,root->token_name,NULL,root->parent->nodeSymbol,"yes",root->nodeSymbol);
//     else
// 	fprintf(outline,"%-25s\t%-*d\t%-15s\t%-10s\t%-25s\t%-6s\t%s\n",root->lexeme,5,root->line_no,root->token_name,root->value,root->parent->nodeSymbol,"no",root->nodeSymbol);
//     if(p!=NULL) printParseTreeRecursive(p,outline);
// }

void printParseTreeRecursive(parseTreeNode *root, FILE *outline)
{
    // add file handle later
    parseTreeNode *p = root->childHead;
    if (p != NULL)
        printParseTreeRecursive(p, outline);
    if (root->isLeafNode == 1 && (root->token_name == "TK_NUM" || root->token_name == "TK_RNUM"))
        fprintf(outline, "%-25s\t%-*d\t%-15s\t%-10s\t%-25s\t%-6s\t%s\n", root->lexeme, 5, root->line_no, root->token_name, root->value, root->parent->nodeSymbol, "yes", root->nodeSymbol);
    else if (root->isLeafNode == 1)
        fprintf(outline, "%-25s\t%-*d\t%-15s\t%-10s\t%-25s\t%-6s\t%s\n", root->lexeme, 5, root->line_no, root->token_name, NULL, root->parent->nodeSymbol, "yes", root->nodeSymbol);
    else
        fprintf(outline, "%-25s\t%-*d\t%-15s\t%-10s\t%-25s\t%-6s\t%s\n", root->lexeme, 5, root->line_no, root->token_name, root->value, root->parent->nodeSymbol, "no", root->nodeSymbol);
    if (p != NULL)
    {
        p = p->next;
    }

    while (p != NULL)
    {
        printParseTreeRecursive(p, outline);
        p = p->next;
    }
}

void parser(stack *st, lex_header *inp1, char *parseT[][tokens][max_gram_len], hashmap *h1, hashmap *h2)
{
    int inpptr = 0;
    while ((inpptr < inp1->size) && cap(st) > 0)
    {
        stackEntry *entry = peek(st);
        char *input = inp1->arr[inpptr]->token;
        if (entry->bool == 0)
        {
            if (strcmp(entry->str, input) == 0)
            {
                // printf("%s ",input);
                inpptr++;
                pop(st);
            }
            else
            {
                printf("Parsing Error at line no  ---   %d    at lexeme  %s\n", inp1->arr[inpptr]->line_no, inp1->arr[inpptr]->lexe);
                pop(st);
                // call error routine
            }
        }
        else
        {
            int a = lookup(entry->str, h1);
            int b = lookup(input, h2);
            int k = 0;
            parseTreeNode *parent = entry->node;
            // int aalo;
            // if(strcmp(entry->str,"<primitiveDatatype>")==0) {
            //     aalo=5;
            // }
            // printf("%s \n",entry->str); //use this for debugging
            pop(st);
            // printf("a=%d  b=%d\n",a,b);
            while ((k < max_gram_len) && (parseT[a][b][k] != NULL) && (strcmp(parseT[a][b][k], "ERROR") != 0) && (strcmp(parseT[a][b][k], "syn") != 0) && (b != -1))
            {
                if (lookup(parseT[a][b][k], h2) == -1)
                {
                    parseTreeNode *n1 = initParseTreeNode("----", inp1->arr[inpptr]->line_no, NULL, NULL, parent, parseT[a][b][k], 0);
                    push(st, parseT[a][b][k], 1, n1);
                    n1->next = parent->childHead;
                    parent->childHead = n1;
                }

                else
                {
                    parseTreeNode *n2 = initParseTreeNode(inp1->arr[inpptr]->lexe, inp1->arr[inpptr]->line_no, inp1->arr[inpptr]->token, inp1->arr[inpptr]->lexe, parent, NULL, 1);
                    push(st, parseT[a][b][k], 0, n2);
                    n2->next = parent->childHead;
                    parent->childHead = n2;
                }
                // printf("%s \t",parseT[a][b][k]);
                k++;
            }
            // printf("\n");
            if (b != -1 && parseT[a][b][k] == NULL)
                k++;

            if (k == 0)
            {
                if ((b != -1) && strcmp(parseT[a][b][k], "syn") == 0)
                {
                    printf("Parsing Error at line no  ---   %d    at lexeme   %s\n", inp1->arr[inpptr]->line_no, inp1->arr[inpptr]->lexe);
                    pop(st);
                }
                else
                {
                    // printf("ERROR\n");
                    printf("Parsing Error at line no  ---   %d   at lexeme  %s\n", inp1->arr[inpptr]->line_no, inp1->arr[inpptr]->lexe);
                    inpptr++;
                }

                // error routine
            }
        }
    }
}

struct List *GRAMMAR()
{
    // <program> ===> <otherFunctions> <mainFunction>
    struct List *l = create_List();

    Insert("<program>", l->LIST, 1);
    Insert("<otherFunctions>", l->LIST, 1);
    Insert("<mainFunction>", l->LIST, 1);

    // Inserting <mainFunction> rule
    Insert("<mainFunction>", l->LIST, 2);
    Insert("TK_MAIN", l->LIST, 2);
    Insert("<stmts>", l->LIST, 2);
    Insert("TK_END", l->LIST, 2);

    // Inserting <otherFunctions> rules
    Insert("<otherFunctions>", l->LIST, 3);
    Insert("<function>", l->LIST, 3);

    Insert("<otherFunctions>", l->LIST, 4);
    Insert("eps", l->LIST, 4);

    // Inserting <function> rule
    Insert("<function>", l->LIST, 5);
    Insert("TK_FUNID", l->LIST, 5);
    Insert("<input_par>", l->LIST, 5);
    Insert("<output_par>", l->LIST, 5);
    Insert("TK_SEM", l->LIST, 5);
    Insert("<stmts>", l->LIST, 5);
    Insert("TK_END", l->LIST, 5);

    // Inserting <input_par> and <output_par> rules
    Insert("<input_par>", l->LIST, 6);
    Insert("TK_INPUT", l->LIST, 6);
    Insert("TK_PARAMETER", l->LIST, 6);
    Insert("TK_LIST", l->LIST, 6);
    Insert("TK_SQL", l->LIST, 6);
    Insert("<parameter_list>", l->LIST, 6);
    Insert("TK_SQR", l->LIST, 6);

    Insert("<output_par>", l->LIST, 7);
    Insert("TK_OUTPUT", l->LIST, 7);
    Insert("TK_PARAMETER", l->LIST, 7);
    Insert("TK_LIST", l->LIST, 7);
    Insert("TK_SQL", l->LIST, 7);
    Insert("<parameter_list>", l->LIST, 7);
    Insert("TK_SQR", l->LIST, 7);

    Insert("<output_par>", l->LIST, 8);
    Insert("eps", l->LIST, 8);

    // Inserting <parameter_list> rule
    Insert("<parameter_list>", l->LIST, 9);
    Insert("<dataType>", l->LIST, 9);
    Insert("TK_ID", l->LIST, 9);
    Insert("<remaining_list>", l->LIST, 9);

    // Inserting <dataType> rules
    Insert("<dataType>", l->LIST, 10);
    Insert("<primitiveDatatype>", l->LIST, 10);

    Insert("<dataType>", l->LIST, 11);
    Insert("<constructedDatatype>", l->LIST, 11);

    // Inserting <primitiveDatatype> rules
    Insert("<primitiveDatatype>", l->LIST, 12);
    Insert("TK_INT", l->LIST, 12);

    Insert("<primitiveDatatype>", l->LIST, 13);
    Insert("TK_REAL", l->LIST, 13);

    // Inserting <constructedDatatype> rules

    Insert("<constructedDatatype>", l->LIST, 14);
    Insert("TK_RECORD", l->LIST, 14);
    Insert("TK_RUID", l->LIST, 14);

    Insert("<constructedDatatype>", l->LIST, 15);
    Insert("TK_UNION", l->LIST, 15);
    Insert("TK_RUID", l->LIST, 15);

    Insert("<constructedDatatype>", l->LIST, 16);
    Insert("TK_RUID", l->LIST, 16);

    // Inserting <remaining_list> rules
    Insert("<remaining_list>", l->LIST, 17);
    Insert("TK_COMMA", l->LIST, 17);
    Insert("<parameter_list>", l->LIST, 17);

    Insert("<remaining_list>", l->LIST, 18);
    Insert("eps", l->LIST, 18);

    // Inserting <stmts> rule
    Insert("<stmts>", l->LIST, 19);
    Insert("<typeDefinitions>", l->LIST, 19);
    Insert("<declarations>", l->LIST, 19);
    Insert("<otherStmts>", l->LIST, 19);
    Insert("<returnStmt>", l->LIST, 19);

    // Inserting <typeDefinitions> rule
    Insert("<typeDefinitions>", l->LIST, 20);
    Insert("<actualOrRedefined>", l->LIST, 20);
    Insert("<typeDefinitions>", l->LIST, 20);

    Insert("<typeDefinitions>", l->LIST, 21);
    Insert("eps", l->LIST, 21);

    // Inserting <actualOrRedefined> rules
    Insert("<actualOrRedefined>", l->LIST, 22);
    Insert("<typeDefinition>", l->LIST, 22);

    Insert("<actualOrRedefined>", l->LIST, 23);
    Insert("<definetypestmt>", l->LIST, 23);

    // Inserting <typeDefinition> rules
    Insert("<typeDefinition>", l->LIST, 24);
    Insert("TK_RECORD", l->LIST, 24);
    Insert("TK_RUID", l->LIST, 24);
    Insert("<fieldDefinitions>", l->LIST, 24);
    Insert("TK_ENDRECORD", l->LIST, 24);

    Insert("<typeDefinition>", l->LIST, 25);
    Insert("TK_UNION", l->LIST, 25);
    Insert("TK_RUID", l->LIST, 25);
    Insert("<fieldDefinitions>", l->LIST, 25);
    Insert("TK_ENDUNION", l->LIST, 25);

    // Inserting <fieldDefinitions> rules
    Insert("<fieldDefinitions>", l->LIST, 26);
    Insert("<fieldDefinition>", l->LIST, 26);
    Insert("<fieldDefinition>", l->LIST, 26);
    Insert("<moreFields>", l->LIST, 26);

    // Inserting <fieldDefinition> rules
    Insert("<fieldDefinition>", l->LIST, 27);
    Insert("TK_TYPE", l->LIST, 27);
    Insert("<fieldtype>", l->LIST, 27);
    Insert("TK_COLON", l->LIST, 27);
    Insert("TK_FIELDID", l->LIST, 27);
    Insert("TK_SEM", l->LIST, 27);

    Insert("<fieldDefinition>", l->LIST, 28);
    Insert("<typeDefinition>", l->LIST, 28);
    Insert("<fieldDefinition>", l->LIST, 28);

    // Inserting <fieldtype> rules
    Insert("<fieldtype>", l->LIST, 29);
    Insert("<primitiveDatatype>", l->LIST, 29);

    Insert("<fieldtype>", l->LIST, 30);
    Insert("<constructedDatatype>", l->LIST, 30);

    // Inserting <moreFields> rules
    Insert("<moreFields>", l->LIST, 31);
    Insert("<fieldDefinition>", l->LIST, 31);
    Insert("<moreFields>", l->LIST, 31);

    Insert("<moreFields>", l->LIST, 32);
    Insert("eps", l->LIST, 32);

    // Inserting <declarations> rule
    Insert("<declarations>", l->LIST, 33);
    Insert("<declaration>", l->LIST, 33);
    Insert("<declarations>", l->LIST, 33);

    Insert("<declarations>", l->LIST, 34);
    Insert("eps", l->LIST, 34);

    // Inserting <declaration> rule
    Insert("<declaration>", l->LIST, 35);
    Insert("TK_TYPE", l->LIST, 35);
    Insert("<dataType>", l->LIST, 35);
    Insert("TK_COLON", l->LIST, 35);
    Insert("TK_ID", l->LIST, 35);
    Insert("<global_or_not>", l->LIST, 35);
    Insert("TK_SEM", l->LIST, 35);

    // Inserting <global_or_not> rules
    Insert("<global_or_not>", l->LIST, 36);
    Insert("TK_COLON", l->LIST, 36);
    Insert("TK_GLOBAL", l->LIST, 36);

    Insert("<global_or_not>", l->LIST, 37);
    Insert("eps", l->LIST, 37);

    // Inserting <otherStmts> rule
    Insert("<otherStmts>", l->LIST, 38);
    Insert("<stmt>", l->LIST, 38);
    Insert("<otherStmts>", l->LIST, 38);

    Insert("<otherStmts>", l->LIST, 39);
    Insert("eps", l->LIST, 39);

    // Inserting <stmt> rules
    Insert("<stmt>", l->LIST, 40);
    Insert("<assignmentStmt>", l->LIST, 40);

    Insert("<stmt>", l->LIST, 41);
    Insert("<iterativeStmt>", l->LIST, 41);

    Insert("<stmt>", l->LIST, 42);
    Insert("<conditionalStmt>", l->LIST, 42);

    Insert("<stmt>", l->LIST, 43);
    Insert("<ioStmt>", l->LIST, 43);

    Insert("<stmt>", l->LIST, 44);
    Insert("<funCallStmt>", l->LIST, 44);

    // Inserting <assignmentStmt> rule
    Insert("<assignmentStmt>", l->LIST, 45);
    Insert("<singleOrRecId>", l->LIST, 45);
    Insert("TK_ASSIGNOP", l->LIST, 45);
    Insert("<arithmeticExpression>", l->LIST, 45);
    Insert("TK_SEM", l->LIST, 45);

    // Inserting <singleOrRecId> rule
    Insert("<singleOrRecId>", l->LIST, 46);
    Insert("TK_ID", l->LIST, 46);
    Insert("<SorRecHelper>", l->LIST, 46);

    // Inserting <SorRecHelper> rules
    Insert("<SorRecHelper>", l->LIST, 47);
    Insert("<oneExpansion>", l->LIST, 47);
    Insert("<moreExpansions>", l->LIST, 47);

    Insert("<SorRecHelper>", l->LIST, 48);
    Insert("eps", l->LIST, 48);

    // Inserting <oneExpansion> rule
    Insert("<oneExpansion>", l->LIST, 49);
    Insert("TK_DOT", l->LIST, 49);
    Insert("TK_FIELDID", l->LIST, 49);

    // Inserting <moreExpansions> rules
    Insert("<moreExpansions>", l->LIST, 50);
    Insert("<oneExpansion>", l->LIST, 50);
    Insert("<moreExpansions>", l->LIST, 50);

    Insert("<moreExpansions>", l->LIST, 51);
    Insert("eps", l->LIST, 51);

    // Inserting <funCallStmt> rule
    Insert("<funCallStmt>", l->LIST, 52);
    Insert("<outputParameters>", l->LIST, 52);
    Insert("TK_CALL", l->LIST, 52);
    Insert("TK_FUNID", l->LIST, 52);
    Insert("TK_WITH", l->LIST, 52);
    Insert("TK_PARAMETERS", l->LIST, 52);
    Insert("<inputParameters>", l->LIST, 52);
    Insert("TK_SEM", l->LIST, 52);

    // Inserting <outputParameters> rules
    Insert("<outputParameters>", l->LIST, 53);
    Insert("TK_SQL", l->LIST, 53);
    Insert("<idList>", l->LIST, 53);
    Insert("TK_SQR", l->LIST, 53);
    Insert("TK_ASSIGNOP", l->LIST, 53);

    Insert("<outputParameters>", l->LIST, 54);
    Insert("eps", l->LIST, 54);

    // Inserting <inputParameters> rule
    Insert("<inputParameters>", l->LIST, 55);
    Insert("TK_SQL", l->LIST, 55);
    Insert("<idList>", l->LIST, 55);
    Insert("TK_SQR", l->LIST, 55);

    // Inserting <iterativeStmt> rule
    Insert("<iterativeStmt>", l->LIST, 56);
    Insert("TK_WHILE", l->LIST, 56);
    Insert("TK_OP", l->LIST, 56);
    Insert("<booleanExpression>", l->LIST, 56);
    Insert("TK_CL", l->LIST, 56);
    Insert("<stmt>", l->LIST, 56);
    Insert("<otherStmts>", l->LIST, 56);
    Insert("TK_ENDWHILE", l->LIST, 56);

    // Inserting <conditionalStmt> rule
    Insert("<conditionalStmt>", l->LIST, 57);
    Insert("TK_IF", l->LIST, 57);
    Insert("TK_OP", l->LIST, 57);
    Insert("<booleanExpression>", l->LIST, 57);
    Insert("TK_CL", l->LIST, 57);
    Insert("TK_THEN", l->LIST, 57);
    Insert("<stmt>", l->LIST, 57);
    Insert("<otherStmts>", l->LIST, 57);
    Insert("<elsePart>", l->LIST, 57);

    //  Insert("TK_ENDIF", l->LIST, 35);

    // Inserting <elsePart> rules
    Insert("<elsePart>", l->LIST, 58);
    Insert("TK_ELSE", l->LIST, 58);
    Insert("<stmt>", l->LIST, 58);
    Insert("<otherStmts>", l->LIST, 58);
    Insert("TK_ENDIF", l->LIST, 58);

    Insert("<elsePart>", l->LIST, 59);
    Insert("TK_ENDIF", l->LIST, 59);

    // Inserting <ioStmt> rules
    Insert("<ioStmt>", l->LIST, 60);
    Insert("TK_READ", l->LIST, 60);
    Insert("TK_OP", l->LIST, 60);
    Insert("<var>", l->LIST, 60);
    Insert("TK_CL", l->LIST, 60);
    Insert("TK_SEM", l->LIST, 60);

    Insert("<ioStmt>", l->LIST, 61);
    Insert("TK_WRITE", l->LIST, 61);
    Insert("TK_OP", l->LIST, 61);
    Insert("<var>", l->LIST, 61);
    Insert("TK_CL", l->LIST, 61);
    Insert("TK_SEM", l->LIST, 61);

    // Inserting <arithmeticExpression> rule
    Insert("<arithmeticExpression>", l->LIST, 62);
    Insert("<term>", l->LIST, 62);
    Insert("<expPrime>", l->LIST, 62);

    // Inserting <expPrime> rules
    Insert("<expPrime>", l->LIST, 63);
    Insert("<lowPrecedenceOperators>", l->LIST, 63);
    Insert("<term>", l->LIST, 63);
    Insert("<expPrime>", l->LIST, 63);

    Insert("<expPrime>", l->LIST, 64);
    Insert("eps", l->LIST, 64);

    // Inserting <term> rule
    Insert("<term>", l->LIST, 65);
    Insert("<factor>", l->LIST, 65);
    Insert("<termPrime>", l->LIST, 65);

    // Inserting <termPrime> rules
    Insert("<termPrime>", l->LIST, 66);
    Insert("<highPrecedenceOperators>", l->LIST, 66);
    Insert("<factor>", l->LIST, 66);
    Insert("<termPrime>", l->LIST, 66);

    Insert("<termPrime>", l->LIST, 67);
    Insert("eps", l->LIST, 67);

    // Inserting <factor> rule
    Insert("<factor>", l->LIST, 68);
    Insert("TK_OP", l->LIST, 68);
    Insert("<arithmeticExpression>", l->LIST, 68);
    Insert("TK_CL", l->LIST, 68);

    Insert("<factor>", l->LIST, 69);
    Insert("<var>", l->LIST, 69);

    // Inserting <highPrecedenceOperators> rules
    Insert("<highPrecedenceOperators>", l->LIST, 70);
    Insert("TK_MUL", l->LIST, 70);

    Insert("<highPrecedenceOperators>", l->LIST, 71);
    Insert("TK_DIV", l->LIST, 71);

    // Inserting <lowPrecedenceOperators> rules
    Insert("<lowPrecedenceOperators>", l->LIST, 72);
    Insert("TK_PLUS", l->LIST, 72);

    Insert("<lowPrecedenceOperators>", l->LIST, 73);
    Insert("TK_MINUS", l->LIST, 73);

    // Inserting <booleanExpression> rules
    Insert("<booleanExpression>", l->LIST, 74);
    Insert("TK_OP", l->LIST, 74);
    Insert("<booleanExpression>", l->LIST, 74);
    Insert("TK_CL", l->LIST, 74);
    Insert("<logicalOp>", l->LIST, 74);
    Insert("TK_OP", l->LIST, 74);
    Insert("<booleanExpression>", l->LIST, 74);
    Insert("TK_CL", l->LIST, 74);

    Insert("<booleanExpression>", l->LIST, 75);
    Insert("<var>", l->LIST, 75);
    Insert("<relationalOp>", l->LIST, 75);
    Insert("<var>", l->LIST, 75);

    Insert("<booleanExpression>", l->LIST, 76);
    Insert("TK_NOT", l->LIST, 76);
    Insert("TK_OP", l->LIST, 76);
    Insert("<booleanExpression>", l->LIST, 76);
    Insert("TK_CL", l->LIST, 76);

    // Inserting <var> rules
    Insert("<var>", l->LIST, 77);
    Insert("<singleOrRecId>", l->LIST, 77);

    Insert("<var>", l->LIST, 78);
    Insert("TK_NUM", l->LIST, 78);

    Insert("<var>", l->LIST, 79);
    Insert("TK_RNUM", l->LIST, 79);

    // Inserting <logicalOp> rules
    Insert("<logicalOp>", l->LIST, 80);
    Insert("TK_AND", l->LIST, 80);
    Insert("<logicalOp>", l->LIST, 81);
    Insert("TK_OR", l->LIST, 81);

    // Inserting <relationalOp> rules
    Insert("<relationalOp>", l->LIST, 82);
    Insert("TK_LT", l->LIST, 82);
    Insert("<relationalOp>", l->LIST, 83);
    Insert("TK_LE", l->LIST, 83);
    Insert("<relationalOp>", l->LIST, 84);
    Insert("TK_EQ", l->LIST, 84);
    Insert("<relationalOp>", l->LIST, 85);
    Insert("TK_GT", l->LIST, 85);
    Insert("<relationalOp>", l->LIST, 86);
    Insert("TK_GE", l->LIST, 86);
    Insert("<relationalOp>", l->LIST, 87);
    Insert("TK_NE", l->LIST, 87);

    // Inserting <returnStmt> rule
    Insert("<returnStmt>", l->LIST, 88);
    Insert("TK_RETURN", l->LIST, 88);
    Insert("<optionalReturn>", l->LIST, 88);
    Insert("TK_SEM", l->LIST, 88);

    // Inserting <optionalReturn> rules
    Insert("<optionalReturn>", l->LIST, 89);
    Insert("TK_SQL", l->LIST, 89);
    Insert("<idList>", l->LIST, 89);
    Insert("TK_SQR", l->LIST, 89);

    Insert("<optionalReturn>", l->LIST, 90);
    Insert("eps", l->LIST, 90);

    // Inserting <idList> rules
    Insert("<idList>", l->LIST, 91);
    Insert("TK_ID", l->LIST, 91);
    Insert("<more_ids>", l->LIST, 91);

    // Inserting <more_ids> rules
    Insert("<more_ids>", l->LIST, 92);
    Insert("TK_COMMA", l->LIST, 92);
    Insert("<idList>", l->LIST, 92);

    Insert("<more_ids>", l->LIST, 93);
    Insert("eps", l->LIST, 93);

    // Inserting <definetypestmt> rule
    Insert("<definetypestmt>", l->LIST, 94);
    Insert("TK_DEFINETYPE", l->LIST, 94);
    Insert("<A>", l->LIST, 94);
    Insert("TK_RUID", l->LIST, 94);
    Insert("TK_AS", l->LIST, 94);
    Insert("TK_RUID", l->LIST, 94);

    // Inserting <A> rules
    Insert("<A>", l->LIST, 95);
    Insert("TK_RECORD", l->LIST, 95);

    Insert("<A>", l->LIST, 96);
    Insert("TK_UNION", l->LIST, 96);
    return l;
}

void mainParser(FILE *fp, char *outfile)
{
    char *parseT[non_tokens][tokens][max_gram_len];
    for (int i = 0; i < non_tokens; i++)
    {
        for (int j = 0; j < 57; j++)
        {
            for (int k = 0; k < max_gram_len; k++)
            {
                parseT[i][j][k] = (char *)malloc(6);
                strcpy(parseT[i][j][k], "ERROR");
            }
        }
    }
    hashmap *h1 = inithashmap(); // non terminals
    insert("<program>", 0, h1);
    insert("<otherFunctions>", 1, h1);
    insert("<mainFunction>", 2, h1);
    insert("<stmts>", 3, h1);
    insert("<function>", 4, h1);
    insert("<input_par>", 5, h1);
    insert("<output_par>", 6, h1);
    insert("<parameter_list>", 7, h1);
    insert("<dataType>", 8, h1);
    insert("<remaining_list>", 9, h1);
    insert("<primitiveDatatype>", 10, h1);
    insert("<constructedDatatype>", 11, h1);
    insert("<typeDefinitions>", 12, h1);
    insert("<declarations>", 13, h1);
    insert("<otherStmts>", 14, h1);
    insert("<returnStmt>", 15, h1);
    insert("<actualOrRedefined>", 16, h1);
    insert("<typeDefinition>", 17, h1);
    insert("<definetypestmt>", 18, h1);
    insert("<fieldDefinitions>", 19, h1);
    insert("<fieldDefinition>", 20, h1);
    insert("<moreFields>", 21, h1);
    insert("<fieldtype>", 22, h1);
    insert("<declaration>", 23, h1);
    insert("<global_or_not>", 24, h1);
    insert("<stmt>", 25, h1);
    insert("<assignmentStmt>", 26, h1);
    insert("<iterativeStmt>", 27, h1);
    insert("<conditionalStmt>", 28, h1);
    insert("<ioStmt>", 29, h1);
    insert("<funCallStmt>", 30, h1);
    insert("<singleOrRecId>", 31, h1);
    insert("<arithmeticExpression>", 32, h1);
    insert("<SorRecHelper>", 33, h1);
    insert("<oneExpansion>", 34, h1);
    insert("<moreExpansions>", 35, h1);
    insert("<outputParameters>", 36, h1);
    insert("<inputParameters>", 37, h1);
    insert("<idList>", 38, h1);
    insert("<booleanExpression>", 39, h1);
    insert("<elsePart>", 40, h1);
    insert("<var>", 41, h1);
    insert("<term>", 42, h1);
    insert("<expPrime>", 43, h1);
    insert("<lowPrecedenceOperators>", 44, h1);
    insert("<factor>", 45, h1);
    insert("<termPrime>", 46, h1);
    insert("<highPrecedenceOperators>", 47, h1);
    insert("<logicalOp>", 48, h1);
    insert("<relationalOp>", 49, h1);
    insert("<optionalReturn>", 50, h1);
    insert("<more_ids>", 51, h1);
    insert("<A>", 52, h1);

    hashmap *h2 = inithashmap(); // terminals + $
    insert("TK_MAIN", 0, h2);
    insert("TK_END", 1, h2);
    insert("eps", 2, h2);
    insert("TK_FUNID", 3, h2);
    insert("TK_SEM", 4, h2);
    insert("TK_INPUT", 5, h2);
    insert("TK_PARAMETER", 6, h2);
    insert("TK_LIST", 7, h2);
    insert("TK_SQL", 8, h2);
    insert("TK_SQR", 9, h2);
    insert("TK_OUTPUT", 10, h2);
    insert("TK_ID", 11, h2);
    insert("TK_INT", 12, h2);
    insert("TK_REAL", 13, h2);
    insert("TK_RECORD", 14, h2);
    insert("TK_RUID", 15, h2);
    insert("TK_UNION", 16, h2);
    insert("TK_COMMA", 17, h2);
    insert("TK_ENDRECORD", 18, h2);
    insert("TK_ENDUNION", 19, h2);
    insert("TK_TYPE", 20, h2);
    insert("TK_COLON", 21, h2);
    insert("TK_FIELDID", 22, h2);
    insert("TK_GLOBAL", 23, h2);
    insert("TK_ASSIGNOP", 24, h2);
    insert("TK_DOT", 25, h2);
    insert("TK_CALL", 26, h2);
    insert("TK_WITH", 27, h2);
    insert("TK_PARAMETERS", 28, h2);
    insert("TK_WHILE", 29, h2);
    insert("TK_OP", 30, h2);
    insert("TK_CL", 31, h2);
    insert("TK_ENDWHILE", 32, h2);
    insert("TK_IF", 33, h2);
    insert("TK_THEN", 34, h2);
    insert("TK_ELSE", 35, h2);
    insert("TK_ENDIF", 36, h2);
    insert("TK_READ", 37, h2);
    insert("TK_WRITE", 38, h2);
    insert("TK_MUL", 39, h2);
    insert("TK_DIV", 40, h2);
    insert("TK_PLUS", 41, h2);
    insert("TK_MINUS", 42, h2);
    insert("TK_NOT", 43, h2);
    insert("TK_NUM", 44, h2);
    insert("TK_RNUM", 45, h2);
    insert("TK_AND", 46, h2);
    insert("TK_OR", 47, h2);
    insert("TK_LT", 48, h2);
    insert("TK_LE", 49, h2);
    insert("TK_EQ", 50, h2);
    insert("TK_GT", 51, h2);
    insert("TK_GE", 52, h2);
    insert("TK_NE", 53, h2);
    insert("TK_RETURN", 54, h2);
    insert("TK_DEFINETYPE", 55, h2);
    insert("TK_AS", 56, h2);
    insert("$", 57, h2);

    first_node **firstArr = (first_node **)malloc(sizeof(first_node *) * non_tokens);
    first_follow_node **followArr = (first_follow_node **)malloc(sizeof(first_follow_node *) * non_tokens);

    for (int i = 0; i < non_tokens; i++)
    {
        firstArr[i] = NULL;
        followArr[i] = NULL;
    }

    // first_follow_node*node1=(first_follow_node*)malloc(sizeof(first_follow_node));
    // populate_node(node1,"TK_MAIN");
    // node1->next=firstArr[0];
    // firstArr[0]=node1;
    firstFollow(firstArr, "TK_MAIN", 0, 0);
    firstFollow(firstArr, "eps", 0, 0);
    firstFollow(firstArr, "TK_FUNID", 0, 0);
    firstFollow(firstArr, "TK_FUNID", 1, 2);
    firstFollow(firstArr, "eps", 1, 3);
    firstFollow(firstArr, "TK_MAIN", 2, 1);
    firstFollow(firstArr, "TK_RETURN", 3, 18);
    firstFollow(firstArr, "eps", 3, 18);
    firstFollow(firstArr, "TK_ID", 3, 18);
    firstFollow(firstArr, "TK_WHILE", 3, 18);
    firstFollow(firstArr, "TK_IF", 3, 18);
    firstFollow(firstArr, "TK_WRITE", 3, 18);
    firstFollow(firstArr, "TK_READ", 3, 18);
    firstFollow(firstArr, "TK_SQL", 3, 18);
    firstFollow(firstArr, "eps", 3, 18);
    firstFollow(firstArr, "TK_CALL", 3, 18);
    firstFollow(firstArr, "eps", 3, 18);
    firstFollow(firstArr, "TK_TYPE", 3, 18);
    firstFollow(firstArr, "eps", 3, 18);
    firstFollow(firstArr, "TK_UNION", 3, 18);
    firstFollow(firstArr, "TK_RECORD", 3, 18);
    firstFollow(firstArr, "TK_DEFINETYPE", 3, 18);
    firstFollow(firstArr, "TK_FUNID", 4, 4);
    firstFollow(firstArr, "TK_INPUT", 5, 5);
    firstFollow(firstArr, "TK_OUTPUT", 6, 6);
    firstFollow(firstArr, "eps", 6, 7);
    firstFollow(firstArr, "TK_RECORD", 7, 8);
    firstFollow(firstArr, "TK_UNION", 7, 8);
    firstFollow(firstArr, "TK_RUID", 7, 8);
    firstFollow(firstArr, "TK_INT", 7, 8);
    firstFollow(firstArr, "TK_REAL", 7, 8);
    firstFollow(firstArr, "TK_REAL", 8, 9);
    firstFollow(firstArr, "TK_INT", 8, 9);
    firstFollow(firstArr, "TK_RUID", 8, 10);
    firstFollow(firstArr, "TK_UNION", 8, 10);
    firstFollow(firstArr, "TK_RECORD", 8, 10);
    firstFollow(firstArr, "TK_COMMA", 9, 16);
    firstFollow(firstArr, "eps", 9, 17);
    firstFollow(firstArr, "TK_INT", 10, 11);
    firstFollow(firstArr, "TK_REAL", 10, 12);
    firstFollow(firstArr, "TK_RECORD", 11, 13);
    firstFollow(firstArr, "TK_UNION", 11, 14);
    firstFollow(firstArr, "TK_RUID", 11, 15);
    firstFollow(firstArr, "TK_DEFINETYPE", 12, 19);
    firstFollow(firstArr, "TK_RECORD", 12, 19);
    firstFollow(firstArr, "TK_UNION", 12, 19);
    firstFollow(firstArr, "eps", 12, 20);
    firstFollow(firstArr, "TK_TYPE", 13, 32);
    firstFollow(firstArr, "eps", 13, 33);
    firstFollow(firstArr, "TK_CALL", 14, 37);
    firstFollow(firstArr, "eps", 14, 37);
    firstFollow(firstArr, "TK_SQL", 14, 37);
    firstFollow(firstArr, "TK_READ", 14, 37);
    firstFollow(firstArr, "TK_WRITE", 14, 37);
    firstFollow(firstArr, "TK_IF", 14, 37);
    firstFollow(firstArr, "TK_WHILE", 14, 37);
    firstFollow(firstArr, "TK_ID", 14, 37);
    firstFollow(firstArr, "eps", 14, 38);
    firstFollow(firstArr, "TK_RETURN", 15, 87);
    firstFollow(firstArr, "TK_UNION", 16, 21);
    firstFollow(firstArr, "TK_RECORD", 16, 21);
    firstFollow(firstArr, "TK_DEFINETYPE", 16, 22);
    firstFollow(firstArr, "TK_RECORD", 17, 23);
    firstFollow(firstArr, "TK_UNION", 17, 24);
    firstFollow(firstArr, "TK_DEFINETYPE", 18, 93);
    firstFollow(firstArr, "TK_RECORD", 19, 25);
    firstFollow(firstArr, "TK_UNION", 19, 25);
    firstFollow(firstArr, "TK_TYPE", 19, 25);
    firstFollow(firstArr, "TK_TYPE", 20, 26);
    firstFollow(firstArr, "TK_UNION", 20, 27);
    firstFollow(firstArr, "TK_RECORD", 20, 27);
    firstFollow(firstArr, "TK_RECORD", 21, 30);
    firstFollow(firstArr, "TK_UNION", 21, 30);
    firstFollow(firstArr, "TK_TYPE", 21, 30);
    firstFollow(firstArr, "eps", 21, 31);
    firstFollow(firstArr, "TK_REAL", 22, 28);
    firstFollow(firstArr, "TK_INT", 22, 28);
    firstFollow(firstArr, "TK_RUID", 22, 29);
    firstFollow(firstArr, "TK_UNION", 22, 29);
    firstFollow(firstArr, "TK_RECORD", 22, 29);
    firstFollow(firstArr, "TK_TYPE", 23, 34);
    firstFollow(firstArr, "TK_COLON", 24, 35);
    firstFollow(firstArr, "eps", 24, 36);
    firstFollow(firstArr, "TK_ID", 25, 39);
    firstFollow(firstArr, "TK_WHILE", 25, 40);
    firstFollow(firstArr, "TK_IF", 25, 41);
    firstFollow(firstArr, "TK_WRITE", 25, 42);
    firstFollow(firstArr, "TK_READ", 25, 42);
    firstFollow(firstArr, "TK_SQL", 25, 43);
    firstFollow(firstArr, "eps", 25, 43);
    firstFollow(firstArr, "TK_CALL", 25, 43);
    firstFollow(firstArr, "TK_ID", 26, 44);
    firstFollow(firstArr, "TK_WHILE", 27, 55);
    firstFollow(firstArr, "TK_IF", 28, 56);
    firstFollow(firstArr, "TK_WRITE", 29, 60);
    firstFollow(firstArr, "TK_READ", 29, 59);
    firstFollow(firstArr, "TK_CALL", 30, 51);
    firstFollow(firstArr, "eps", 30, 51);
    firstFollow(firstArr, "TK_SQL", 30, 51);
    firstFollow(firstArr, "TK_ID", 31, 45);
    firstFollow(firstArr, "TK_OP", 32, 61);
    firstFollow(firstArr, "TK_RNUM", 32, 61);
    firstFollow(firstArr, "TK_NUM", 32, 61);
    firstFollow(firstArr, "TK_ID", 32, 61);
    firstFollow(firstArr, "TK_DOT", 33, 46);
    firstFollow(firstArr, "eps", 33, 47);
    firstFollow(firstArr, "TK_DOT", 34, 48);
    firstFollow(firstArr, "TK_DOT", 35, 49);
    firstFollow(firstArr, "eps", 35, 50);
    firstFollow(firstArr, "TK_SQL", 36, 52);
    firstFollow(firstArr, "eps", 36, 53);
    firstFollow(firstArr, "TK_SQL", 37, 54);
    firstFollow(firstArr, "TK_ID", 38, 90);
    firstFollow(firstArr, "TK_OP", 39, 73);
    firstFollow(firstArr, "TK_RNUM", 39, 74);
    firstFollow(firstArr, "TK_NUM", 39, 74);
    firstFollow(firstArr, "TK_ID", 39, 74);
    firstFollow(firstArr, "TK_NOT", 39, 75);
    firstFollow(firstArr, "TK_ELSE", 40, 57);
    firstFollow(firstArr, "TK_ENDIF", 40, 58);
    firstFollow(firstArr, "TK_ID", 41, 76);
    firstFollow(firstArr, "TK_NUM", 41, 77);
    firstFollow(firstArr, "TK_RNUM", 41, 78);
    firstFollow(firstArr, "TK_ID", 42, 64);
    firstFollow(firstArr, "TK_NUM", 42, 64);
    firstFollow(firstArr, "TK_RNUM", 42, 64);
    firstFollow(firstArr, "TK_OP", 42, 64);
    firstFollow(firstArr, "TK_MINUS", 43, 62);
    firstFollow(firstArr, "TK_PLUS", 43, 62);
    firstFollow(firstArr, "eps", 43, 63);
    firstFollow(firstArr, "TK_PLUS", 44, 71);
    firstFollow(firstArr, "TK_MINUS", 44, 72);
    firstFollow(firstArr, "TK_OP", 45, 67);
    firstFollow(firstArr, "TK_RNUM", 45, 68);
    firstFollow(firstArr, "TK_NUM", 45, 68);
    firstFollow(firstArr, "TK_ID", 45, 68);
    firstFollow(firstArr, "TK_DIV", 46, 65);
    firstFollow(firstArr, "TK_MUL", 46, 65);
    firstFollow(firstArr, "eps", 46, 66);
    firstFollow(firstArr, "TK_MUL", 47, 69);
    firstFollow(firstArr, "TK_DIV", 47, 70);
    firstFollow(firstArr, "TK_AND", 48, 79);
    firstFollow(firstArr, "TK_OR", 48, 80);
    firstFollow(firstArr, "TK_LT", 49, 81);
    firstFollow(firstArr, "TK_LE", 49, 82);
    firstFollow(firstArr, "TK_EQ", 49, 83);
    firstFollow(firstArr, "TK_GT", 49, 84);
    firstFollow(firstArr, "TK_GE", 49, 85);
    firstFollow(firstArr, "TK_NE", 49, 86);
    firstFollow(firstArr, "TK_SQL", 50, 88);
    firstFollow(firstArr, "eps", 50, 89);
    firstFollow(firstArr, "TK_COMMA", 51, 91);
    firstFollow(firstArr, "eps", 51, 92);
    firstFollow(firstArr, "TK_RECORD", 52, 94);
    firstFollow(firstArr, "TK_UNION", 52, 95);

    firstAdder(followArr, "$", 0);
    firstAdder(followArr, "TK_MAIN", 1);
    firstAdder(followArr, "$", 2);
    firstAdder(followArr, "TK_END", 3);
    firstAdder(followArr, "TK_WHILE", 25);
    firstAdder(followArr, "TK_IF", 25);
    firstAdder(followArr, "TK_READ", 25);
    firstAdder(followArr, "TK_WRITE", 25);
    firstAdder(followArr, "TK_ID", 25);
    firstAdder(followArr, "TK_SQL", 25);
    firstAdder(followArr, "TK_CALL", 25);
    firstAdder(followArr, "TK_RETURN", 25);
    firstAdder(followArr, "TK_ENDWHILE", 25);
    firstAdder(followArr, "TK_ELSE", 25);
    firstAdder(followArr, "TK_ENDIF", 25);
    firstAdder(followArr, "TK_FUNID", 4);
    firstAdder(followArr, "TK_MAIN", 4);
    firstAdder(followArr, "TK_OUTPUT", 5);
    firstAdder(followArr, "TK_SEM", 5);
    firstAdder(followArr, "TK_SEM", 6);
    firstAdder(followArr, "TK_SQR", 7);
    firstAdder(followArr, "TK_ID", 8);
    firstAdder(followArr, "TK_COLON", 8);
    firstAdder(followArr, "TK_SQR", 9);
    firstAdder(followArr, "TK_ID", 10);
    firstAdder(followArr, "TK_COLON", 10);
    firstAdder(followArr, "TK_ID", 11);
    firstAdder(followArr, "TK_COLON", 11);
    firstAdder(followArr, "TK_TYPE", 12);
    firstAdder(followArr, "TK_WHILE", 12);
    firstAdder(followArr, "TK_IF", 12);
    firstAdder(followArr, "TK_READ", 12);
    firstAdder(followArr, "TK_WRITE", 12);
    firstAdder(followArr, "TK_ID", 12);
    firstAdder(followArr, "TK_SQL", 12);
    firstAdder(followArr, "TK_CALL", 12);
    firstAdder(followArr, "TK_RETURN", 12);

    firstAdder(followArr, "TK_RECORD", 16);
    firstAdder(followArr, "TK_UNION", 16);
    firstAdder(followArr, "TK_DEFINETYPE", 16);
    firstAdder(followArr, "TK_TYPE", 16);
    firstAdder(followArr, "TK_WHILE", 16);
    firstAdder(followArr, "TK_IF", 16);
    firstAdder(followArr, "TK_READ", 16);
    firstAdder(followArr, "TK_WRITE", 16);
    firstAdder(followArr, "TK_ID", 16);
    firstAdder(followArr, "TK_SQL", 16);
    firstAdder(followArr, "TK_CALL", 16);
    firstAdder(followArr, "TK_RETURN", 16);

    firstAdder(followArr, "TK_RECORD", 17);
    firstAdder(followArr, "TK_UNION", 17);
    firstAdder(followArr, "TK_DEFINETYPE", 17);
    firstAdder(followArr, "TK_TYPE", 17);
    firstAdder(followArr, "TK_WHILE", 17);
    firstAdder(followArr, "TK_IF", 17);
    firstAdder(followArr, "TK_READ", 17);
    firstAdder(followArr, "TK_WRITE", 17);
    firstAdder(followArr, "TK_ID", 17);
    firstAdder(followArr, "TK_SQL", 17);
    firstAdder(followArr, "TK_CALL", 17);
    firstAdder(followArr, "TK_RETURN", 17);

    firstAdder(followArr, "TK_WHILE", 13);
    firstAdder(followArr, "TK_IF", 13);
    firstAdder(followArr, "TK_READ", 13);
    firstAdder(followArr, "TK_WRITE", 13);
    firstAdder(followArr, "TK_ID", 13);
    firstAdder(followArr, "TK_SQL", 13);
    firstAdder(followArr, "TK_CALL", 13);
    firstAdder(followArr, "TK_RETURN", 13);

    firstAdder(followArr, "TK_TYPE", 23);
    firstAdder(followArr, "TK_WHILE", 23);
    firstAdder(followArr, "TK_IF", 23);
    firstAdder(followArr, "TK_READ", 23);
    firstAdder(followArr, "TK_WRITE", 23);
    firstAdder(followArr, "TK_ID", 23);
    firstAdder(followArr, "TK_SQL", 23);
    firstAdder(followArr, "TK_CALL", 23);
    firstAdder(followArr, "TK_RETURN", 23);

    firstAdder(followArr, "TK_RETURN", 14);
    firstAdder(followArr, "TK_ENDWHILE", 14);
    firstAdder(followArr, "TK_ELSE", 14);
    firstAdder(followArr, "TK_ENDIF", 14);

    firstAdder(followArr, "TK_END", 15);

    firstAdder(followArr, "TK_RECORD", 18);
    firstAdder(followArr, "TK_UNION", 18);
    firstAdder(followArr, "TK_DEFINETYPE", 18);
    firstAdder(followArr, "TK_TYPE", 18);
    firstAdder(followArr, "TK_WHILE", 18);
    firstAdder(followArr, "TK_IF", 18);
    firstAdder(followArr, "TK_READ", 18);
    firstAdder(followArr, "TK_WRITE", 18);
    firstAdder(followArr, "TK_ID", 18);
    firstAdder(followArr, "TK_SQL", 18);
    firstAdder(followArr, "TK_CALL", 18);
    firstAdder(followArr, "TK_RETURN", 18);

    firstAdder(followArr, "TK_TYPE", 20);
    firstAdder(followArr, "TK_ENDRECORD", 20);
    firstAdder(followArr, "TK_ENDUNION", 20);

    firstAdder(followArr, "TK_COLON", 22);

    firstAdder(followArr, "TK_ENDRECORD", 19);
    firstAdder(followArr, "TK_ENDUNION", 19);
    firstAdder(followArr, "TK_ENDRECORD", 21);
    firstAdder(followArr, "TK_ENDUNION", 21);
    firstAdder(followArr, "TK_SEM", 24);
    firstAdder(followArr, "TK_WHILE", 26);
    firstAdder(followArr, "TK_IF", 26);
    firstAdder(followArr, "TK_READ", 26);
    firstAdder(followArr, "TK_WRITE", 26);
    firstAdder(followArr, "TK_ID", 26);
    firstAdder(followArr, "TK_SQL", 26);
    firstAdder(followArr, "TK_CALL", 26);
    firstAdder(followArr, "TK_RETURN", 26);
    firstAdder(followArr, "TK_ENDWHILE", 26);
    firstAdder(followArr, "TK_ELSE", 26);
    firstAdder(followArr, "TK_ENDIF", 26);
    firstAdder(followArr, "TK_WHILE", 27);
    firstAdder(followArr, "TK_IF", 27);
    firstAdder(followArr, "TK_READ", 27);
    firstAdder(followArr, "TK_WRITE", 27);
    firstAdder(followArr, "TK_ID", 27);
    firstAdder(followArr, "TK_SQL", 27);
    firstAdder(followArr, "TK_CALL", 27);
    firstAdder(followArr, "TK_RETURN", 27);
    firstAdder(followArr, "TK_ENDWHILE", 27);
    firstAdder(followArr, "TK_ELSE", 27);
    firstAdder(followArr, "TK_ENDIF", 27);
    firstAdder(followArr, "TK_WHILE", 28);
    firstAdder(followArr, "TK_IF", 28);
    firstAdder(followArr, "TK_READ", 28);
    firstAdder(followArr, "TK_WRITE", 28);
    firstAdder(followArr, "TK_ID", 28);
    firstAdder(followArr, "TK_SQL", 28);
    firstAdder(followArr, "TK_CALL", 28);
    firstAdder(followArr, "TK_RETURN", 28);
    firstAdder(followArr, "TK_ENDWHILE", 28);
    firstAdder(followArr, "TK_ELSE", 28);
    firstAdder(followArr, "TK_ENDIF", 28);
    firstAdder(followArr, "TK_WHILE", 40);
    firstAdder(followArr, "TK_IF", 40);
    firstAdder(followArr, "TK_READ", 40);
    firstAdder(followArr, "TK_WRITE", 40);
    firstAdder(followArr, "TK_ID", 40);
    firstAdder(followArr, "TK_SQL", 40);
    firstAdder(followArr, "TK_CALL", 40);
    firstAdder(followArr, "TK_RETURN", 40);
    firstAdder(followArr, "TK_ENDWHILE", 40);
    firstAdder(followArr, "TK_ELSE", 40);
    firstAdder(followArr, "TK_ENDIF", 40);
    firstAdder(followArr, "TK_WHILE", 29);
    firstAdder(followArr, "TK_IF", 29);
    firstAdder(followArr, "TK_READ", 29);
    firstAdder(followArr, "TK_WRITE", 29);
    firstAdder(followArr, "TK_ID", 29);
    firstAdder(followArr, "TK_SQL", 29);
    firstAdder(followArr, "TK_CALL", 29);
    firstAdder(followArr, "TK_RETURN", 29);
    firstAdder(followArr, "TK_ENDWHILE", 29);
    firstAdder(followArr, "TK_ELSE", 29);
    firstAdder(followArr, "TK_ENDIF", 29);
    firstAdder(followArr, "TK_WHILE", 30);
    firstAdder(followArr, "TK_IF", 30);
    firstAdder(followArr, "TK_READ", 30);
    firstAdder(followArr, "TK_WRITE", 30);
    firstAdder(followArr, "TK_ID", 30);
    firstAdder(followArr, "TK_SQL", 30);
    firstAdder(followArr, "TK_CALL", 30);
    firstAdder(followArr, "TK_RETURN", 30);
    firstAdder(followArr, "TK_ENDWHILE", 30);
    firstAdder(followArr, "TK_ELSE", 30);
    firstAdder(followArr, "TK_ENDIF", 30);
    firstAdder(followArr, "TK_ASSIGNOP", 31);
    firstAdder(followArr, "TK_CL", 31);
    firstAdder(followArr, "TK_MUL", 31);
    firstAdder(followArr, "TK_DIV", 31);
    firstAdder(followArr, "TK_PLUS", 31);
    firstAdder(followArr, "TK_MINUS", 31);
    firstAdder(followArr, "TK_SEM", 31);
    firstAdder(followArr, "TK_LT", 31);
    firstAdder(followArr, "TK_LE", 31);
    firstAdder(followArr, "TK_EQ", 31);
    firstAdder(followArr, "TK_GT", 31);
    firstAdder(followArr, "TK_GE", 31);
    firstAdder(followArr, "TK_NE", 31);
    firstAdder(followArr, "TK_ASSIGNOP", 33);
    firstAdder(followArr, "TK_CL", 33);
    firstAdder(followArr, "TK_MUL", 33);
    firstAdder(followArr, "TK_DIV", 33);
    firstAdder(followArr, "TK_PLUS", 33);
    firstAdder(followArr, "TK_MINUS", 33);
    firstAdder(followArr, "TK_SEM", 33);
    firstAdder(followArr, "TK_LT", 33);
    firstAdder(followArr, "TK_LE", 33);
    firstAdder(followArr, "TK_EQ", 33);
    firstAdder(followArr, "TK_GT", 33);
    firstAdder(followArr, "TK_GE", 33);
    firstAdder(followArr, "TK_NE", 33);
    firstAdder(followArr, "TK_DOT", 34);
    firstAdder(followArr, "TK_ASSIGNOP", 34);
    firstAdder(followArr, "TK_CL", 34);
    firstAdder(followArr, "TK_MUL", 34);
    firstAdder(followArr, "TK_DIV", 34);
    firstAdder(followArr, "TK_PLUS", 34);
    firstAdder(followArr, "TK_MINUS", 34);
    firstAdder(followArr, "TK_SEM", 34);
    firstAdder(followArr, "TK_LT", 34);
    firstAdder(followArr, "TK_LE", 34);
    firstAdder(followArr, "TK_EQ", 34);
    firstAdder(followArr, "TK_GT", 34);
    firstAdder(followArr, "TK_GE", 34);
    firstAdder(followArr, "TK_NE", 34);
    firstAdder(followArr, "TK_ASSIGNOP", 35);
    firstAdder(followArr, "TK_CL", 35);
    firstAdder(followArr, "TK_MUL", 35);
    firstAdder(followArr, "TK_DIV", 35);
    firstAdder(followArr, "TK_PLUS", 35);
    firstAdder(followArr, "TK_MINUS", 35);
    firstAdder(followArr, "TK_SEM", 35);
    firstAdder(followArr, "TK_LT", 35);
    firstAdder(followArr, "TK_LE", 35);
    firstAdder(followArr, "TK_EQ", 35);
    firstAdder(followArr, "TK_GT", 35);
    firstAdder(followArr, "TK_GE", 35);
    firstAdder(followArr, "TK_NE", 35);
    firstAdder(followArr, "TK_SEM", 32);
    firstAdder(followArr, "TK_CL", 32);
    firstAdder(followArr, "TK_SEM", 43);
    firstAdder(followArr, "TK_CL", 43);
    firstAdder(followArr, "TK_PLUS", 42);
    firstAdder(followArr, "TK_MINUS", 42);
    firstAdder(followArr, "TK_SEM", 42);
    firstAdder(followArr, "TK_CL", 42);
    firstAdder(followArr, "TK_PLUS", 46);
    firstAdder(followArr, "TK_MINUS", 46);
    firstAdder(followArr, "TK_SEM", 46);
    firstAdder(followArr, "TK_CL", 46);
    firstAdder(followArr, "TK_MUL", 45);
    firstAdder(followArr, "TK_DIV", 45);
    firstAdder(followArr, "TK_PLUS", 45);
    firstAdder(followArr, "TK_MINUS", 45);
    firstAdder(followArr, "TK_SEM", 45);
    firstAdder(followArr, "TK_CL", 45);
    firstAdder(followArr, "TK_OP", 47);
    firstAdder(followArr, "TK_ID", 47);
    firstAdder(followArr, "TK_NUM", 47);
    firstAdder(followArr, "TK_RNUM", 47);
    firstAdder(followArr, "TK_OP", 44);
    firstAdder(followArr, "TK_ID", 44);
    firstAdder(followArr, "TK_NUM", 44);
    firstAdder(followArr, "TK_RNUM", 44);
    firstAdder(followArr, "TK_CALL", 36);
    firstAdder(followArr, "TK_SEM", 37);
    firstAdder(followArr, "TK_SQR", 38);
    firstAdder(followArr, "TK_CL", 39);
    firstAdder(followArr, "TK_CL", 41);
    firstAdder(followArr, "TK_MUL", 41);
    firstAdder(followArr, "TK_DIV", 41);
    firstAdder(followArr, "TK_PLUS", 41);
    firstAdder(followArr, "TK_MINUS", 41);
    firstAdder(followArr, "TK_SEM", 41);
    firstAdder(followArr, "TK_LT", 41);
    firstAdder(followArr, "TK_LE", 41);
    firstAdder(followArr, "TK_EQ", 41);
    firstAdder(followArr, "TK_GT", 41);
    firstAdder(followArr, "TK_GE", 41);
    firstAdder(followArr, "TK_NE", 41);
    firstAdder(followArr, "TK_OP", 48);
    firstAdder(followArr, "TK_ID", 49);
    firstAdder(followArr, "TK_NUM", 49);
    firstAdder(followArr, "TK_RNUM", 49);
    firstAdder(followArr, "TK_SEM", 50);
    firstAdder(followArr, "TK_SQR", 51);
    firstAdder(followArr, "TK_RUID", 52);

    stack *st = initStack();

    push(st, "$", 0, NULL);
    parseTreeNode *root1 = initParseTreeNode(NULL, -1, NULL, NULL, NULL, "ROOT", 0);
    parseTreeNode *root = initParseTreeNode("----", 0, NULL, NULL, root1, "<program>", 0);
    push(st, "<program>", 1, root);
    // printf("%s\n",parseT[0][0][0]);
    // lex_header*input=(lex_header*)malloc(sizeof(lex_header));
    // FILE*foo=fopen("text.txt","r");
    // symTable* map=initsymbolTable();
    // print(foo,map,input);

    symTable *map = initsymbolTable();
    lex_header *input = create_Larray();
    // fp = fopen("text.txt", "r");
    getStream(fp);
    print(fp, map, input);
    char *u = "$";

    rewind(fp);
    int local_line_no = 1;
    char c;
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            local_line_no = local_line_no + 1;

    lexeme *lex = new_lex("$", u, local_line_no);
    add_lex(input, lex);
    struct List *l = GRAMMAR();
    createParseTable(firstArr, h2, followArr, l, parseT);

    parser(st, input, parseT, h1, h2);
    printf("\n");
    // FILE*fptr = fopen(outfile, "w");
    printParseTree(root, outfile);
}
