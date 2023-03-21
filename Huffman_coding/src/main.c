#include "main.h"
int main() {
  lab5();
  return EXIT_SUCCESS;
}

void lab5() {
  FILE *in = fopen("in.txt", "rb");
  FILE *out = fopen("out.txt", "wb");
  int mode;
  if ((mode = fgetc(in)) == EOF) {
    fclose(in);
    fclose(out);
    return;
  }
  if (mode == 'c') {
    Encode(in, out);
  } else if (mode == 'd') {
    Decode(in, out);
  }
  fclose(in);
  fclose(out);
}

void Encode(FILE *in, FILE *out) {
  int *list = (int *)calloc(ALPH, sizeof(int));
  if (!FillList(in, list)) {
    free(list);
    return;
  }
  HTree *Queue = PriorityQueue(list);
  HTree *Tree = HaffmanTree(Queue);
  TCode Code[ALPH] = {0};
  BuildCode(Tree, Code);
  TBitOut *Bitout = (TBitOut *)calloc(1, sizeof(TBitOut));
  int encodeSize =
      EXTRA_SIZE + CodeTree(Tree, Bitout, out) + EncodeFileLen(list, Code);
  int extraSize = (TBITS_SIZE - encodeSize % TBITS_SIZE) % TBITS_SIZE;
  WriteBit(Bitout, EXTRA_SIZE, (Tbits *)&extraSize, out);
  BitWriter(Bitout, Code, in, out);
  Destroy(Tree);
  free(Bitout);
  free(list);
}

int EncodeFileLen(int *list, TCode *Code) {
  int N = 0;
  for (int i = 0; i < ALPH; i++) {
    N += list[i] * Code[i].Size;
  }
  return N;
}

int FillList(FILE *in, int *list) {
  int Num;
  Tbits buff[BUFF_SIZE + 1];
  if (!(Num = fread(buff, sizeof(Tbits), BUFF_SIZE, in))) {
    return 0;
  }
  while (Num) {
    for (int i = 0; i < Num; i++) {
      list[buff[i]]++;
    }
    Num = fread(buff, sizeof(Tbits), BUFF_SIZE, in);
  }
  return 1;
}

void Decode(FILE *in, FILE *out) {
  int i = 0;
  Tbits value;
  HTree *Tree = GetTree(in, &i, &value);
  if (!Tree) return;
  DecodeFile(Tree, in, &value, &i, out);
  Destroy(Tree);
}

void DecodeFile(HTree *Tree, FILE *in, Tbits *value, int *i, FILE *out) {
  HTree *tmp = Tree;
  TDecodeValue *val = (TDecodeValue *)calloc(1, sizeof(TDecodeValue));
  val->addInf = GetValue(in, value, i, EXTRA_SIZE);
  val->curs = 0;
  val->temp = fgetc(in);
  val->IsEnded = feof(in);
  val->pos = *i;
  if (val->IsEnded) {
    val->addInf = (val->addInf + val->pos % TBITS_SIZE) % TBITS_SIZE;
  }
  int isCorrect = !val->IsEnded || val->addInf < TBITS_SIZE;
  val->value = *value;
  int flag = 0;
  while (isCorrect) {
    Tree = tmp;
    if (Tree->value != -1) flag = 1;
    while (Tree->value == -1 && isCorrect) {
      if (GetBit(val->value, val->pos) == 0) {
        Tree = Tree->Lchild;
      } else {
        Tree = Tree->Rchild;
      }
      if (val->IsEnded) val->curs += 1;
      isCorrect = MoveSigns(val, in);
    }
    if (Tree->value != -1) fputc(Tree->value, out);
    if (flag) {
      if (val->IsEnded) val->curs += 1;
      isCorrect = MoveSigns(val, in);
    }
  }
  free(val);
}

int MoveSigns(TDecodeValue *value, FILE *in) {
  value->pos += 1;
  if (value->pos % TBITS_SIZE == 0) {
    value->value = value->temp;
    if (!value->IsEnded) value->temp = fgetc(in);
    if (feof(in)) value->IsEnded = 1;
  }
  return !value->IsEnded || value->curs < TBITS_SIZE - value->addInf;
}

HTree *GetTree(FILE *in, int *i, Tbits *value) {
  *value = fgetc(in);
  if (feof(in)) return NULL;
  return Child(in, value, i);
}

HTree *Child(FILE *in, Tbits *value, int *i) {
  HTree *Tree = calloc(1, sizeof(HTree));
  int sign = GetBit(*value, *i);
  *i += 1;
  if (*i % TBITS_SIZE == 0) *value = fgetc(in);
  if (sign == 0) {
    Tree->value = -1;
    Tree->Lchild = Child(in, value, i);
    Tree->Rchild = Child(in, value, i);
  } else {
    Tree->value = GetValue(in, value, i, TBITS_SIZE);
  }
  return Tree;
}

int GetValue(FILE *in, Tbits *value, int *i, int len) {
  Tbits res = (*value << *i % TBITS_SIZE);
  res >>= (TBITS_SIZE - len);
  if (*i % TBITS_SIZE + len >= TBITS_SIZE) {
    *value = fgetc(in);
    res += *value >> (TBITS_SIZE - (len + *i % TBITS_SIZE) % TBITS_SIZE);
  }
  *i += len;
  return res;
}

int GetBit(Tbits value, int i) { return !!(value & 1 << (7 - i % TBITS_SIZE)); }

