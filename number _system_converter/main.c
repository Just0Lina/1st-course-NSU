#define MAXLEN_X 49
#define STRINGIFY(a) STRINGIFy(a)
#define STRINGIFy(a) #a
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "main.h"

int main() {
    Lab0();
    return 0;
}

void Lab0() {
    int b1, b2;
    if (scanf("%d %d", &b1, &b2) != 2) {
        Error();
    }
     if (b1 > 16 || b1 < 2 || b2 > 16 || b2 < 2) {
         Error();
        }
    char X[MAXLEN_X + 1];
    if (!scanf("%" STRINGIFY(MAXLEN_X) "s", X)) {
        Error();
    }
    if (!CheckStr(X, b1)) {
      Error();
    } 
    ConvertToValue(X, b1,b2);
}

int CheckStr(char* X, int b1) {
    char* tmp_X = X;
    int no_mistakes = 1, dots = 0;
    if (*(tmp_X) == '.') {
        no_mistakes = 0;
    }
    for (; *tmp_X!= '\0' && no_mistakes; tmp_X++) {
        char current_char = *tmp_X; 
        if ((current_char - '0' >= b1 || current_char < '0') && current_char != '.' ) {
            no_mistakes = 0;
        } else if (current_char == '.') {
            dots += 1;
            }
        if (b1 > 10) {
            current_char = tolower(*tmp_X); //Если заглавная буква меняем, иначе ничего не происходит
            if (current_char >= 'a' && current_char < b1 - 10 + 'a') {
                no_mistakes = 1;
            }
        } 
    } if (no_mistakes) {
        if (*(tmp_X-1) == '.' || dots > 1) {
            no_mistakes = 0;
        }
    }
    return no_mistakes;// Проверяем число на валидность
}

void ConvertToValue(char *chis, int b1, int b2) {
    long long int numerator = 0;
    long long int denominator = 1;
    for (; *chis!= '\0' && *chis!= '.' ; chis++) {
        numerator = CharToValue(chis, &numerator, b1);
    }
    if (*chis == '.') {
        chis++;
        for ( ;*chis != '\0'; chis++, denominator *= b1) {
            numerator = CharToValue(chis, &numerator, b1);
        }
    }
    ConvertToB2(&numerator, denominator, b2);
}

void Error() {
    printf("bad input");
    exit(0); // В случае ошибки
}

void ConvertToB2(long long int *numerator, long long int denominator, int b2) {
    char ans_rev[2 * MAXLEN_X];
    int i = 0;
    if (*numerator / denominator == 0) {            
        (ans_rev)[i] = '0';
        i++;
    } else { 
        long long int tempo = *numerator / denominator; 
        while (tempo > 0) {
        (ans_rev)[i] = DigitToStr(tempo % b2);
        i++;
        tempo/= b2;
        }
    }
    char ans[2 * MAXLEN_X];
    ReflectStr(ans, ans_rev, i);
    *numerator -= *numerator / denominator * denominator; // Оставляем обыкновенную дробь
    if (*numerator / denominator * denominator != *numerator) {
        (ans)[i] = '.';
        i++;
        int counter = 0;
        for (;*numerator != 0 && counter < 12; counter++, i++) {
            int temp = *numerator * b2 / denominator;
            *numerator = *numerator * b2  - temp  * denominator ;
            (ans)[i] = DigitToStr(temp);
        }
    }
    ans[i] = '\0';
    printf("%s\n", ans);
}

void ReflectStr(char* ans, char* ans_rev, int i) {
    for (int j = i - 1; j >= 0; j--) {
        (ans)[i - 1 - j] = ans_rev[j]; 
    }
}

char DigitToStr(long long int chis) {
    return (chis) > 9 ? (chis + 'A' - 10  ): (chis + '0');
}

long long int CharToValue(char *chis, long long int *numerator, int b1) {
    if (isdigit(*chis)) {
        *numerator = (*chis - '0')  + *numerator * b1;
    } else {
        *numerator = (toupper(*chis) - 'A' + 10) + *numerator * b1;
    }
    return *numerator;
}

