#include <stdio.h>
#include <stdlib.h>
void Swap(int *a, int *b);
void Heapify(int arr[], long int N, int index);
void lab3_0();
void PrintOutput(int arr[], int N);

int main() {
    lab3_0();
    return 0;
}

void lab3_0() {
    long int N;  // ДО 2млн
    if (scanf("%ld", &N) == 1) {
        int *arr = (int*)malloc(sizeof(int) * N+1);
        for (int j = 0; j < N; j++) {
            if (scanf("%d", &arr[j]) != 1) {
                free(arr);
                exit(0);
            }
        }
        for (long int i = N / 2 - 1; i >= 0; i--) Heapify(arr, N, i);
        for (long int i = N - 1; i >= 0; i--) {
            Swap(&arr[0], &arr[i]);
            Heapify(arr, i, 0);
        }
        PrintOutput(arr, N);
        free(arr);
    }
}

void Heapify(int arr[], long int N, int index) {
    long int largest = index;
    long int leftCh = 2 * index + 1; // leftCh = leftChildIndex
    long int rightCh = 2 * index + 2; // right = rightChildIndex
    if (leftCh < N && arr[leftCh] > arr[largest]) {
        largest = leftCh;
    }
    if (rightCh < N && arr[rightCh] > arr[largest]) {
        largest = rightCh;
    }
    if (largest != index) {
        Swap(&arr[index], &arr[largest]);
        Heapify(arr, N, largest);
    }
}

void Swap(int *a, int *b) {
    long int tmp = *a;
    *a = *b;
    *b = tmp;
}

void PrintOutput(int arr[], int N) {
    for (int j = 0; j < N; j++) {
        printf("%d ", arr[j]);
    }
}