int CodeTree(HTree *tree, TBitOut *bitout, FILE *out) {
  int N = 0;
  if (tree->Lchild || tree->Rchild) {
    N = MoveTree(tree, bitout, out);
  } else {
    FindLeaf(bitout, 1, tree, &N, out);
  }
  return N;
}

int MoveTree(HTree *tree, TBitOut *bitout, FILE *out) {
  int N = 0;
  if (tree->value == -1) {
    Tbits tmp0 = 0;
    WriteBit(bitout, 1, &tmp0, out);
    N++;
    if (tree->Lchild) {
      N += MoveTree(tree->Lchild, bitout, out);
    }
    if (tree->Rchild) {
      N += MoveTree(tree->Rchild, bitout, out);
    }
  } else {
    FindLeaf(bitout, 1, tree, &N, out);
  }
  return N;
}

void FindLeaf(TBitOut *bitout, Tbits tmp0, HTree *tree, int *N, FILE *out) {
  WriteBit(bitout, 1, &tmp0, out);
  Tbits tmp = (unsigned char)tree->value;
  WriteBit(bitout, TBITS_SIZE, &tmp, out);
  *N += 1 + TBITS_SIZE;
}

void BitWriter(TBitOut *bitout, TCode *Code, FILE *in, FILE *out) {
  fseek(in, 1, SEEK_SET);
  Tbits buff[BUFF_SIZE + 1];
  int K;
  do {
    K = fread(buff, sizeof(Tbits), BUFF_SIZE, in);
    for (int i = 0; i < K; i++) {
      TCode *entryCode = &Code[buff[i]];
      WriteBit(bitout, entryCode->Size, &entryCode->Code, out);
    }
  } while (K);
  if (bitout->Size) fputc(bitout->out, out);
}

void WriteBit(TBitOut *bitout, int size, Tbits *code, FILE *out) {
  int access = TBITS_SIZE - bitout->Size, forWrite;
  Tbits mask;
  while (size > 0) {
    if (size > access) {
      forWrite = access;
      size -= access;
      mask = *code >> size;

    } else {
      forWrite = size;
      size = 0;
      mask = *code << (TBITS_SIZE - forWrite - bitout->Size);
    }
    bitout->Size += forWrite;
    bitout->out = bitout->out | mask;
    if (bitout->Size == TBITS_SIZE) {
      fputc(bitout->out, out);
      bitout->out = 0;
      bitout->Size = 0;
    }
    access = TBITS_SIZE - bitout->Size;
  }
}

void BuildCode(HTree *tree, TCode *Code) {
  TCode cur = {0, 0};
  if (tree->value != -1) {
    cur.Size = 1;
    cur.Code = cur.Code >> 1;
    Code[tree->value] = cur;
  } else {
    WalkTree(tree, Code, &cur);
  }
}

void WalkTree(HTree *tree, TCode *Code, TCode *cur) {
  if (tree->value == -1) {
    if (tree->Lchild) {
      TCode *curl = cur;
      curl->Size = curl->Size + 1;
      curl->Code = cur->Code << 1;
      WalkTree(tree->Lchild, Code, curl);
    }
    if (tree->Rchild) {
      TCode *curr = cur;
      curr->Size += 1;
      curr->Code = cur->Code << 1 | 1;
      WalkTree(tree->Rchild, Code, curr);
    }
  } else {
    Code[tree->value] = *cur;
  }
  cur->Size -= 1;
  cur->Code = cur->Code >> 1;
}

HTree *NewHTree(int value, HTree *next) {
  HTree *tmp = (HTree *)calloc(1, sizeof(HTree));
  tmp->value = value;
  tmp->priority = 1;
  tmp->next = next;
  return tmp;
}

HTree *PriorityQueue(int *list) {
  HTree *Queue = NULL;
  for (int i = 0; i < ALPH; i++) {
    if (!list[i]) continue;
    HTree *NewTree = (HTree *)calloc(1, sizeof(HTree));
    NewTree->value = i;
    NewTree->priority = list[i];
    if (!Queue) {
      Queue = NewTree;
      continue;
    }
    Queue = FindPlace(Queue, NewTree);
  }
  return Queue;
}

HTree *LowerBound(HTree *Queue, int priority) {
  if (Queue->priority >= priority) return NULL;
  HTree *p = Queue;
  for (; p->next && p->next->priority < priority; p = p->next) {
  }
  return p;
}

HTree *FindPlace(HTree *Queue, HTree *NewHTree) {
  HTree *tmp = LowerBound(Queue, NewHTree->priority);
  if (!tmp) {
    NewHTree->next = Queue;
    Queue = NewHTree;
  } else {
    NewHTree->next = tmp->next;
    tmp->next = NewHTree;
  }
  return Queue;
}

HTree *HaffmanTree(HTree *Queue) {
  while (Queue && (Queue)->next) {
    HTree *Rtmp = Queue;
    HTree *Ltmp = Queue->next;
    HTree *tmp = calloc(1, sizeof(HTree));
    tmp->priority = (Queue)->priority + (Queue)->next->priority;
    tmp->value = -1;
    tmp->Rchild = Rtmp;
    tmp->Lchild = Ltmp;
    Queue = Queue->next->next;
    if (!(Queue)) {
      Queue = tmp;
    } else {
      Queue = FindPlace(Queue, tmp);
    }
  }

  return Queue;
}

void Destroy(HTree *tree) {  //  Отчистка
  if (tree->next) tree->next = NULL;
  if (tree->Lchild) Destroy(tree->Lchild);
  if (tree->Rchild) Destroy(tree->Rchild);
  free(tree);
}

