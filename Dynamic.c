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
#include "hashmap.c"
#define listsize 1000

char LIST_terminals[10000][10000];
char LIST_nonterminals[10000][10000];
int i_t = 0;
int i_nt = 0;

struct lexeme
{
	char *lexeme;
};

// struct list_of_lexemes
// {
// struct lexeme LEX;
// struct list_of_lexemes* NEXT;
// };

struct FIRST_SET
{
	struct list_of_lexemes **LIST;
};

struct FIRST_SET *create_FIRST_SET()
{
	struct FIRST_SET *FS = (struct FIRST_SET *)(malloc((sizeof(struct FIRST_SET))));
	FS->LIST = (struct list_of_lexemes **)(malloc(sizeof(struct list_of_lexemes *) * i_nt));
	return FS;
}

struct grammar_rule_node
{
	int is_Parsed;
	int is_ParsedFollow;
	int from_which;
	struct lexeme *lex;
	struct grammar_rule_node *next;
};

typedef struct grammar_rule_node grammar;

struct lexeme *create_lexeme(char *s)
{
	while (s[strlen(s) - 1] == '\n' || s[strlen(s) - 1] == '\r')
		s[strlen(s) - 1] = '\0';
	struct lexeme *lex = (struct lexeme *)malloc(sizeof(struct lexeme));
	lex->lexeme = (char *)malloc(sizeof(char) * 100);
	memset(lex->lexeme, '\0', 100);
	strcpy(lex->lexeme, s);
	return lex;
}

struct grammar_rule_node *create_grammar_rule_node()
{
	struct grammar_rule_node *p = (struct grammar_rule_node *)malloc(sizeof(struct grammar_rule_node));
	p->is_Parsed = -1;
	p->is_ParsedFollow = -1;
	p->next = NULL;
	return p;
}

struct grammar_rule_node *create_grammar_rule(char *s, hashmap *hash_nterminals, hashmap *hash_terminals)
{

	struct grammar_rule_node *GR = create_grammar_rule_node();
	char *token = strtok(s, " ");
	if (strcmp(s, "<expPrime>") == 0)
	{
		printf(" ");
	}

	struct lexeme *ll = create_lexeme(token);
	GR->lex = ll;

	if (token[0] == '<' && token[strlen(token) - 1] == '>' && (lookup(token, hash_nterminals) == -1))
	{
		insert(token, i_nt, hash_nterminals);
		// printf("%ld, %d , %d , %d \n", strlen(token), token[0] == '<' , token[strlen(token)-1] == '>', (lookup(token, hash_nterminals) == -1));
		strcpy(LIST_nonterminals[i_nt++], token);
		// printf("Nonterminal == %s \n", token);
	}
	else if ((token[0] != '<' || token[strlen(token) - 1] != '>') && lookup(token, hash_terminals) == -1)
	{
		insert(token, i_t, hash_terminals);
		strcpy(LIST_terminals[i_t++], token);
		// printf("%ld, %d , %d , %d \n", strlen(token), token[0] == '<' , token[strlen(token)-1] == '>', (lookup(token, hash_nterminals) == -1));
		// printf("Terminal == %s \n", token);
	}

	token = strtok(NULL, " ");
	struct grammar_rule_node *temp = GR;
	while (token != NULL && strcmp("\r", token) != 0)
	{
		if (strcmp(token, "===>") == 0)
		{
			token = strtok(NULL, " ");
			continue;
		}
		if (token[strlen(token) - 1] == '\n')
			token[strlen(token) - 1] = '\0';
		struct grammar_rule_node *G = create_grammar_rule_node();
		struct lexeme *l = create_lexeme(token);
		G->lex = l;
		temp->next = G;
		temp = G;
		if (token[0] == '<' && token[strlen(token) - 1] == '>' && (lookup(token, hash_nterminals) == -1))
		{
			insert(token, i_nt, hash_nterminals);
			// printf("%ld, %d , %d , %d \n", strlen(token), token[0] == '<' , token[strlen(token)-1] == '>', (lookup(token, hash_nterminals) == -1));
			strcpy(LIST_nonterminals[i_nt++], token);
			// printf("Nonterminal == %s \n", token);
		}
		else if ((token[0] != '<' || token[strlen(token) - 1] != '>') && lookup(token, hash_terminals) == -1)
		{
			insert(token, i_t, hash_terminals);
			strcpy(LIST_terminals[i_t++], token);
			// printf("%ld, %d , %d , %d \n", strlen(token), token[0] == '<' , token[strlen(token)-1] == '>', (lookup(token, hash_nterminals) == -1));
			// printf("Terminal == %s \n", token);
		}

		token = strtok(NULL, " ");
	}
	return GR;
}

