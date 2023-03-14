#include "main.h"

#define HERE fprintf(stderr, " HERE %i\n", __LINE__);
int main() {
  lab9();
  return EXIT_SUCCESS;
}

void lab9() {
  errors status = success;
  int numOfVert, pathStart, pathEnd, numOfEdges;
  if (scanf("%d %d %d %d", &numOfVert, &pathStart, &pathEnd, &numOfEdges) != 4)
    return;

  if (!IsValidSize(numOfVert, pathStart, pathEnd, numOfEdges)) return;
  TGraph* graph = CreateGraph(numOfVert, numOfEdges);
  FillGraph(graph);
  TMinDist* minDistances = FindAllShortestPaths(pathStart, graph);
  PrintPathsToAllVertices(minDistances);
  PrintAnswer(minDistances, pathEnd, pathStart);
  // TVector* path =
  //     GetShortestPath(pathStart, pathEnd, minDistances, graph, &status);
  DestroyGraph(graph);
}

void FindShortestPath(TGraph graph, int indexSource, TMinDist* pathLengthArr,
                      char* visited, int* count, int* printArr) {
  for (int i = 0; i < graph.vertCount; i++) {
    int index = SearchIndex(indexSource, i, graph);
    if (graph.AdjMatrix[index] != 0 && visited[i] == 0) {
      long long curLength =
          (long long)graph.AdjMatrix[index] + pathLengthArr->value[indexSource];
      if (curLength <= pathLengthArr->value[i]) {
        pathLengthArr->value[i] = curLength;
        printArr[i] = indexSource;
      }
      if (*count != 2 && pathLengthArr->value[i] > INT_MAX) {
        ++(*count);
      }
    }
  }
}

// TVector* GetShortestPath(int start, int finish, TMinDist* minDist,
//                          TGraph* graph, errors* status) {
//   TVector* path = CreateVector(graph->vertCount);
//   int endOfPath = finish - 1;
//   path->vector[0] = finish;

//   return path;
// }

void PrintAnswer(TMinDist* minDist, int end, int start) {
  while (minDist->vert[end - 1] != start - 1) {
    printf("%d ", minDist->vert[end - 1]);
    end = minDist->vert[end - 1]
  }
}

TVector* CreateVector(int size) {
  TVector* newPath = malloc(sizeof(TVector));
  newPath->vector = malloc(sizeof(int) * size);
  newPath->size = size;
  newPath->capacity = size;
  return newPath;
}

TMinDist* FindAllShortestPaths(int start, TGraph* graph) {
  int size = graph->vertCount;
  TMinDist* minDist = InitMinDistArray(start, size);
  bool* visited = calloc(size, sizeof(bool));
  int minimumIdx;
  do {
    minimumIdx = MAX_VERTICES_SIZE;
    long long int minValueOfPath = LLONG_MAX;
    for (int i = 0; i < size; i++) {
      if (!visited[i] && (minDist->value[i] < minValueOfPath)) {
        minValueOfPath = minDist->value[i];
        minimumIdx = i;
      }
    }
    if (minimumIdx != MAX_VERTICES_SIZE) {
      for (int i = 0; i < size; i++) {
        long long int weightOfCurEdge =
            GetEdgeWeight(minimumIdx, i, size, graph->AdjMatrix);
        if (weightOfCurEdge > 0) {
          long long int temp = minValueOfPath + weightOfCurEdge;
          if (temp < minDist->value[i]) {
            minDist->value[i] = temp;
            minDist->vert[i] = minimumIdx;
          }
        }
      }
      visited[minimumIdx] = 1;  // Посещена
    }
  } while (minimumIdx < MAX_VERTICES_SIZE);
  free(visited);
  return minDist;
}

TMinDist* InitMinDistArray(int start, int vertCount) {
  TMinDist* minDist = malloc(sizeof(TMinDist));
  minDist->value = malloc(sizeof(long long int) * vertCount);
  minDist->vert = malloc(sizeof(long long int) * vertCount);
  minDist->size = vertCount;
  for (int i = 0; i < vertCount; i++) {
    minDist->value[i] = LLONG_MAX;
  }
  minDist->value[start - 1] = 0;
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

TGraph* CreateGraph(const int numOfVert, const int numOfEdges) {
  struct TGraph* graph = calloc(1, sizeof(struct TGraph));
  graph->vertCount = numOfVert;
  graph->edgesCount = numOfEdges;
  graph->AdjMatrix = calloc(MATRIX_SIZE(numOfVert), sizeof(unsigned int));
  return graph;
}

void FillGraph(TGraph* graph) {
  for (int i = 0; i < graph->edgesCount; i++) {
    int n1, n2, weight;
    if (!(scanf("%d %d %d", &n1, &n2, &weight) == 3)) {
      printf("bad number of lines");
      return;
    }
    if (!IsValidValue(n1, n2, weight, graph->vertCount)) return;
    PushEdge(n1 - 1, n2 - 1, weight,
             graph);  //  -1 for adding values from zero pos
  }
}

void AddToQueue(TQueue** queue, int v, int weight) {
  TQueue* newTree = (TQueue*)calloc(1, sizeof(TQueue));
  newTree->value = v;
  newTree->weight = weight;
  newTree->next = NULL;
  if (!(*queue)) {
    *queue = newTree;
  } else {
    *queue = FindPlace(*queue, newTree);
  }
}

void PrintPathsToAllVertices(TMinDist* minDist) {
  for (size_t i = 0; i < minDist->size; i++) {
    if (minDist->value[i] == LLONG_MAX) {
      printf("oo ");
    } else if (minDist->value[i] > INT_MAX) {
      printf("INT_MAX+ ");
    } else {
      printf("%lld ", minDist->value[i]);
    }
  }
  printf("\n");
}

int Pop(TQueue** head) {
  int ans = 0;
  if (*head) {
    TQueue* tmp = *head;
    ans = tmp->value;
    *head = (*head)->next;
    free(tmp);
  }
  return ans;
}

void RemoveVert(TQueue** queue, int v) {
  if (*queue) {
    if (v == (*queue)->value) {
      Pop(queue);
    } else {
      RemoveVert(&(*queue)->next, v);
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

int MaxValue(const int v1, const int v2) { return (v1 > v2) ? v1 : v2; }

int MinValue(const int v1, const int v2) { return (v1 < v2) ? v1 : v2; }

void DestroyGraph(TGraph* graph) {
  free(graph->AdjMatrix);
  free(graph);
}

TQueue* LowerBound(TQueue* queue, unsigned int weight) {
  if (queue->weight >= weight) return NULL;
  TQueue* p = queue;
  while (p->next && p->next->weight < weight) {
    p = p->next;
  }
  return p;
}

TQueue* FindPlace(TQueue* queue, TQueue* newTQueue) {
  TQueue* tmp = LowerBound(queue, newTQueue->weight);
  if (!tmp) {
    newTQueue->next = queue;
    queue = newTQueue;
  } else {
    newTQueue->next = tmp->next;
    tmp->next = newTQueue;
  }
  return queue;
}

unsigned int GetEdgeWeight(int v1, int v2, int size, unsigned int* matrix) {
  int row = MinValue(v1, v2);
  int column = MaxValue(v1, v2);
  int index = GetMatrixIndex(row, column, size);
  return matrix[index];
}
