#include "main.h"

int main(void) {
    lab2();
    return EXIT_SUCCESS;
}

void lab2() {
    char str[NMAX + 1];
    int alf[NMAX] = {0};
    int counter = 0;
    char ch;
    while ((ch = getchar()) != '\n') {
        if (ch < '0' || ch > '9' || alf[ch - '0'] != 0 || ch == ' ') {
            Error();
        }
        alf[ch - '0'] += 1;
        str[counter] = ch;
        counter++;
    }
    str[counter] = '\0';
    long long int n;
    if (scanf("%lld", &n) != 1) {
        Error();
    }
    int perm = n > counter * counter ? counter * counter : n;  // permutations,
    // counter * counter is the number of possible permutations
    for (int i = 0; i < perm; i++)
    Narayana(str, counter);
}

int Narayana(char str[], int strl) {  // strl = str length
    int stri = BIGGEST, strj = 0, min = '9' + 1;
    for (int i = strl - 2; i >= 0; i--) {
            if (str[i] < str[i+1]) {
            stri = i;
            break;
        }
    }
    if (stri == BIGGEST)
        return 1;
    for (int j = stri + 1; j < strl; j++) {
        if (str[j] > str[stri] && min > str[j]) {
            min = str[j];
            strj = j;
        }
    }
    Swap(&str[stri], &str[strj]);
    ReverseStr(str, stri + 1, strl);
    printf("%s\n", str);
    return 1;
}

void Error() {
    printf("bad input\n");
    exit(0);
}

void Swap(char *a, char *b) {
    long int tmp = *a;
    *a = *b;
    *b = tmp;
}

void ReverseStr(char ans[], int left, int right) {
    char rev[NMAX + 1];
    int i = 0;
    if (left >= right)
        exit(0);
    for (int j = left; j < right; j++, i++) {
        rev[i] = ans[j];
    }
    i = 0;
    for (int j = right - 1; j >= left; j--, i++) {
        ans[j] = rev[i];
    }
}
