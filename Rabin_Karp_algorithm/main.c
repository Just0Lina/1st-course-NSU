#include "main.h"

int main(void) {
    lab1_1();
    return EXIT_SUCCESS;
}

void lab1_1() {
    FILE* input = fopen("in.txt", "rx");
    if (input == NULL) return;
    unsigned char pattern[MAXPATLEN+1];
    int patLen = 0;
    if (ReadPattern(input, pattern, &patLen)) {
        RabinCarpSearch(input, pattern, patLen);
    }
    fclose(input);
}

int ReadPattern(FILE* input, unsigned char* pattern, int* patLen) {
    for (int i = 0; i < MAXPATLEN + 1; ++i) {
        if (!(pattern[i] = fgetc(input)))
            return 0;
        if (pattern[i] == '\n') {
            pattern[i] = '\0';
            break;
        }
        *patLen += 1;
    }
    return 1;
}

void RabinCarpSearch(FILE* input, unsigned char* pattern, int patLen) {
    long long int textHash, patternHash, size = 1, s = 1;
    int  count = 0, textGet = 0;
    unsigned char text[MAXTEXTLEN + 1];
    patternHash = FirstHash(pattern, patLen, &size);
    printf("%lld ", patternHash);
    if (!GetText(input, text, &textGet))
        return;
    textHash = FirstHash(text, patLen, &s);
    int curs = 0, flag = textGet == MAXTEXTLEN ? 1 : 0; // flag shows whether the end of text found
    for (; 1; ++curs, ++count) {
        if (textHash == patternHash)
            IsMatch(pattern, text, patLen, curs, count);
        if (!NextHash(input, &textHash, text, patLen, size, &curs, &flag, &textGet)) {
            return;
        }
    }
}

int GetText(FILE* input, unsigned char* text, int *textGet) {
    if (!(*textGet = fread(text, sizeof(unsigned char), MAXTEXTLEN, input))) {
        return 0;
    }
    text[MAXTEXTLEN] = '\0';
    return 1;
}

long long int NextHash(FILE* input, long long int* textHash, unsigned char* text, int patLen, long long int size, int* curs, int *flag, int *textGet) {
    *textHash = *textHash / SIMPLENUM;
    if (*curs + patLen >= MAXTEXTLEN && *flag) {
        memmove(text, &text[*curs], MAXTEXTLEN - *curs);
        *textGet = fread(&text[MAXTEXTLEN - *curs], sizeof(unsigned char), *curs, input);
        if (*textGet != *curs) {
            *flag = 0;
        }
        *textGet += MAXTEXTLEN - *curs;
        *curs = 0;
    }
    if (*curs + patLen > MAXTEXTLEN || (!(*flag) && *curs + patLen >= *textGet)) { return 0; }
    *textHash += text[*curs + patLen] % SIMPLENUM * size;
    return 1;
}


long long int FirstHash(unsigned char* str, int len, long long * size) {
    long long int res = 0;
    for (int i = 0; i < len; i++) {
        res += str[i] % SIMPLENUM * *size;
        *size *= SIMPLENUM;
    }
    *size /= SIMPLENUM;
    return res;
}

void IsMatch(unsigned char* pattern, unsigned char* text, int patLen, int curs, int count) {
    for (int i = 0; i < patLen; i++) {
        printf("%d ", count + i + 1);
        if (!(pattern[i] == text[curs + i])) return;
    }
}
