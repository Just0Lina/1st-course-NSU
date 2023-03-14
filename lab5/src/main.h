#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t Tbits;
typedef struct HTree HTree;
struct HTree {
  int value;
  int priority;
  HTree *next;
  HTree *Rchild;
  HTree *Lchild;
};

typedef struct TBitOut {
  int Size;
  Tbits out;
} TBitOut;

typedef struct {
  int Size;
  Tbits Code;
} TCode;

typedef struct {
  int pos;
  Tbits value;
  Tbits temp;
  int addInf;
  int curs;
  int IsEnded;
} TDecodeValue;

#define ALPH 256
#define BUFF_SIZE 1024
#define TBITS_SIZE 8
#define EXTRA_SIZE 3

void lab5();
void Encode(FILE *in, FILE *out);
void Decode(FILE *in, FILE *out);
void Destroy(HTree *tree);
void WriteBit(TBitOut *bitout, int size, Tbits *code, FILE *out);

//  Encode func
HTree *PriorityQueue(int *list);
HTree *LowerBound(HTree *Queue, int priority);
HTree *HaffmanTree(HTree *Queue);
HTree *FindPlace(HTree *Queue, HTree *NewHTree);
void BuildCode(HTree *tree, TCode *Code);
void WalkTree(HTree *tree, TCode *Code, TCode *Cur);
int CodeTree(HTree *tree, TBitOut *bitout, FILE *out);
int MoveTree(HTree *tree, TBitOut *bitout, FILE *out);
void BitWriter(TBitOut *bitout, TCode *Code, FILE *in, FILE *out);
HTree *Child(FILE *in, Tbits *value, int *i);
void FindLeaf(TBitOut *bitout, Tbits tmp0, HTree *tree, int *N, FILE *out);

//  Decode func
int GetValue(FILE *in, Tbits *value, int *i, int Len);
int GetBit(Tbits value, int i);
HTree *GetTree(FILE *in, int *i, Tbits *value);
void DecodeFile(HTree *Tree, FILE *in, Tbits *value, int *i, FILE *out);
int FillList(FILE *in, int *list);
int EncodeFileLen(int *list, TCode *Code);
int MoveSigns(TDecodeValue *value, FILE *in);

