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
#include "lexer.h"
#include "symboltable.c"

#define L_size 10000
#define BUFFERSIZE 100

FILE *fp;
char DoubleBuffer[2][BUFFERSIZE];
int p;
int preend;
int readSize;
int merge;
int Line_No;

lexeme *new_lex(char *c, char *t, int lineno)
{
    lexeme *L = (lexeme *)malloc(sizeof(lexeme));
    L->lexe = c;
    L->token = t;
    L->line_no = lineno;
    return L;
}
lex_header *create_Larray()
{
    lex_header *head = (lex_header *)malloc(sizeof(lex_header));
    head->size = 0;
}
void add_lex(lex_header *head, lexeme *l)
{
    head->arr[head->size] = l;
    head->size++;
}
void concatenate_string(char *s, int j, char *s1)
{
    int i;
    for (i = 0; s1[i] != '\0'; i++)
    {
        s[i + j] = s1[i];
    }
    s[i + j] = '\0';
    return;
}

int isAlpha(char c)
{
    if (c >= 65 && c <= 90)
        return 1;
    else if (c >= 97 && c <= 122)
        return 1;
    else
        return 0;
}

void init_lexer_vars()
{
    p = 1;
    preend = -1;
    readSize;
    merge = 0;
    Line_No = 1;
}

int getStream(FILE *fp)
{
    if (fp == NULL)
    {
        printf("Error /n");
    }
    if (p == 1)
    {
        readSize = fread(DoubleBuffer[0], sizeof(char), BUFFERSIZE, fp);
        // printf("Buffer 1 filled \n");
        p = 0;
    }
    else
    {
        readSize = fread(DoubleBuffer[1], sizeof(char), BUFFERSIZE, fp);
        // printf("Buffer 2 filled \n");
        p = 1;
    }
}

char read(int *end, FILE *fp)
{
    if (readSize <= 1)
    {
        readSize--;
        return '$';
    }

    if (readSize < BUFFERSIZE)
    {
        // readSize--;
        if (preend == -1)
        {
            preend = *end;
            readSize--;
        }
        if (preend != *end)
        {
            readSize--;
            preend = *end;
        }
    }
    if (*end >= BUFFERSIZE)
    {
        *end = 0;
        getStream(fp);
        merge = 1;
    }

    // if(DoubleBuffer[p][*end] == '\n')
    // Line_No++;
    // printf(" {%d , %c} \n",readSize, DoubleBuffer[p][(*end)]);
    return DoubleBuffer[p][(*end)];
}

char *ret_lexeme_error(int beg, int end)
{
    if (merge == 1 && beg <= end)
        merge = 0;
    //  if(Line_No>14) exit(0);
    if (merge == 0)
    {
        char *ans = (char *)malloc(sizeof(char) * (end - beg + 2));
        strncpy(ans, DoubleBuffer[p] + beg, end - beg + 1);
        ans[end - beg + 1] = '\0';
        // printf("%s --- %d \n", ans, Line_No);
        return ans;
    }
    else
    {
        // printf("issue %d && %d",beg,end);
        char *ans1 = (char *)malloc(sizeof(char) * (BUFFERSIZE - beg + 1));
        char *ans = (char *)malloc(sizeof(char) * (BUFFERSIZE - beg + end + 2));
        char *ans2 = (char *)malloc(sizeof(char) * (end + 2));
        strncpy(ans1, DoubleBuffer[1 - p] + beg, BUFFERSIZE - beg);
        strncpy(ans2, DoubleBuffer[p], end + 1);
        ans2[end + 1] = '\0';
        ans1[BUFFERSIZE - beg] = '\0';
        // char* ans = (char*)malloc(sizeof(char)*(BUFFERSIZE -beg+end+2));
        strncpy(ans, ans1, BUFFERSIZE - beg);
        concatenate_string(ans, BUFFERSIZE - beg, ans2);
        // printf("%s --- %d \n", ans, Line_No);
        merge = 0;
        // printf("buffer issues end");
        return ans;
    }
}

