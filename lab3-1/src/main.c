#include <stdio.h>
#include <stdlib.h>
void Swap(int *a, int *b);
void lab3_1();
void QuickSort(int arr[], int first, int last);
void PrintOutput(int arr[], int N);

int main() {
    lab3_1();
    return 0;
}

void lab3_1() {
    long int N;  // ДО 2млн
    if (scanf("%ld", &N) == 1) {
        int *arr = (int*)malloc(sizeof(int) * N);
        for (int j = 0; j < N; j++) {
            if (scanf("%d", &arr[j]) != 1) {
                free(arr);
                exit(0);
            }
        }
        QuickSort(arr, 0, N - 1);
        PrintOutput(arr, N);
        free(arr);
    }
}

void QuickSort(int arr[], int first, int last) {
    if (first < last) {
        int left = first, right = last, middle = arr[(first + last) / 2];
        do {
            while (arr[left] < middle) {
                left++;
            }
            while (arr[right] > middle) {
                right--;
            }
            if (left <= right) {
                Swap(&arr[left], &arr[right]);
                right--;
                left++;
            }
        } while (left <= right);
        QuickSort(arr, left, last);
        QuickSort(arr, first, right);
    }
}

void Swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void PrintOutput(int arr[], int N) {
    for (int j = 0; j < N; j++) {
        printf("%d ", arr[j]);
    }
}

