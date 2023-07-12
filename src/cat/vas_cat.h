#ifndef SRC_CAT_VAS_CAT_H_
#define SRC_CAT_VAS_CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flagsStruct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} flags_default = {0, 0, 0, 0, 0, 0};

typedef struct flagsStruct Flags;

void noSuch(char *argvi);
void illegOption(char argvOneOne);
int isFlag(char *input);
int fileExists(const char *fileName);
void myPrintFile(char *argvin, Flags flagsIn);
void myPrintStrWithFlags(int *tempStrIn, Flags flagsIn,
                         int *currentLineIsEmptyIn, int *lineCountIn,
                         int isLastLineIn);
int checkCharFlagLegality(char inputChar);
int flagCheckAndParse(char *input, char *illegalCharIn, Flags *flagsIn);
void myPrintStr(int *inputStr, int vBool, int tabBool);
char *decodeUnprintable(int c);
int myStrLen(int *inArray);
int charSlide(char inputInt);

#endif  // SRC_CAT_VAS_CAT_H_
