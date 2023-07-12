#include "vas_grep.h"

#include <regex.h>

#define N_FLAG_LETTERS 10

int main(int argc, char **argv) {
  if (argc == 1) {  // ARGC == 1
    printError();
  } else if (argc == 2 && isFlag(argv[1]) == 1) {  // ARGC == 2
    printError();
  } else {  // ARGC > 2:
    mainLogic(argc, argv);
  }
  return 0;
}

void mainLogic(int argc, char **argv) {
  Flags flags = flags_default;
  int hasIllegFlag = 0;
  int *fileNamesIdxs = NULL;
  int *patternsIdxs = NULL;
  int fileNamesCount = 0;
  int patternsCount = 0;
  char additionalPattern[16384] = {0};
  int additionalPatternsCount = 0;
  fileNamesIdxs = calloc(argc + 1, sizeof(int));
  patternsIdxs = calloc(argc + 1, sizeof(int));
  int patternLoaded = 0;
  // ---------
  // PARSING :
  // ---------
  int i = 1;
  for (; i < argc; i++) {
    if (isFlag(argv[i]) == 1 && match(i, patternsIdxs, patternsCount) == 0) {
      if (isEAndWhatType(argv[i]) == 2) {  // handle complex e like -eaboba
        handleComplexE(argv[i], additionalPattern, additionalPatternsCount);
        additionalPatternsCount++;
        // if flag has a simple e flag, next word is a pattern:
      } else if (flagCheckAndParse(argv[i], &flags) == 2) {
        if (i != argc - 1) {
          patternsIdxs[patternsCount] = i + 1;
          patternsCount++;
          patternLoaded = 1;
        }
      } else if (flagCheckAndParse(argv[i], &flags) == 1) {
        // do nothing, flags are parsed;
      } else if (flagCheckAndParse(argv[i], &flags) == 0) {
        // flag is not legal:
        if (hasIllegFlag == 0) {
          hasIllegFlag = 1;
          printError();
        }
      }
    } else {  // if its -e pattern - dont count string as a file name:
      if (match(i, patternsIdxs, patternsCount) == 0) {
        // if there's no -e then first non-flag is a pattern:
        if (isThereAnEInArgv(argv, argc) == 0 && patternLoaded == 0) {
          patternsIdxs[patternsCount] = i;
          patternsCount++;
          patternLoaded = 1;
        } else {  // if not a flag then filename goes into the file list:
          fileNamesIdxs[fileNamesCount] = i;
          fileNamesCount++;
        }
      }
    }
  }
  // ----------
  // PRINTING :
  // ----------
  if (hasIllegFlag == 0) {
    for (int i = 0; i < fileNamesCount; i++) {
      if (fileExists(argv[fileNamesIdxs[i]])) {
        int isMultipleFiles = 0;
        if (fileNamesCount > 1) {
          isMultipleFiles = 1;
        }
        searchAndOutputPatternsInAFile(argv, patternsIdxs, patternsCount,
                                       argv[fileNamesIdxs[i]], flags,
                                       isMultipleFiles, additionalPattern);
      } else {
        // no such file:
        if (flags.s != 1) {  // FLAG S
          noSuch(argv[fileNamesIdxs[i]]);
        }
      }
    }
  }
  if (fileNamesIdxs != NULL) {
    free(fileNamesIdxs);
  }
  if (patternsIdxs != NULL) {
    free(patternsIdxs);
  }
}

void handleComplexE(char *argvin, char additionalPatternIn[16384],
                    int additionalPatternsCountIn) {
  char tempStrForEParse[16384] = {0};
  parseComplexE(argvin, tempStrForEParse);
  if (additionalPatternsCountIn == 0) {  // no pipe needed
    concatPattern(additionalPatternIn, tempStrForEParse, 0);
  } else {
    concatPattern(additionalPatternIn, tempStrForEParse, -1);
  }
}

void parseComplexE(char *input, char tempStrIn[1024]) {
  int i = 2;
  while (input[i] != '\0' || i < 1014) {
    tempStrIn[i - 2] = input[i];
    i++;
  }
  tempStrIn[i - 1] = '\0';
}

int match(int inputInt, int *inputIntArr, int n) {
  int match = 0;
  for (int i = 0; i < n; i++) {
    if (inputIntArr[i] == inputInt) {
      match = 1;
    }
  }
  return match;
}

