#include <stdio.h>
#include <stdlib.h>
void lab7();
char CheckVerticesEdges(short N, int M);
char AddEdge(char* input, int N, int M);
void OutputTopSort(short* output, short N);
char TopologicalSort(char* input, short* output, short N, short* curOut);
char DFS(char* visited, char* input, short* output, short N, short i, short* cur_out);

char CheckVerticesEdges(short N, int M) {
    if (N > 2000 || N < 0) {
        printf("%s", "bad number of vertices");
        return -1;
    }
    if (M < 0 || M > (N * (N + 1)) / 2) {
        printf("%s", "bad number of edges");
        return -1;
    }
    return 0;
}

char AddEdge(char* input, int N, int M) {
    for (int i = 0; i < M; i++) {
        short from, to;
        if (scanf("%hi %hi", &from, &to) != 2) {
            printf("%s", "bad number of lines");
            return -1;
        }
        if (from < 1 || from > N || to < 1 || to > N) {
            printf("%s", "bad vertex");
            return -1;
        }
        int position = (from - 1) * N + (to - 1);
        if ((input[position / 8] & (1 << (position % 8))) == 0) {
            input[position / 8] += 1 << (position % 8);
        }
    }
    return 0;
}

void OutputTopSort(short* output, short N) {
    for (short i = N - 1; i >= 0; i--) {
        printf("%d ", output[i] + 1);
    }
    free(output);
}

char DFS(char* visited, char* input, short* output, short N, short i, short* cur_out) {
    if (visited[i] == -1) return 0;
    if (visited[i] == 1) return -1;
    visited[i] = 1;
    for (short m = 0; m < N; m++) {
        int pos = i * N + m;
        if ((input[pos / 8] & (1 << (pos % 8))) == 1 << (pos % 8)) {
            if (DFS(visited, input, output, N, m, cur_out) == -1) {
                return -1;
            }
        }
    }
    visited[i] = -1;
    output[*cur_out] = i;
    (*cur_out)++;
    return 0;
}

char TopologicalSort(char* input, short* output, short N, short* curOut) {
    char* visited = (char*)calloc(N, sizeof(char));
    for (short i = 0; i < N; i++) {
        if (DFS(visited, input, output, N, i, curOut) == -1) {
            free(visited);
            return -1;
        }
    }
    free(visited);
    return 0;
}

int main() {
    lab7();
    return 0;
}

void lab7() {
    int M, N;
    if (scanf("%d %d", &N, &M) != 2) {
        printf("%s", "bad number of lines");
        return;
    }
    if (CheckVerticesEdges(N, M) == -1) return;
    char* input = (char*)calloc((N * N) / 8 + 1, sizeof(char));
    if (AddEdge(input, N, M) == -1) {
        free(input);
        return;
    }
    short* output = (short*)calloc(N, sizeof(short));
    short curOut = 0;
    if (TopologicalSort(input, output, N, &curOut) == -1) {
        printf("impossible to sort");
        free(input);
        free(output);
        return;
    }
    OutputTopSort(output, N);
    free(input);
}

