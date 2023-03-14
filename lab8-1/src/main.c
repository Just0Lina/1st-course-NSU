#include "main.h"

#define HERE fprintf(stderr, " HERE %i\n", __LINE__);
int main() {
  lab8_1();
  return EXIT_SUCCESS;
}

void lab8_1() {
  int numOfVert, numOfEdges;
  if (scanf("%d %d", &numOfVert, &numOfEdges) != 2) return;
  if (!IsValidSize(numOfVert, numOfEdges)) return;
  if (!numOfVert) {
    printf("no spanning tree");
    return;
  }
  TGraph* graph = CreateGraph(numOfVert, numOfEdges);

  FillGraph(graph);
  TTree* MST = PrimAlgorithm(graph);
  if (MST) PrintAnswer(MST, numOfVert);
  DestroyGraph(graph);
  free(MST);
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

int IsValidSize(int numOfVert, int numOfEdges) {
  if (numOfVert < 0 || numOfVert > 5000) {
    printf("bad number of vertices");
    return 0;
  } else if (numOfEdges < 0 || numOfEdges > MAX_EDGE_COUNT(numOfVert)) {
    printf("bad number of edges");
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

TTree* PrimAlgorithm(TGraph* graph) {
  int countVert = 0, vertSize = graph->vertCount;
  TTree* spTree = calloc(vertSize, sizeof(TTree));  // Часть ответа
  TQueue* queue = NULL;
  AddToQueue(&queue, 0, 0);
  while (queue) {
    int vert = Pop(&queue);
    MainBody(&countVert, vert, vertSize, spTree, &queue, graph->AdjMatrix);
  }
  if (NoSpTree(countVert, vertSize)) {
    free(spTree);
    return NULL;
  }
  return spTree;
}

void MainBody(int* countVert, int vert, int size, TTree* spTree, TQueue** queue,
              unsigned int* matrix) {
  *countVert += 1;
  spTree[vert].addedToAns = true;
  for (int i = 0; i < size; i++) {
    if (spTree[i].addedToAns) continue;
    unsigned int weight =
        GetEdgeWeight(MinValue(vert, i), MaxValue(vert, i), size, matrix);
    if (weight && (weight < spTree[i].weight || !spTree[i].weight)) {
      if (*queue) RemoveVert(queue, i);
      ChangeSpTree(i, vert, weight, spTree);
      AddToQueue(queue, i, weight);
    }
  }
}

int NoSpTree(int getSize, int size) {
  if (getSize != size) {
    printf("no spanning tree");
    return 1;
  }
  return 0;
}

void ChangeSpTree(int n1, int n2, int weight, TTree* spTree) {
  spTree[n1].n1 = n1;
  spTree[n1].n2 = n2;
  spTree[n1].weight = weight;
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

void PrintAnswer(TTree* T, int addedToAns) {
  for (int i = 1; i < addedToAns; i++) {
    if (T[i].addedToAns) printf("%d %d\n", T[i].n1 + 1, T[i].n2 + 1);
  }
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

unsigned int GetEdgeWeight(int row, int column, int size,
                           unsigned int* matrix) {
  int index = GetMatrixIndex(row, column, size);
  return matrix[index];
}