void noSuch(char *argvi) {
  printf("grep: %s: No such file or directory\n", argvi);
}

void illegOption(char argvOneOne) {
  printf("illegal option -- %c\n", argvOneOne);
}

int isFlag(char *input) {
  int returnValue = 0;
  if (input[0] == '-') {
    returnValue = 1;
  }
  return returnValue;
}

int fileExists(const char *fileName) {
  int returnValue = 0;
  FILE *tempFile = NULL;
  if ((tempFile = fopen(fileName, "r"))) {
    fclose(tempFile);
    returnValue = 1;
  }
  return returnValue;
}

void searchAndOutputPatternsInAFile(char **argvIn, int *patternsIdxsIn,
                                    int patternsCountIn, char *inFileName,
                                    Flags flagsIn, int isMultiFilesIn,
                                    char additionalPattern[16384]) {
  FILE *inputFile = fopen(inFileName, "rb");
  char curStr[16384] = {0};
  char combinedPattern[16384] = {0};
  // -----------------------------
  // COMBINING PATTERNS FOR REGEX:
  // -----------------------------
  if (patternsCountIn > 0) {
    concatPatternsFromArgv(combinedPattern, argvIn, patternsIdxsIn,
                           patternsCountIn);
  }
  if (strlen(additionalPattern) > 0 && patternsCountIn > 0) {
    concatPattern(combinedPattern, additionalPattern, -1);
  } else if (strlen(additionalPattern) > 0 && patternsCountIn == 0) {
    concatPattern(combinedPattern, additionalPattern, 0);
  }
  regex_t regexedPattern = {0};
  int cFlag = REG_EXTENDED;
  if (flagsIn.i == 1) {
    cFlag = REG_ICASE + REG_EXTENDED;
  }
  int regexReturnVal = 0;
  regexReturnVal = regcomp(&regexedPattern, combinedPattern, cFlag);
  // --------------------------------------------
  // SEARCHING AND PRINTING MATCHES LINE BY LINE:
  // --------------------------------------------
  int matchCount = 0;
  int lineCount = 0;
  if (regexReturnVal == 0) {  // if no regex errors
    while (fgets(curStr, 16384, inputFile)) {
      regexReturnVal = regexec(&regexedPattern, curStr, 0, NULL, 0);
      if (curStr[strlen(curStr) - 1] == '\n') {
        curStr[strlen(curStr) - 1] = '\0';  // cleaning new line for puts
      }
      if (flagsIn.v != 1) {
        if (regexReturnVal == 0) {  // REGEX MATCHED
          smartPrint(curStr, flagsIn, isMultiFilesIn, inFileName,
                     lineCount + 1);
          matchCount++;
        }
      } else {  // FLAG V
        if (regexReturnVal == REG_NOMATCH) {
          smartPrint(curStr, flagsIn, isMultiFilesIn, inFileName,
                     lineCount + 1);
          matchCount++;
        }
      }
      lineCount++;
    }
    // --------------
    // FLAGS C AND L:
    // --------------
    if (flagsIn.c == 1 && flagsIn.l == 1) {
      if (flagsIn.l == 1) {  // FLAGS L+C combo
        if (isMultiFilesIn == 1) {
          if (matchCount > 0) {
            printf("%s:1\n", inFileName);
            puts(inFileName);
          } else {  // no matches found
            printf("%s:0\n", inFileName);
          }
        } else {  // l+c combo for only one file
          if (matchCount > 0) {
            printf("1\n");
            puts(inFileName);
          } else {  // no matches found
            printf("0\n");
          }
        }
      }
    } else if (flagsIn.c == 1) {  // FLAG C
      if (isMultiFilesIn == 1) {  // just flag c with multiple files
        printf("%s:", inFileName);
        printf("%i\n", matchCount);
      } else {  // just flag c for only one file
        printf("%i\n", matchCount);
      }
    } else if (flagsIn.l == 1 && matchCount > 0) {  // FLAG L
      puts(inFileName);
    }
  } else {
    printf("INCORRECT REGEX");
  }
  regfree(&regexedPattern);
  fclose(inputFile);
}