void print_grammar_rule(struct grammar_rule_node *rule)
{

	struct grammar_rule_node *GR = rule;
	while (GR != NULL)
	{
		printf("%s ", GR->lex->lexeme);
		GR = GR->next;
	}
	printf("\n");
}
struct GRAMMAR
{
	struct grammar_rule_node **LIST;
	hashmap *hash_nterminals, *hash_terminals;
	int size;
	int *isEphsilon;
};

struct GRAMMAR *create_grammar(FILE *F)
{
	struct GRAMMAR *G = (struct GRAMMAR *)malloc(sizeof(struct GRAMMAR));
	G->LIST = (struct grammar_rule_node **)malloc(sizeof(struct grammar_rule_node *) * listsize);
	G->hash_terminals = inithashmap();
	G->hash_nterminals = inithashmap();

	int index = 0;
	char str[1000];
	while (fgets(str, 1000, F) != NULL)
	{
		if (index == 62)
		{
			printf(" ");
		}
		G->LIST[index] = create_grammar_rule(str, G->hash_nterminals, G->hash_terminals);
		index++;
	}
	G->size = index;

	return G;
}

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
	containerNode **array = (containerNode **)malloc(sizeof(containerNode *) * non_terminals);
	for (int i = 0; i < non_terminals; i++)
		array[i] = NULL;
	int num = g->size;
	for (int i = 0; i < num; i++)
	{
		grammar *head = g->LIST[i];
		head = head->next;
		while (head != NULL)
		{
			if (lookup(head->lex->lexeme, g->hash_nterminals) != -1)
			{
				// printf("yahan pahuncho\n");
				containerNode *node = fillNode(i);
				node->next = array[lookup(head->lex->lexeme, g->hash_nterminals)];
				array[lookup(head->lex->lexeme, g->hash_nterminals)] = node;
			}
			head = head->next;
		}
	}
	return array;
}

containerNode **workOnGramFirst(struct GRAMMAR *g, int non_terminals)
{
	containerNode **array = (containerNode **)malloc(sizeof(containerNode *) * non_terminals);
	for (int i = 0; i < non_terminals; i++)
		array[i] = NULL;
	int num = g->size;
	for (int i = 0; i < num; i++)
	{
		grammar *head = g->LIST[i];
		if (lookup(head->lex->lexeme, g->hash_nterminals) != -1)
		{
			containerNode *node = fillNode(i);
			node->next = array[lookup(head->lex->lexeme, g->hash_nterminals)];
			array[lookup(head->lex->lexeme, g->hash_nterminals)] = node;
		}
	}
	return array;
}

struct GRAMMAR *populate_FS(struct GRAMMAR *GR, containerNode **C)
{

	GR->isEphsilon = (int *)malloc(sizeof(int) * i_nt);
	for (int i = 0; i < i_nt; i++)
	{
		GR->isEphsilon[i] = 0;
	}
	for (int i = 0; i < GR->size; i++)
	{
		if (strcmp(GR->LIST[i]->next->lex->lexeme, "eps") == 0)
			GR->isEphsilon[lookup(GR->LIST[i]->lex->lexeme, GR->hash_nterminals)] = 1;
	}
	// for(int i=0; i<i_nt; i++)
	// {
	// 	printf("%d ", GR->isEphsilon[i]);
	// }
	// printf("\n");
	struct GRAMMAR *FS = (struct GRAMMAR *)malloc(sizeof(struct GRAMMAR));

	FS->LIST = (struct grammar_rule_node **)malloc(sizeof(struct grammar_rule_node *) * i_nt);

