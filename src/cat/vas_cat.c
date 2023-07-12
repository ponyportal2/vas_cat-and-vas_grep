#include "vas_cat.h"

#define N_FLAG_LETTERS 8
#define N_DDFLAGS 3

int main(int argc, char **argv) {
  Flags flags = flags_default;
  char badChar = '-';
  int hasIllegFlag = 0;

  if (argc == 2 && isFlag(argv[1]) == 0) {
    if (fileExists(argv[1])) {
      myPrintFile(argv[1], flags);
    } else {  // no such file:
      noSuch(argv[1]);
    }
  } else {  // ARGC > 2:
    int *fileNamesIdxs = NULL;
    fileNamesIdxs = calloc(argc + 1, sizeof(char));  // for safety
    int fileNamesCount = 0;
    for (int i = 1; i < argc; i++) {
      if (isFlag(argv[i]) == 1) {  // if is a flag:
        if (flagCheckAndParse(argv[i], &badChar, &flags) == 1) {
          // if flag is legal then do nothing, flags are parsed
        } else {  // flag is not legal:
          if (hasIllegFlag == 0) {
            illegOption(badChar);
            hasIllegFlag = 1;
          }
        }
      } else {  // if not a flag then filename goes into the file list:
        fileNamesIdxs[fileNamesCount] = i;
        fileNamesCount++;
      }
    }
    // NEXT STEP, PRINTING STUFF:
    if (hasIllegFlag == 0) {
      for (int i = 0; i < fileNamesCount; i++) {
        if (fileExists(argv[fileNamesIdxs[i]])) {
          myPrintFile(argv[fileNamesIdxs[i]], flags);
        } else {  // no such file:
          noSuch(argv[fileNamesIdxs[i]]);
        }
      }
    }

    if (fileNamesIdxs != NULL) {
      free(fileNamesIdxs);
    }
  }
  return 0;
}

void noSuch(char *argvi) {
  printf("cat: %s: No such file or directory\n", argvi);
}

void illegOption(char argvOneOne) {
  printf("cat: illegal option -- %c\nusage: cat [-benstuv] [file ...]\n",
         argvOneOne);
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
  FILE *tempFile;
  if ((tempFile = fopen(fileName, "r"))) {
    fclose(tempFile);
    returnValue = 1;
  }
  return returnValue;
}

void myPrintFile(char *argvin, Flags flagsIn) {
  int lastLineWasEmpty = 0;
  FILE *inputFile = fopen(argvin, "rb");
  int tempStr[16384] = {0};
  char curChar = '*';
  int lineCount = 0;
  int isLastLine = 0;
  do {
    int i = 0;
    do {
      curChar = fgetc(inputFile);
      if (i < 16374) {
        tempStr[i] = charSlide(curChar);
      }
      if (curChar == '\n' || curChar == EOF) {
        tempStr[i + 1] = -340;
        if (curChar == EOF) {
          isLastLine = 1;
        }
      }
      i++;
    } while (curChar != '\n' && curChar != EOF);

    int currentLineIsEmpty = 0;
    if (myStrLen(tempStr) == 1 && (tempStr[0] == '\n' || tempStr[0] == EOF)) {
      currentLineIsEmpty = 1;
    }

    if (flagsIn.s == 1 && lastLineWasEmpty == 1 && currentLineIsEmpty == 1) {
      // do nothing
    } else if (currentLineIsEmpty == 1 && isLastLine == 1) {
      // do nothing
    } else {
      myPrintStrWithFlags(tempStr, flagsIn, &currentLineIsEmpty, &lineCount,
                          isLastLine);
    }

    lastLineWasEmpty = currentLineIsEmpty;
  } while (curChar != EOF);
  fclose(inputFile);
}

void myPrintStrWithFlags(int *tempStrIn, Flags flagsIn,
                         int *currentLineIsEmptyIn, int *lineCountIn,
                         int isLastLineIn) {
  int localLen = myStrLen(tempStrIn);

  if (flagsIn.b == 1) {  // FLAG B
    if (*currentLineIsEmptyIn == 1) {
      // do nothing
    } else {
      printf("%6i\t", *lineCountIn + 1);
      *lineCountIn = *lineCountIn + 1;
    }
  } else if (flagsIn.n == 1) {  // FLAG N
    printf("%6i\t", *lineCountIn + 1);
    *lineCountIn = *lineCountIn + 1;
  }

  if (flagsIn.e == 1 && localLen > 0 && isLastLineIn != 1) {  // FLAG E
    char tempChar = '-';
    tempChar = tempStrIn[localLen - 1];
    tempStrIn[localLen + 1] = -340;
    tempStrIn[localLen - 1] = '$';
    tempStrIn[localLen] = tempChar;
  }

  myPrintStr(tempStrIn, flagsIn.v, flagsIn.t);
}

