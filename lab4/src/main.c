#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#define MAXSTRLEN 1000
#define MAXSTRLENFORARRAY 1001
#define STRINGIFY(a) STRINGIFy(a)
#define STRINGIFy(a) #a

typedef struct Notation Notation;
struct Notation{
    int value;
    Notation* next;
};

void lab4();
int SyntaxERROR(char* input);
void SendToStack(int a, Notation** head);
void PopNoValue(Notation** head);
void Destroy(Notation* head);
int Calculate(char input[]);
int ConvertToRPN(char* input, char* output);

int main(void) {
    lab4();
    return EXIT_SUCCESS;
}

void SendToStack(int a, Notation** head) {
    Notation* tmp = malloc(sizeof(Notation));
    tmp->next = *head;
    tmp->value = a;
    *head = tmp;
}

void PopNoValue(Notation** head) {
    if (*head) {
        Notation* tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}

void Destroy(Notation* head) {
    if (head != NULL) {
        Notation* tmp;
        tmp = head->next;
        free(head);
        Destroy(tmp);
    }
}

int SyntaxError(char* input) {
    int i = 0, bracket = 0;
    while (input[i] != '\0') {
        if (input[i] <= '9' && input[i] >= '0') {
            i++;
            continue;
        }
        if (input[i] == '(' && input[i + 1] != ')') {
            i++;
            bracket++;
            continue;
        }
        if (input[i] == ')') {
            if (bracket > 0) {
                bracket--;
                i++;
                continue;
            }
        }
        if (i > 0 && (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/')) {
            if (input[i] == '/' && input[i + 1] == '0') {
                printf("division by zero");
                exit(EXIT_SUCCESS);
            }
            if (isdigit(input[i - 1]) || input[i - 1] == ')') {  //  Is 2 signes near
                if (isdigit(input[i + 1]) || input[i + 1] == '(') {
                    i++;
                    continue;
                }
            }
        }
        return 1;
    }
    if (bracket != 0)
        return 1;
    return 0;
}

void lab4() {
    char input[MAXSTRLEN + 2];
    if (!scanf("%" STRINGIFY(MAXSTRLENFORARRAY) "[^\n]", input)) {
        printf("syntax error");
        return;
    }
    if (SyntaxError(input)) {
        printf("syntax error");
        return;
    }
    int result = Calculate(input);
    printf("%d", result);
}

int Calculate(char input[]) {
    char output[MAXSTRLEN * 2];
    int i = 0, length = ConvertToRPN(input, output);
    Notation* signStack = NULL;
    while (i < length) {
        if (isdigit(output[i])) {
            int count = 0;
            while (isdigit(output[i])) {
                count *= 10;
                count += output[i] - '0';
                i++;
            }
            i++;
            SendToStack(count, &signStack);
            continue;
        }
        int one = signStack->value, two = (signStack->next)->value;
        PopNoValue(&signStack);
        switch (output[i]) {
        case '+':
            two += one;
            break;
        case '-':
            two -= one;
            break;
        case '/':
            if (one == 0) {
                printf("division by zero");
                Destroy(signStack);
                exit(EXIT_SUCCESS);  // put it as we should track down and finish the program
            } else {
                two /= one;
            }
            break;
        case '*':
            two *= one;
            break;
        }
        i += 2;
        signStack->value = two;
    }
    int result = signStack->value;
    Destroy(signStack);
    return result;
}

int ConvertToRPN(char* input, char* output) {
    int i = 0, j = 0;
    Notation* head = NULL;
    while (input[i] != '\0') {
        if (isdigit(input[i])) {
            while (isdigit(input[i])) {
                output[j] = input[i];
                i++;
                j++;
            }
            output[j] = ' ';
            j++;
        }
        switch (input[i]) {
        case '+':
        case '-':
            if ((head != NULL) && (head->value == '*' ||
                head->value == '/' || head->value == '-' || head->value == '+')) {
                while (head != NULL && head->value != '(') {
                    output[j] = head->value;
                    output[j + 1] = ' ';
                    j += 2;
                    PopNoValue(&head);
                }
            }
            SendToStack(input[i], &head);
            i++;
            break;
        case '*':
        case '/':
            if ((head != NULL) && (head->value == '*' || head->value == '/')) {
                output[j] = head->value;
                output[j + 1] = ' ';
                j += 2;
                PopNoValue(&head);
            }
            SendToStack(input[i], &head);
            i++;
            break;
        case '(':
            SendToStack(input[i], &head);
            i++;
            break;
        case ')':
            if ((head != NULL) && (head->value == '*' ||
             head->value == '/' || head->value == '-' || head->value == '+')) {
                while (head->value != '(') {
                    output[j] = head->value;
                    output[j + 1] = ' ';
                    j += 2;
                    PopNoValue(&head);
                }
            }
            i++;
            PopNoValue(&head);  //  deleting '(' bracket
            break;
        }
    }
    // All characters are processed
    while (head != NULL) {
        output[j] = head->value;
        output[j + 1] = ' ';
        PopNoValue(&head);
        j += 2;
    }
    j -= 2;
    return j + 1;
}

