#ifndef SRC_GREP_VAS_GREP_H_
#define SRC_GREP_VAS_GREP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flagsStruct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} flags_default = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

typedef struct flagsStruct Flags;

void mainLogic(int argc, char **argv);
void handleComplexE(char *argvin, char additionalPatternIn[16384],
                    int additionalPatternsCountIn);
void parseComplexE(char *input, char tempStrIn[1024]);
int match(int inputInt, int *inputIntArr, int n);
void noSuch(char *argvi);
void illegOption(char argvOneOne);
int isFlag(char *input);
int fileExists(const char *fileName);
void searchAndOutputPatternsInAFile(char **argvIn, int *patternsIdxsIn,
                                    int patternsCountIn, char *inFileName,
                                    Flags flagsIn, int isMultiFilesIn,
                                    char additionalPattern[16384]);
void smartPrint(char *input, Flags flagsIn, int isMultipleFilesIn,
                char *fileNameIn, int matchCount);
int isThereAnEInArgv(char **argvin, int argcin);
void concatPatternsFromArgv(char concatTo[16384], char **argvIn,
                            int *patternsIdxs, int patternsCountIn);
void concatPattern(char concatTo[16384], char *pattern, int pipePosition);
int checkCharFlagLegality(char inputChar);
int isEAndWhatType(char *input);
int flagCheckAndParse(char *input, Flags *flagsIn);
void printError();

#endif  // SRC_GREP_VAS_GREP_H_