char *ret_lexeme(int beg, int end)
{
    if (merge == 1 && beg <= end)
        merge = 0;
    //  if(Line_No>14) exit(0);
    if (merge == 0)
    {
        char *ans = (char *)malloc(sizeof(char) * (end - beg + 2));
        strncpy(ans, DoubleBuffer[p] + beg, end - beg + 1);
        ans[end - beg + 1] = '\0';
        printf("%s      ---      %d \n", ans, Line_No);
        return ans;
    }
    else
    {
        // printf("issue %d && %d",beg,end);
        char *ans1 = (char *)malloc(sizeof(char) * (BUFFERSIZE - beg + 1));
        char *ans = (char *)malloc(sizeof(char) * (BUFFERSIZE - beg + end + 2));
        char *ans2 = (char *)malloc(sizeof(char) * (end + 2));
        strncpy(ans1, DoubleBuffer[1 - p] + beg, BUFFERSIZE - beg);
        strncpy(ans2, DoubleBuffer[p], end + 1);
        ans2[end + 1] = '\0';
        ans1[BUFFERSIZE - beg] = '\0';
        // char* ans = (char*)malloc(sizeof(char)*(BUFFERSIZE -beg+end+2));
        strncpy(ans, ans1, BUFFERSIZE - beg);
        concatenate_string(ans, BUFFERSIZE - beg, ans2);
        printf("%s      ---      %d \n", ans, Line_No);
        merge = 0;
        // printf("buffer issues end");
        return ans;
    }
}

