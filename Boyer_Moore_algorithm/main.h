#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAXSTRLEN 17
#define MAXPATLEN 17
#define ALPHABETLEN 256

void lab1_0();
void AlphIndex(int *pattern, int* moves, int patLen);
void BoyerMoore(int* text, int* pattern, int* moves, int patLen, int textLen);
int TextGet(int text[], int *curs, int *textLen);