	for (int i = 0; i < i_nt; i++)
	{
		FS->LIST[i] = NULL;
	}

	int finished[i_nt];
	for (int i = 0; i < i_nt; i++)
		finished[i] = 0;
	int FLAG = 30000;
	int fag = 10000;
	while (fag--)
	{
		FLAG = 0;
		for (int i = 0; i < i_nt; i++)
		{
			if (C[i] != NULL)
			{
				containerNode *temp = C[i];
				containerNode *prev = NULL;
				int j = 0;
				while (temp != NULL)
				{
					// printf("Infinite LOOP ");
					struct grammar_rule_node *gnode = GR->LIST[temp->index];
					gnode = gnode->next;
					while (gnode != NULL && gnode->is_Parsed == 1)
					{
						gnode = gnode->next;
					}
					if (gnode == NULL)
					{
						struct lexeme *l = create_lexeme("$");
						struct grammar_rule_node *Temp = create_grammar_rule_node();
						Temp->from_which = temp->index;
						Temp->lex = l;
						if (FS->LIST[i] == NULL)
							FS->LIST[i] = Temp;
						else
						{
							struct grammar_rule_node *NF = FS->LIST[i];
							while (NF->next != NULL)
								NF = NF->next;
							NF->next = Temp;
							// Temp->next = FS->LIST[i];
							// FS->LIST[i] = Temp;
						}
						if (prev == NULL)
						{
							C[i] = temp->next;
						}
						else
						{
							prev->next = temp->next;
							prev = temp;
						}
					}
					else
					{
						// printf("__%s__ \n", gnode->lex->lexeme);
						if (lookup(gnode->lex->lexeme, GR->hash_nterminals) == -1)
						{

							struct lexeme *l = create_lexeme(gnode->lex->lexeme);
							struct grammar_rule_node *Temp = create_grammar_rule_node();
							Temp->lex = l;
							Temp->from_which = temp->index;
							if (FS->LIST[i] == NULL)
								FS->LIST[i] = Temp;
							else
							{
								struct grammar_rule_node *NF = FS->LIST[i];
								while (NF->next != NULL)
									NF = NF->next;
								NF->next = Temp;

								// Temp->next = FS->LIST[i];
								// FS->LIST[i] = Temp;
							}
							if (prev == NULL)
							{
								C[i] = temp->next;
							}
							else
							{
								prev->next = temp->next;
								prev = temp;
							}
							temp = temp->next;
							continue;
						}
						else if (C[lookup(gnode->lex->lexeme, GR->hash_nterminals)] == NULL && GR->isEphsilon[lookup(gnode->lex->lexeme, GR->hash_nterminals)] == 1)
						{

							struct grammar_rule_node *temp1 = FS->LIST[lookup(gnode->lex->lexeme, GR->hash_nterminals)];

							while (temp1 != NULL)
							{
								struct lexeme *l = create_lexeme(temp1->lex->lexeme);
								struct grammar_rule_node *Temp = create_grammar_rule_node();
								Temp->from_which = temp->index;
								Temp->lex = l;
								if (FS->LIST[i] == NULL)
									FS->LIST[i] = Temp;
								else
								{
									struct grammar_rule_node *NF = FS->LIST[i];
									while (NF->next != NULL)
										NF = NF->next;
									// NF->next = SET;
									NF->next = Temp;
									// Temp->next = FS->LIST[i];
									// FS->LIST[i] = Temp;
								}
								temp1 = temp1->next;
							}
							if (GR->isEphsilon[lookup(gnode->lex->lexeme, GR->hash_nterminals)] == 0)
							{
								if (prev == NULL)
								{
									C[i] = temp->next;
								}
								else
								{
									prev->next = temp->next;
									prev = temp;
								}
							}
							else
							{
								gnode->is_Parsed = 1;
							}
							continue;
						}
						else if (C[lookup(gnode->lex->lexeme, GR->hash_nterminals)] == NULL && GR->isEphsilon[lookup(gnode->lex->lexeme, GR->hash_nterminals)] == 0)
						{
							struct grammar_rule_node *temp1 = FS->LIST[lookup(gnode->lex->lexeme, GR->hash_nterminals)];

							while (temp1 != NULL)
							{
								struct lexeme *l = create_lexeme(temp1->lex->lexeme);
								struct grammar_rule_node *Temp = create_grammar_rule_node();
								Temp->lex = l;
								if (FS->LIST[i] == NULL)
									FS->LIST[i] = Temp;
								else
								{
									struct grammar_rule_node *NF = FS->LIST[i];
									while (NF->next != NULL)
										NF = NF->next;
									// NF->next = SET;
									NF->next = Temp;
									// Temp->next = FS->LIST[i];
									// FS->LIST[i] = Temp;
								}
								temp1 = temp1->next;
							}
							if (GR->isEphsilon[lookup(gnode->lex->lexeme, GR->hash_nterminals)] == 0)
							{
								if (prev == NULL)
								{
									C[i] = temp->next;
								}
								else
								{
									prev->next = temp->next;
									prev = temp;
								}
							}
						}
						else
							break;
					}
					temp = temp->next;
				}
				if (C[i] != NULL)
					FLAG = 1;
			}
		}
	}
	return FS;
}