void print(FILE *fp, symTable *map, lex_header *lex_list)
{
    int plag = 0;
    int beg = 0;
    int end = 0;
    int t = 1;
    while (t)
    {
        // if(end>=BUFFERSIZE) end=0
        if (plag == 1)
        {
            int flag = 0;
            // if(merge==1) flag=1;
            char *t = ret_lexeme_error(beg, end - 1);
            // merge=1;
            lexeme *lex = new_lex(t, "TK_ERROR", Line_No);
            add_lex(lex_list, lex);
            plag = 0;
        }
        beg = end;
        char inp = read(&end, fp);
        if (isdigit(inp))
        {
            inp = '1';
        }
        else if (isAlpha(inp))
        {
            if (inp == 'b' || inp == 'c' || inp == 'd')
                inp = '2';
            else
                inp = '3';
        }
        switch (inp)
        {
        case '!':
            end++;
            if (read(&end, fp) == '=')
            {
                char *u = "TK_NE";
                printf("TK_NE ");
                char *string = ret_lexeme(beg, end);
                lexeme *lex = new_lex(string, u, Line_No);
                add_lex(lex_list, lex);
                end++;
            }
            else
            {
                printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                plag = 1;
            }

            break;
        case '=':
            end++;
            if (read(&end, fp) == '=')
            {
                printf("TK_EQ ");
                char *u = "TK_EQ";
                char *string = ret_lexeme(beg, end);
                lexeme *lex = new_lex(string, u, Line_No);
                add_lex(lex_list, lex);
                end++;
            }
            else
            {
                printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                plag = 1;
            }
            break;
        case '>':
            end++;
            if (read(&end, fp) == '=')
            {
                char *u = "TK_GE";
                printf("TK_GE ");
                char *string = ret_lexeme(beg, end);
                lexeme *lex = new_lex(string, u, Line_No);
                add_lex(lex_list, lex);
                end++;
            }
            else
            {
                char *u = "TK_GT";
                printf("TK_GT ");
                char *string = ret_lexeme(beg, end);
                lexeme *lex = new_lex(string, u, Line_No);
                add_lex(lex_list, lex);
                // printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg,end-1), Line_No);
                // plag=1;
            }
            break;
        case '@':
            end++;
            if (read(&end, fp) == '@')
            {
                end++;
                if (read(&end, fp) == '@')
                {
                    char *u = "TK_OR";
                    printf("TK_OR ");
                    char *string = ret_lexeme(beg, end);
                    lexeme *lex = new_lex(string, u, Line_No);
                    add_lex(lex_list, lex);
                    end++;
                }
                else
                {
                    printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                    plag = 1;
                }
            }
            else
            {
                printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                plag = 1;
            }
            break;

        case '&':
            end++;
            if (read(&end, fp) == '&')
            {
                end++;
                if (read(&end, fp) == '&')
                {
                    char *u = "TK_AND";
                    printf("TK_AND ");
                    char *string = ret_lexeme(beg, end);
                    lexeme *lex = new_lex(string, u, Line_No);
                    add_lex(lex_list, lex);
                    end++;
                }
                else
                {
                    printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                    plag = 1;
                }
            }
            else
            {
                printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                plag = 1;
            }
            break;

        case '*':;
            char *u = "TK_MUL";
            printf("TK_MUL ");
            char *string = ret_lexeme(beg, end);
            lexeme *lex = new_lex(string, u, Line_No);
            add_lex(lex_list, lex);
            end++;
            break;

        case '+':
            printf("TK_PLUS ");
            char *u1 = "TK_PLUS";
            char *string1 = ret_lexeme(beg, end);
            lexeme *lex1 = new_lex(string1, u1, Line_No);
            add_lex(lex_list, lex1);
            end++;
            break;

        case '~':
            printf("TK_NOT ");
            char *u2 = "TK_NOT";
            char *string2 = ret_lexeme(beg, end);
            lexeme *lex2 = new_lex(string2, u2, Line_No);
            add_lex(lex_list, lex2);
            end++;
            break;

        case '/':
            printf("TK_DIV ");
            char *u3 = "TK_DIV";
            char *string3 = ret_lexeme(beg, end);
            lexeme *lex3 = new_lex(string3, u3, Line_No);
            add_lex(lex_list, lex3);
            end++;
            break;

        case '.':
            printf("TK_DOT ");
            char *u4 = "TK_DOT";
            char *string4 = ret_lexeme(beg, end);
            lexeme *lex4 = new_lex(string4, u4, Line_No);
            add_lex(lex_list, lex4);
            end++;
            break;

        case '-':
            printf("TK_MINUS ");
            char *u5 = "TK_MINUS";
            char *string5 = ret_lexeme(beg, end);
            lexeme *lex5 = new_lex(string5, u5, Line_No);
            add_lex(lex_list, lex5);
            end++;
            break;

        case ':':
            printf("TK_COLON ");
            char *u6 = "TK_COLON";
            char *string6 = ret_lexeme(beg, end);
            lexeme *lex6 = new_lex(string6, u6, Line_No);
            add_lex(lex_list, lex6);
            end++;
            break;

        case ';':
            printf("TK_SEM ");
            char *u7 = "TK_SEM";
            char *string7 = ret_lexeme(beg, end);
            lexeme *lex7 = new_lex(string7, u7, Line_No);
            add_lex(lex_list, lex7);
            end++;
            break;

        case ',':
            printf("TK_COMMA ");
            char *u8 = "TK_COMMA";
            char *string8 = ret_lexeme(beg, end);
            lexeme *lex8 = new_lex(string8, u8, Line_No);
            add_lex(lex_list, lex8);
            end++;
            break;

        case ')':;
            printf("TK_CL ");
            char *u9 = "TK_CL";
            char *string9 = ret_lexeme(beg, end);
            lexeme *lex9 = new_lex(string9, u9, Line_No);
            add_lex(lex_list, lex9);
            end++;
            break;

        case '(':
            printf("TK_OP ");
            char *u10 = "TK_OP";
            char *string10 = ret_lexeme(beg, end);
            lexeme *lex10 = new_lex(string10, u10, Line_No);
            add_lex(lex_list, lex10);
            end++;
            break;

        case ']':;
            printf("TK_SQR ");
            char *u11 = "TK_SQR";
            char *string11 = ret_lexeme(beg, end);
            lexeme *lex11 = new_lex(string11, u11, Line_No);
            add_lex(lex_list, lex11);
            end++;
            break;

        case '[':
            printf("TK_SQL ");
            char *u12 = "TK_SQL";
            char *string12 = ret_lexeme(beg, end);
            lexeme *lex12 = new_lex(string12, u12, Line_No);
            add_lex(lex_list, lex12);
            end++;
            break;

        case '%':
            printf("TK_COMMENT ");
            while (read(&end, fp) != '\n')
                end++;
            char *u13 = "TK_COMMENT";
            char *string13 = ret_lexeme(beg, end - 1);
            // lexeme* lex13=new_lex(string13,u13,Line_No);
            // add_lex(lex_list,lex13);

            break;

        case '#':
            end++;
            if (isAlpha(read(&end, fp)))
            {
                end++;
                while (isAlpha(read(&end, fp)))
                    end++;
                char *u = "TK_RUID";
                printf("TK_RUID ");
                char *string = ret_lexeme(beg, end - 1);
                lexeme *lex = new_lex(string, u, Line_No);
                add_lex(lex_list, lex);
                insertST(string, u, map);
                // ret_lexeme(beg,end - 1);
            }
            else
            {
                printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                plag = 1;
            }
            break;

        case '<':
            end++;
            if (read(&end, fp) == '=')
            {
                char *u = "TK_LE";
                printf("TK_LE ");
                char *string = ret_lexeme(beg, end);
                lexeme *lex = new_lex(string, u, Line_No);
                add_lex(lex_list, lex);
                end++;
            }
            else if (read(&end, fp) == '-')
            {
                end++;
                if (read(&end, fp) == '-')
                {
                    end++;
                    if (read(&end, fp) == '-')
                    {
                        char *u = "TK_ASSIGNOP";
                        printf("TK_ASSIGNOP ");
                        char *string = ret_lexeme(beg, end);
                        lexeme *lex = new_lex(string, u, Line_No);
                        add_lex(lex_list, lex);
                        end++;
                    }
                    else
                    {
                        printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                        plag = 1;
                    }
                }
                else
                {
                    printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                    plag = 1;
                }
            }
            else
            {
                printf("TK_LT ");
                char *u = "TK_LT";
                char *string = ret_lexeme(beg, end - 1);
                lexeme *lex = new_lex(string, u, Line_No);
                add_lex(lex_list, lex);
                // end++;
            }
            break;

        case '_':
            end++;
            if (isAlpha(read(&end, fp)))
            {
                end++;
                while (isAlpha(read(&end, fp)))
                    end++;
                while (isdigit(read(&end, fp)))
                    end++;
                int flag = 0;
                if (merge == 1)
                    flag = 1;
                if (strlen(ret_lexeme_error(beg, end - 1)) > 30)
                    printf("Error: The length of function id lexeme given is greater than 30 --- %d\n", Line_No);
                else
                {
                    // printf("TK_FUNID ");
                    // char* u="TK_FUNID";
                    if (flag == 1)
                        merge = 1;
                    char *string = ret_lexeme_error(beg, end - 1);
                    char *output = lookupST(string, map);
                    if (output == NULL || strcmp(output, "TK_FUNID") == 0)
                    {
                        char *t = "TK_FUNID";
                        lexeme *lex = new_lex(string, t, Line_No);
                        add_lex(lex_list, lex);
                        insertST(string, t, map);
                        printf("TK_FUNID ");
                    }
                    else
                    {
                        lexeme *lex = new_lex(string, output, Line_No);
                        add_lex(lex_list, lex);
                        printf("%s ", output);
                    }
                    merge = flag;
                    // char* string=ret_lexeme(beg,end-1);
                    // lexeme* lex=new_lex(string,u,Line_No);
                    // add_lex(lex_list,lex);
                    // insertST(string,u,map);
                    ret_lexeme(beg, end - 1);
                }
            }
            else
            {
                printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                plag = 1;
            }
            break;

        case '1':
            end++;
            while (isdigit(read(&end, fp)))
                end++;
            if (read(&end, fp) == '.')
            {
                end++;
                if (isdigit(read(&end, fp)))
                {
                    end++;
                    if (isdigit(read(&end, fp)))
                    {
                        end++;
                        if (read(&end, fp) == 'E')
                        {
                            end++;
                            if (read(&end, fp) == '+' || read(&end, fp) == '-')
                            {
                                end++;
                                if (isdigit(read(&end, fp)))
                                {
                                    end++;
                                    if (isdigit(read(&end, fp)))
                                    {
                                        char *t = "TK_RNUM";
                                        printf("TK_RNUM ");
                                        char *string = ret_lexeme(beg, end);
                                        lexeme *lex = new_lex(string, t, Line_No);
                                        add_lex(lex_list, lex);
                                        end++;
                                    }
                                    else
                                    {
                                        printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                                        plag = 1;
                                    }
                                }
                            }
                            else if (isdigit(read(&end, fp)))
                            {
                                end++;
                                if (isdigit(read(&end, fp)))
                                {
                                    char *t = "TK_RNUM";
                                    printf("TK_RNUM ");
                                    char *string = ret_lexeme(beg, end);
                                    lexeme *lex = new_lex(string, t, Line_No);
                                    add_lex(lex_list, lex);
                                    end++;
                                }
                                else
                                {
                                    printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                                    plag = 1;
                                }
                            }
                            else
                            {
                                printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                                plag = 1;
                            }
                        }
                        else
                        {
                            char *t = "TK_RNUM";
                            printf("TK_RNUM ");
                            char *string = ret_lexeme(beg, end - 1);
                            lexeme *lex = new_lex(string, t, Line_No);
                            add_lex(lex_list, lex);
                        }
                    }
                    else
                    {
                        printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                        plag = 1;
                    }
                }
                else
                {
                    printf("error  unrecognized pattern <%s> --- %d\n", ret_lexeme_error(beg, end - 1), Line_No);
                    plag = 1;
                }
            }
            else
            {
                printf("TK_NUM ");
                char *t = "TK_NUM";
                char *string = ret_lexeme(beg, end - 1);
                lexeme *lex = new_lex(string, t, Line_No);
                add_lex(lex_list, lex);
            }

            break;

        case '2':
            end++;
            char c = read(&end, fp);
            //  printf("%c",c);
            if (isAlpha(c))
            {
                end++;
                // printf("here");
                while (isAlpha(read(&end, fp)))
                    end++;
                // if(strlen(ret_lexeme_error(beg,end-1))>20) printf("Error: The length of variable id lexeme given is greater than 20\n");
                // else
                // {
                // printf("lookupST ");
                int flag = 0;
                if (merge == 1)
                    flag = 1;
                char *string = ret_lexeme_error(beg, end - 1);
                char *output = lookupST(string, map);
                if (output == NULL || strcmp(output, "TK_FIELDID") == 0)
                {
                    char *t = "TK_FIELDID";
                    lexeme *lex = new_lex(string, t, Line_No);
                    add_lex(lex_list, lex);
                    insertST(string, t, map);
                    printf("TK_FIELDID ");
                }
                else
                {
                    lexeme *lex = new_lex(string, output, Line_No);
                    add_lex(lex_list, lex);
                    printf("%s ", output);
                }
                merge = flag;
                ret_lexeme(beg, end - 1);
                // }
            }
            else if (read(&end, fp) <= 55 && read(&end, fp) >= 50)
            {
                // printf("lalu");
                end++;
                while (read(&end, fp) <= 100 && read(&end, fp) >= 98)
                    end++;
                if (read(&end, fp) <= 55 && read(&end, fp) >= 50)
                {
                    end++;
                    while (read(&end, fp) <= 55 && read(&end, fp) >= 50)
                        end++;
                    int flag = 0;
                    if (merge == 1)
                        flag = 1;
                    if (strlen(ret_lexeme_error(beg, end - 1)) > 20)
                        printf("Error: The length of variable id lexeme given is greater than 20\n");
                    else
                    {
                        printf("TK_ID ");
                        char *t = "TK_ID";
                        merge = flag;
                        char *string = ret_lexeme(beg, end - 1);
                        lexeme *lex = new_lex(string, t, Line_No);
                        add_lex(lex_list, lex);
                        insertST(string, t, map);
                    }
                }
                else
                {
                    int flag = 0;
                    if (merge == 1)
                        flag = 1;
                    if (strlen(ret_lexeme_error(beg, end - 1)) > 20)
                        printf("Error: The length of variable id lexeme given is greater than 20\n");
                    else
                    {
                        printf("TK_ID ");
                        char *t = "TK_ID";
                        merge = flag;
                        char *string = ret_lexeme(beg, end - 1);
                        lexeme *lex = new_lex(string, t, Line_No);
                        add_lex(lex_list, lex);
                        insertST(string, t, map);
                    }
                }
            }
            break;

        case '3':
            end++;
            if (isalpha(read(&end, fp)))
            {
                end++;
                while (isalpha(read(&end, fp)))
                    end++;
                // if(strlen(ret_lexeme_error(beg,end-1))>20) printf("Error:   The length of variable id lexeme given is greater than 20\n");
                // printf("lookupST ");
                int flag = 0;
                if (merge == 1)
                    flag = 1;
                char *string = ret_lexeme_error(beg, end - 1);
                char *output = lookupST(string, map);
                if (output == NULL || (strcmp(output, "TK_FIELDID") == 0))
                {
                    char *t = "TK_FIELDID";
                    insertST(string, t, map);
                    printf("TK_FIELDID ");
                    lexeme *lex = new_lex(string, t, Line_No);
                    add_lex(lex_list, lex);
                }
                else
                {
                    lexeme *lex = new_lex(string, output, Line_No);
                    add_lex(lex_list, lex);
                    printf("%s ", output);
                }
                merge = flag;
                ret_lexeme(beg, end - 1);
            }
            else
            {
                printf("TK_FIELDID ");
                char *string = ret_lexeme(beg, end - 1);
                lexeme *lex = new_lex(string, "TK_FIELDID", Line_No);
                add_lex(lex_list, lex);
                // ret_lexeme(beg,end-1);
            }
            break;

        case '\n':
            Line_No++;
            end++;
            break;

        case '\r':
            end++;
            break;

        case ' ':
            end++;
            break;

        case '$':
            if (readSize <= 0)
                t = 0;
            else
            {
                printf("Error Unrecognized Symbol <$> --- %d\n", Line_No);
            }
            end++;
            break;

        case '\t':
            end++;
            break;

            // case '|':
            // end++;
            // break;

        default:
            printf("Error Unrecognized Symbol <%s> --- %d \n", ret_lexeme_error(beg, end), Line_No);
            // ret_lexeme(beg,end);
            end++;
            break;
        }
    }
    init_lexer_vars();
    rewind(fp);
}

void removeComments(FILE *fp)
{
    char c = fgetc(fp);
    while (c != EOF)
    {
        if (c == '%')
        {
            while (c != '\n' && c != EOF)
            {
                c = fgetc(fp);
            }
        }
        if (c == EOF)
            break;
        printf("%c", c);
        c = fgetc(fp);
    }
    printf("\n");
    rewind(fp);
}