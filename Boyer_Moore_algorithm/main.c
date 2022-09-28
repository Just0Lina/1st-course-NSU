#include "main.h"

int main(void) {
  lab1_0();
  return EXIT_SUCCESS;
}

void lab1_0() {
  int pattern[MAXPATLEN], ch, patLen = 0;
  for (; (ch = getchar()) != '\n'; ++patLen) {
    pattern[patLen] = ch;
  }
  int moves[ALPHABETLEN] = {0}, textLen = 0;
  AlphIndex(pattern, moves, patLen);
  int text[MAXSTRLEN];
  for (; textLen < MAXSTRLEN && (ch = getchar()) != EOF; ++textLen) {
    text[textLen] = ch;
  }
  if (textLen > 0) {
    BoyerMoore(text, pattern, moves, patLen, textLen);
  }
}

int TextGet(int text[], int* curs, int* textLen) {
  int tmp = *curs;
  for (*curs = 0; tmp < MAXSTRLEN; *curs += 1, ++tmp) {
    text[*curs] = text[tmp];
  }
  int ch;
  for (; *curs < MAXSTRLEN && (ch = getchar()) != EOF; *curs += 1) {
    text[*curs] = ch;
  }
  if (*curs == MAXSTRLEN - tmp) return 0;
  *textLen = *curs;
  *curs = 0;
  return 1;
}

void AlphIndex(int* pattern, int* moves, int patLen) {
  for (int i = 0; i < 256; ++i) {
    moves[i] = patLen;
  }
  for (int j = 0; j < patLen - 1; ++j) {
    moves[(int)pattern[j]] = patLen - j - 1;
  }
}

void BoyerMoore(int* text, int* pattern, int* moves, int patLen, int textLen) {
  if (textLen < patLen) {
    exit(0);
  }
  int shift = patLen - 1;
  int curs = 0, charInd = 0;
  while (1) {
    if (curs + shift < textLen) {
      curs += shift;
      charInd += shift;
      shift = patLen - 1;
      int i = 0;
      for (; shift >= 0 && pattern[shift] == text[curs - i]; shift--, i++) {
        printf("%d ", charInd - i + 1);
      }
      if (!i || shift != -1) {
        printf("%d ", charInd - i + 1);
      }
      shift = moves[(int)text[curs]];
    } else {
      if (textLen != MAXSTRLEN || !TextGet(text, &curs, &textLen)) exit(0);
    }
  }
}

