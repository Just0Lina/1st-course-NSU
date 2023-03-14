
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TGraph TGraph;
struct TGraph {
  int vertCount, edgesCount;
  unsigned int *AdjMatrix;
};

typedef struct TTree TTree;
struct TTree {
  int n1, n2;
  unsigned int weight;
  bool addedToAns;
};

typedef struct TQueue TQueue;
struct TQueue {
  int value;
  unsigned int weight;
  TQueue *next;
};

#define MAX_EDGE_COUNT(size) (size * (size - 1) / 2)
#define MATRIX_SIZE(size) ((size + (size * size)) / 2)

void lab8_1();
TGraph *CreateGraph(const int addedToAns, const int numOfEdges);
int IsValidSize(int addedToAns, int numOfEdges);
void FillGraph(TGraph *graph);
int IsValidValue(int n1, int n2, int weight, int addedToAns);
TTree *PrimAlgorithm(TGraph *graph);
void PrintAnswer(TTree *T, int addedToAns);
void DestroyGraph(TGraph *graph);
void AddToQueue(TQueue **queue, int v, int weight);
int Pop(TQueue **head);
void RemoveVert(TQueue **queue, int v);
TQueue *FindPlace(TQueue *queue, TQueue *newTQueue);
TQueue *LowerBound(TQueue *queue, unsigned int weight);
void PushEdge(int n1, int n2, int weight, TGraph *graph);
int MaxValue(const int v1, const int v2);
int MinValue(const int v1, const int v2);
int GetMatrixIndex(int row, int column, int size);
unsigned int GetEdgeWeight(int row, int column, int size, unsigned int *matrix);
void ChangeSpTree(int n1, int n2, int weight, TTree *spTree);
int NoSpTree(int getSize, int size);
void MainBody(int *countVert, int vert, int size, TTree *spTree, TQueue **queue,
              unsigned int *matrix);