int checkCharFlagLegality(char inputChar) {
  int isLegal = 0;
  // legalFlagLetters:
  const char lfl[8] = {'b', 'e', 'n', 's', 't', 'v', 'E', 'T'};
  for (int i = 0; i < N_FLAG_LETTERS; i++) {
    if (inputChar == lfl[i]) {
      isLegal = 1;
    }
  }
  return isLegal;
}

int flagCheckAndParse(char *input, char *illegalCharIn, Flags *flagsIn) {
  // legalDoubleDashFlags:
  const char *lddf[3] = {"--number-nonblank", "--number", "--squeeze-blank"};
  int isLegal = 0;
  int whileBreak = 0;
  // flag is double dash:
  if (input[1] == '-') {
    for (int i = 0; i < N_DDFLAGS; i++) {
      if (strcmp(input, lddf[i]) == 0) {
        isLegal = 1;
        // "--number-nonblank":
        if (strcmp(input, lddf[0]) == 0) {
          flagsIn->b = 1;
          // "--number":
        } else if (strcmp(input, lddf[1]) == 0) {
          flagsIn->n = 1;
          // "--squeeze-blank":
        } else if (strcmp(input, lddf[2]) == 0) {
          flagsIn->s = 1;
        }
      }
    }
    // flag is not double dash:
  } else {
    int i = 1;
    while (input[i] != '\0' && whileBreak == 0) {
      isLegal = checkCharFlagLegality(input[i]);
      if (isLegal == 0) {
        whileBreak = 1;
        *illegalCharIn = input[i];
      } else {  // flag char is legal
        if (input[i] == 'b') {
          flagsIn->b = 1;
        } else if (input[i] == 'e') {
          flagsIn->e = 1;
          flagsIn->v = 1;
        } else if (input[i] == 'E') {
          flagsIn->e = 1;
        } else if (input[i] == 'n') {
          flagsIn->n = 1;
        } else if (input[i] == 's') {
          flagsIn->s = 1;
        } else if (input[i] == 't') {
          flagsIn->t = 1;
          flagsIn->v = 1;
        } else if (input[i] == 'T') {
          flagsIn->t = 1;
        } else if (input[i] == 'v') {
          flagsIn->v = 1;
        }
        i++;
      }
    }
  }
  if (flagsIn->b == 1 && flagsIn->n == 1) {
    flagsIn->n = 0;  // negating n if b present
  }
  return isLegal;
}

void myPrintStr(int *inputStr, int vBool, int tabBool) {
  int i = 0;
  do {
    if (inputStr[i] == '\t' && tabBool == 1) {  // FLAG T
      printf("^I");
    } else if (vBool == 1 && inputStr[i] < 32) {  // FLAG V
      if (inputStr[i] == '\n') {
        printf("\n");
      } else if (inputStr[i] == -200) {
        printf("^@");
      } else if (inputStr[i] == -73) {
        printf("^?");
      } else if (inputStr[i] == -1) {
        // do nothing
      } else {
        printf("%s", decodeUnprintable(inputStr[i]));
      }
    } else if (inputStr[i] < -10) {
      printf("%c", inputStr[i] + 200);
    } else if (inputStr[i] == -1) {
      // do nothing
    } else {
      printf("%c", inputStr[i]);
    }
    i++;
  } while (inputStr[i] != -340);
}

char *decodeUnprintable(int c) {
  char *unprintables[32] = {"^@", "^A", "^B", "^C", "^D",  "^E", "^F", "^G",
                            "^H", "\t", "^M", "^K", "^L",  "^M", "^N", "^O",
                            "^P", "^Q", "^R", "^S", "^T",  "^U", "^V", "^W",
                            "^X", "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_"};
  return unprintables[c];
}

int myStrLen(int *inArray) {
  int i = 0;
  int returnValue = 0;
  while (inArray[i] != -340) {
    i++;
    returnValue = i;
  }
  return returnValue;
}

int charSlide(char inputInt) {
  int returnValue = 0;
  if (inputInt == 0) {
    returnValue = inputInt - 200;
  } else if (inputInt == 127) {
    returnValue = inputInt - 200;
  } else {
    returnValue = inputInt;
  }
  return returnValue;
}
