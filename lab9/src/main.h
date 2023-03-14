#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TGraph TGraph;
struct TGraph {
  int vertCount, edgesCount;
  unsigned int* AdjMatrix;
};

typedef struct Node {
  int vert;
  int intMaxCount;
  long long int value;
} Node;

typedef struct TMinDist {
  size_t size;
  Node* vert;
} TMinDist;

#define MAX_VERTICES_SIZE 5000
#define MAX_EDGE_COUNT(size) ((size) * ((size)-1) / 2)
#define MATRIX_SIZE(size) (((size) + ((size) * (size))) / 2)
#define FAIL 1
#define SUCCESS 0

void lab9();

// Main function
TMinDist* FindAllShortestPaths(int start, TGraph* graph);

// Initialization of graph
TGraph* CreateGraph(int addedToAns, int numOfEdges);
int IsValidSize(int numOfVert, int pathStart, int pathEnd, int numOfEdges);
int FillGraph(TGraph* graph);
int IsValidValue(int n1, int n2, int weight, int addedToAns);

// Helpful functions
void DestroyGraph(TGraph* graph);
void DestroyMinDist(TMinDist* minDist);
TMinDist* InitMinDistArray(int start, int vertCount);
void PushEdge(int n1, int n2, int weight, TGraph* graph);
int MaxValue(int v1, int v2);
int MinValue(int v1, int v2);
int GetMatrixIndex(int row, int column, int size);
unsigned int GetEdgeWeight(int row, int column, int size, unsigned int* matrix);
void PrintPathsToAllVertices(TMinDist* minDist);
void PrintPath(TMinDist* minDist, int end, int start);