struct GRAMMAR *Populate_FOLLOW(struct GRAMMAR *GR, containerNode **C, struct GRAMMAR *FIRSTSET)
{

	GR->isEphsilon = (int *)malloc(sizeof(int) * i_nt);
	for (int i = 0; i < i_nt; i++)
	{
		GR->isEphsilon[i] = 0;
	}
	for (int i = 0; i < GR->size; i++)
	{
		if (strcmp(GR->LIST[i]->next->lex->lexeme, "eps") == 0)
			GR->isEphsilon[lookup(GR->LIST[i]->lex->lexeme, GR->hash_nterminals)] = 1;
	}

	struct GRAMMAR *FS = (struct GRAMMAR *)malloc(sizeof(struct GRAMMAR));

	FS->LIST = (struct grammar_rule_node **)malloc(sizeof(struct grammar_rule_node *) * i_nt);

	for (int i = 0; i < i_nt; i++)
	{
		FS->LIST[i] = NULL;
	}

	struct grammar_rule_node *g = create_grammar_rule_node();
	struct lexeme *Q = create_lexeme("$");
	g->lex = Q;
	FS->LIST[0] = g;

	// int finished[i_nt];
	// for(int i=0; i<i_nt; i++)
	// finished[i] = 0;
	int FLAG = 1;
	while (FLAG)
	{
		FLAG = 0;

		int diditchange = 0;
		for (int i = 0; i < i_nt; i++)
		{
			if (C[i] != NULL)
			{
				containerNode *temp = C[i];
				containerNode *prev = NULL;
				int j = 0;
				while (temp != NULL)
				{
					// printf("Infinite LOOP ");
					struct grammar_rule_node *gnode = GR->LIST[temp->index];
					gnode = gnode->next;
					while (strcmp(gnode->lex->lexeme, LIST_nonterminals[i]) != 0)
					{
						gnode = gnode->next;
					}
					gnode = gnode->next;

					while (gnode != NULL && gnode->is_ParsedFollow == 1)
					{
						gnode = gnode->next;
					}

					if (gnode != NULL)
					{
						if (gnode != NULL && lookup(gnode->lex->lexeme, GR->hash_nterminals) != -1 && GR->isEphsilon[lookup(gnode->lex->lexeme, GR->hash_nterminals)] == 1)
						{
							while (gnode != NULL && lookup(gnode->lex->lexeme, GR->hash_nterminals) != -1 && GR->isEphsilon[lookup(gnode->lex->lexeme, GR->hash_nterminals)] == 1)
							{

								struct grammar_rule_node *G = FIRSTSET->LIST[lookup(gnode->lex->lexeme, GR->hash_nterminals)];
								struct grammar_rule_node *SET = create_grammar_rule_node();
								struct grammar_rule_node *Prev;
								struct lexeme *l = create_lexeme(G->lex->lexeme);
								SET->lex = l;
								Prev = SET;
								G = G->next;
								while (G != NULL)
								{
									struct grammar_rule_node *SET1 = create_grammar_rule_node();
									struct lexeme *l1 = create_lexeme(G->lex->lexeme);
									SET1->lex = l1;
									Prev->next = SET1;
									Prev = SET1;
									G = G->next;
								}

								if (FS->LIST[i] == NULL)
								{
									FS->LIST[i] = SET;
								}
								else
								{
									struct grammar_rule_node *NF = FS->LIST[i];
									while (NF->next != NULL)
										NF = NF->next;
									NF->next = SET;
								}
								gnode->is_ParsedFollow = 1;
								gnode = gnode->next;
								diditchange = 1;
							}
							prev = temp;
							temp = temp->next;
							continue;
						}
						else if (gnode != NULL && lookup(gnode->lex->lexeme, GR->hash_terminals) != -1)
						{
							struct grammar_rule_node *SET = create_grammar_rule_node();
							struct lexeme *l = create_lexeme(gnode->lex->lexeme);
							SET->lex = l;
							if (FS->LIST[i] == NULL)
							{
								FS->LIST[i] = SET;
							}
							else
							{
								struct grammar_rule_node *NF = FS->LIST[i];
								while (NF->next != NULL)
									NF = NF->next;
								NF->next = SET;

								// SET->next = FS->LIST[i];
								// FS->LIST[i] = SET;
							}

							if (prev == NULL)
							{
								C[i] = temp->next;
							}
							else
							{
								prev->next = temp->next;
							}
							temp = temp->next;
							diditchange = 1;
							continue;
						}
						else if (gnode != NULL)
						{
							struct grammar_rule_node *G = FIRSTSET->LIST[lookup(gnode->lex->lexeme, GR->hash_nterminals)];
							struct grammar_rule_node *SET = create_grammar_rule_node(), *Prev;
							struct lexeme *l = create_lexeme(G->lex->lexeme);
							SET->lex = l;
							Prev = SET;
							G = G->next;
							while (G != NULL)
							{
								struct grammar_rule_node *SET1 = create_grammar_rule_node();
								struct lexeme *l1 = create_lexeme(G->lex->lexeme);
								SET1->lex = l1;
								Prev->next = SET1;
								Prev = SET1;
								G = G->next;
							}
							if (FS->LIST[i] == NULL)
							{
								FS->LIST[i] = SET;
							}
							else
							{
								struct grammar_rule_node *NF = FS->LIST[i];
								while (NF->next != NULL)
									NF = NF->next;
								NF->next = SET;
								// SET->next = FS->LIST[i];
								// FS->LIST[i] = SET;
							}

							if (prev == NULL)
							{
								C[i] = temp->next;
							}
							else
							{
								prev->next = temp->next;
							}
							gnode->is_ParsedFollow = 1;
							temp = temp->next;
							diditchange = 1;
							continue;
						}
					}
					else
					{
						if (C[lookup(GR->LIST[temp->index]->lex->lexeme, GR->hash_nterminals)] == NULL)
						{
							// if(gnode != NULL)
							//  gnode->is_ParsedFollow = 1;

							struct grammar_rule_node *G = FS->LIST[lookup(GR->LIST[temp->index]->lex->lexeme, GR->hash_nterminals)];
							struct grammar_rule_node *SET = create_grammar_rule_node();
							struct grammar_rule_node *Prev;
							struct lexeme *l = create_lexeme(G->lex->lexeme);
							SET->lex = l;
							Prev = SET;
							G = G->next;
							while (G != NULL)
							{
								struct grammar_rule_node *SET1 = create_grammar_rule_node();
								struct lexeme *l1 = create_lexeme(G->lex->lexeme);
								SET1->lex = l1;
								Prev->next = SET1;
								Prev = SET1;
								G = G->next;
							}
							struct grammar_rule_node *TEMP = SET;
							// printf("HEHE ");
							// while(TEMP!=NULL)
							// {
							// 	printf("%s ",TEMP->lex->lexeme);
							// 	TEMP = TEMP->next;
							// }
							// printf("\n");

							if (FS->LIST[i] == NULL)
							{
								FS->LIST[i] = SET;
							}
							else
							{
								struct grammar_rule_node *NF = FS->LIST[i];
								while (NF->next != NULL)
									NF = NF->next;
								NF->next = SET;

								// SET->next = FS->LIST[i];
								// FS->LIST[i] = SET;
							}

							if (prev == NULL)
							{
								C[i] = temp->next;
							}
							else
							{
								prev->next = temp->next;
							}
							temp = temp->next;
							diditchange = 1;
							continue;
						}
						else if (strcmp(GR->LIST[temp->index]->lex->lexeme, LIST_nonterminals[i]) == 0)
						{
							if (prev == NULL)
							{
								C[i] = temp->next;
							}
							else
							{
								prev->next = temp->next;
							}
							temp = temp->next;
							diditchange = 1;
							continue;
						}
					}
					prev = temp;
					temp = temp->next;
				}
				if (C[i] != NULL)
					FLAG = 1;
			}
		}
		if (diditchange == 0)
		{
			printf("\n \n Partially Calculated Follow Set: \n");
			FLAG = 0;
		}
	}
	return FS;
}