void smartPrint(char *input, Flags flagsIn, int isMultipleFilesIn,
                char *fileNameIn, int matchCount) {
  char tempStr[16384] = {0};
  if (flagsIn.c != 1 && flagsIn.l != 1) {
    if (isMultipleFilesIn == 1) {
      if (flagsIn.h != 1) {  // FLAG H
        strcat(tempStr, fileNameIn);
        strcat(tempStr, ":");
        printf("%s", tempStr);
        if (flagsIn.n == 1) {  // FLAG N
          printf("%i:", matchCount);
        }
        printf("%s\n", input);
      } else {
        if (flagsIn.n == 1) {  // FLAG N
          printf("%i:", matchCount);
          printf("%s\n", input);
        } else {
          puts(input);
        }
      }
    } else {
      if (flagsIn.n == 1) {  // FLAG N
        printf("%i:", matchCount);
      }
      printf("%s\n", input);
    }
  }
}

int isThereAnEInArgv(char **argvin, int argcin) {
  int yesThereIs = 0;
  for (int i = 1; i < argcin; i++) {
    if (isEAndWhatType(argvin[i]) == 1 || isEAndWhatType(argvin[i]) == 2) {
      yesThereIs = 1;
    }
  }
  return yesThereIs;
}

void concatPatternsFromArgv(char concatTo[16384], char **argvIn,
                            int *patternsIdxs, int patternsCountIn) {
  for (int i = 0; i < patternsCountIn; i++) {
    strcat(concatTo, argvIn[patternsIdxs[i]]);
    if (i != patternsCountIn - 1) {
      strcat(concatTo, "|");
    }
  }
}

void concatPattern(char concatTo[16384], char *pattern, int pipePosition) {
  // if pipePosition is 0, then no pipe will be added
  if (pipePosition == -1) {
    strcat(concatTo, "|");
  }
  strcat(concatTo, pattern);
  if (pipePosition == 1) {
    strcat(concatTo, "|");
  }
}

int checkCharFlagLegality(char inputChar) {
  int isLegal = 0;
  // legalFlagLetters:
  const char legalFlags[10] = {'e', 'i', 'v', 'c', 'l',
                               'n', 'h', 's', 'f', 'o'};
  for (int i = 0; i < N_FLAG_LETTERS; i++) {
    if (inputChar == legalFlags[i]) {
      isLegal = 1;
    }
  }
  return isLegal;
}

int isEAndWhatType(char *input) {
  int returnValue = 0;
  if (input[0] == '-' && input[1] == 'e' && input[2] == '\0') {
    returnValue = 1;  // is pure e
  }
  if (input[0] == '-' && input[1] == 'e' && input[2] != '\0') {
    returnValue = 2;  // is not pure e
  }
  return returnValue;
}

int flagCheckAndParse(char *input, Flags *flagsIn) {
  int isLegal = 0;
  int whileBreak = 0;
  int i = 1;
  int isAE = 0;
  while (input[i] != '\0' && whileBreak == 0) {
    isLegal = checkCharFlagLegality(input[i]);
    if (isLegal == 0) {
      whileBreak = 1;
      // flag char is legal:
    } else {
      if (input[i] == 'e') {
        flagsIn->e = 1;
        isAE = 1;
      } else if (input[i] == 'i') {
        flagsIn->i = 1;
      } else if (input[i] == 'v') {
        flagsIn->v = 1;
      } else if (input[i] == 'c') {
        flagsIn->c = 1;
      } else if (input[i] == 'l') {
        flagsIn->l = 1;
      } else if (input[i] == 'n') {
        flagsIn->n = 1;
      } else if (input[i] == 'h') {
        flagsIn->h = 1;
      } else if (input[i] == 's') {
        flagsIn->s = 1;
      } else if (input[i] == 'f') {
        flagsIn->f = 1;
      } else if (input[i] == 'o') {
        flagsIn->o = 1;
      }
      i++;
    }
  }
  if (isLegal == 1 && isAE == 1) {
    isLegal = 2;  // return 2 means it's an e
  } else {
    isLegal = 1;  // return 1
  }
  return isLegal;
}

void printError() {
  printf(
      "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
      "[-C[num]]\n");
  printf(
      "[-e pattern] [-f file] [--binary-files=value] "
      "[--color=when]\n[--context[=num]] [--directories=action] [--label] "
      "[--line-buffered]\n");
  printf("[--null] [pattern] [file ...]");
}
