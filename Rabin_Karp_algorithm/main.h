#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXPATLEN 17
#define MAXTEXTLEN 1024
#define SIMPLENUM 3

void lab1_1();
int ReadPattern(FILE* input, unsigned char *pattern, int* patLen);
void RabinCarpSearch(FILE* input, unsigned char* pattern, int patLen);
int GetText(FILE* input, unsigned char* text, int *textGet);
long long int NextHash(FILE* input, long long int* textHash, unsigned char* text, int patLen, long long int size, int* curs, int *flag, int *textGet);
long long int FirstHash(unsigned char* str, int len, long long int* size);
void IsMatch(unsigned char* pattern, unsigned char* text, int patLen, int curs, int count);