int main()
{

	FILE *F;
	F = fopen("grammar.txt", "r");
	struct GRAMMAR *G = create_grammar(F);
	containerNode **C = workOnGramFirst(G, i_nt);
	containerNode **D = workOnGram(G, i_nt);
	int size = G->size;
	// printf("%d %d %d \n", size, i_nt, i_t);
	printf("GRAMMAR : \n");
	for (int i = 0; i < size; i++)
	{
		print_grammar_rule(G->LIST[i]);
	}
	printf("\n  \n TERMINALS : \n");
	for (int i = 0; i < i_t; i++)
	{
		printf("%s  %d \n", LIST_terminals[i], i);
	}

	printf("\n  \n NON TERMINALS : \n");
	for (int i = 0; i < i_nt; i++)
	{
		printf("%s  %d \n", LIST_nonterminals[i], i);
	}

	// printf("Bucket for Follow : \n");
	// for(int i=0; i<i_nt; i++)
	// {
	// containerNode* curr = D[i];
	// printf("%d ==> ",i);
	// while(curr != NULL)
	// {
	// printf("%d ", curr->index);
	// curr = curr->next;
	// }
	// printf("\n");
	// }
	// printf("\n");

	// printf("Bucket for First : \n");
	// for(int i=0; i<i_nt; i++)
	// {
	// containerNode* curr = C[i];
	// printf("%d ==> ",i);
	// while(curr != NULL)
	// {
	// printf("%d ", curr->index);
	// curr = curr->next;
	// }
	// printf("\n");
	// }

	struct GRAMMAR *FS = populate_FS(G, C);

	// for(int i=0; i<i_nt; i++)
	// {
	// containerNode* curr = D[i];
	// printf("%d ==> ",i);
	// while(curr != NULL)
	// {
	// printf("%d ", curr->index);
	// curr = curr->next;
	// }
	// printf("\n");
	// }

	printf("FIRST SET: \n");

	for (int i = 0; i < i_nt; i++)
	{
		if (i == 43)
		{
			// printf("hehe");
		}
		printf("%d  ", i);
		if (FS->LIST[i] != NULL)
		{
			struct grammar_rule_node *temp = FS->LIST[i];
			while (temp != NULL)
			{
				printf("[%s - %d] ", temp->lex->lexeme, temp->from_which);
				temp = temp->next;
			}
		}
		printf("\n");
	}

	struct GRAMMAR *FF = Populate_FOLLOW(G, D, FS);

	printf("FOLLOW SET: \n");

	for (int i = 0; i < i_nt; i++)
	{
		if (i == 43)
		{
			// printf("hehe");
		}
		printf("%d  ", i);
		if (FF->LIST[i] != NULL)
		{
			hashmap *h = inithashmap();
			insert("eps", 0, h);
			struct grammar_rule_node *temp = FF->LIST[i];
			while (temp != NULL)
			{
				if (lookup(temp->lex->lexeme, h) == -1)
				{
					printf("[%s - %d] ", temp->lex->lexeme, temp->from_which);
					insert(temp->lex->lexeme, 0, h);
				}
				temp = temp->next;
			}
		}
		printf("\n");
	}

	return 0;
}