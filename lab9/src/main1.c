#include "main.h"

#define HERE fprintf(stderr, " HERE %i\n", __LINE__)
int main() {
  lab9();
  return EXIT_SUCCESS;
}

void lab9() {
  int numOfVert, pathStart, pathEnd, numOfEdges;
  if (scanf("%d %d %d %d", &numOfVert, &pathStart, &pathEnd, &numOfEdges) != 4)
    return;
  if (!IsValidSize(numOfVert, pathStart, pathEnd, numOfEdges)) return;
  TGraph* graph = CreateGraph(numOfVert, numOfEdges);
  if (FillGraph(graph) == FAIL) {
    DestroyGraph(graph);
    return;
  }
  TMinDist* minDistances = FindAllShortestPaths(pathStart, graph);
  PrintPathsToAllVertices(minDistances);
  PrintPath(minDistances, pathEnd, pathStart);
  DestroyGraph(graph);
  DestroyMinDist(minDistances);
}

int IsValidSize(int numOfVert, int pathStart, int pathEnd, int numOfEdges) {
  if (numOfVert < 0 || numOfVert > 5000) {
    printf("bad number of vertices");
    return 0;
  } else if (numOfEdges < 0 || numOfEdges > MAX_EDGE_COUNT(numOfVert)) {
    printf("bad number of edges");
    return 0;
  } else if (pathEnd < 1 || pathEnd > numOfVert || pathStart < 1 ||
             pathStart > numOfVert) {
    printf("bad vertex");
    return 0;
  }
  return 1;
}

TGraph* CreateGraph(int numOfVert, int numOfEdges) {
  struct TGraph* graph = malloc(sizeof(struct TGraph));
  graph->vertCount = numOfVert;
  graph->edgesCount = numOfEdges;
  graph->AdjMatrix = calloc(MATRIX_SIZE(numOfVert), sizeof(unsigned int));
  return graph;
}

TMinDist* FindAllShortestPaths(int start, TGraph* graph) {
  int size = graph->vertCount;
  TMinDist* minDist = InitMinDistArray(start, size);
  bool* visited = calloc(size, sizeof(bool));
  int minimumIdx;
  Node* vertex = minDist->vert;
  do {
    minimumIdx = MAX_VERTICES_SIZE;
    long long int minValueOfPath = LLONG_MAX;
    for (int i = 0; i < size; i++) {
      if (!visited[i] && (vertex[i].value < minValueOfPath)) {
        minValueOfPath = vertex[i].value;
        minimumIdx = i;
      }
    }
    if (minimumIdx != MAX_VERTICES_SIZE) {
      for (int i = 0; i < size; i++) {
        long long int weightOfCurEdge =
            GetEdgeWeight(minimumIdx, i, size, graph->AdjMatrix);
        if (weightOfCurEdge > 0) {
          long long int pathSize = minValueOfPath + weightOfCurEdge;
          if (pathSize <= vertex[i].value) {
            vertex[i].value = pathSize;
            vertex[i].vert = minimumIdx;
            if (vertex[i].value > INT_MAX) {
              vertex[i].intMaxCount++;
            }
          }
        }
      }
      visited[minimumIdx] = 1;
    }
  } while (minimumIdx < MAX_VERTICES_SIZE);
  free(visited);
  return minDist;
}

TMinDist* InitMinDistArray(int start, int vertCount) {
  TMinDist* minDist = calloc(1, sizeof(TMinDist));
  minDist->vert = calloc(vertCount, sizeof(Node));
  minDist->size = vertCount;
  for (int i = 0; i < vertCount; i++) {
    (minDist->vert)[i].value = LLONG_MAX;
  }
  (minDist->vert)[start - 1].value = 0;
  return minDist;
}

int IsValidValue(int n1, int n2, int weight, int numOfVert) {
  if (n1 < 1 || n2 < 1 || n1 > numOfVert || n2 > numOfVert) {
    printf("bad vertex");
    return 0;
  }
  if (weight < 0 || weight > INT_MAX) {
    printf("bad length");
    return 0;
  }
  return 1;
}

int FillGraph(TGraph* graph) {
  for (int i = 0; i < graph->edgesCount; i++) {
    int n1, n2, weight;
    if (!(scanf("%d %d %d", &n1, &n2, &weight) == 3)) {
      printf("bad number of lines");
      return FAIL;
    }
    if (!IsValidValue(n1, n2, weight, graph->vertCount)) return FAIL;
    PushEdge(n1 - 1, n2 - 1, weight, graph);
  }
  return SUCCESS;
}

void PrintPathsToAllVertices(TMinDist* minDist) {
  Node* vertex = minDist->vert;
  for (size_t i = 0; i < minDist->size; i++) {
    if (vertex[i].value == LLONG_MAX) {
      printf("oo ");
    } else if (vertex[i].value > INT_MAX) {
      printf("INT_MAX+ ");
    } else {
      printf("%lld ", vertex[i].value);
    }
  }
  printf("\n");
}

void PrintPath(TMinDist* minDist, int end, int start) {
  Node* vertex = minDist->vert;
  if (vertex[end - 1].intMaxCount >= 2) {
    printf("overflow");
  } else if (vertex[end - 1].value == LLONG_MAX) {
    printf("no path");
  } else if (end == start) {
    printf("%d ", start);
  } else {
    printf("%d ", end--);
    while (1) {
      end = vertex[end].vert;
      printf("%d ", end + 1);
      if (end == start - 1) break;
    }
  }
}

void PushEdge(int n1, int n2, int weight, TGraph* graph) {
  int min = MinValue(n1, n2);
  int max = MaxValue(n1, n2);
  int index = GetMatrixIndex(min, max, graph->vertCount);
  graph->AdjMatrix[index] = weight;
}

int GetMatrixIndex(int row, int column, int size) {
  return (2 * size - row - 1) * row / 2 + column;
}

int MaxValue(int v1, int v2) { return (v1 > v2) ? v1 : v2; }

int MinValue(int v1, int v2) { return (v1 < v2) ? v1 : v2; }

unsigned int GetEdgeWeight(int v1, int v2, int size, unsigned int* matrix) {
  int row = MinValue(v1, v2);
  int column = MaxValue(v1, v2);
  int index = GetMatrixIndex(row, column, size);
  return matrix[index];
}

void DestroyGraph(TGraph* graph) {
  free(graph->AdjMatrix);
  free(graph);
}

void DestroyMinDist(TMinDist* minDist) {
  free(minDist->vert);
  free(minDist);
}
