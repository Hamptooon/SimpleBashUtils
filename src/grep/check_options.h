#ifndef CHECK_OPTIONS_H_
#define CHECK_OPTIONS_H_
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int pattern, regexFlag, invert, count, fileMatch, numberLine, notFileName,
      notPrintFileError, filePatterns, printMatched;
  char* patterns;
} Options;
typedef struct {
  int countPregs;
  regex_t* pregs;
} Patterns;

Options OptionsInit();
Patterns PatternsInit();
int CheckOptions(int argc, char* argv[], Options* options, Patterns* patterns);
void AddPattern(char* patternStr, Options* options, int* countPregs,
                regex_t** pregs);
int AddPatternsFromFile(char* fileName, Options* options, size_t* arraySize,
                        char*** strArray);

int IsFileExist(char* fileName, int isNotPrintError);
void AddPatternToStrArray(char* patternStr, char*** strArray,
                          size_t* arraySize);

void FreePatterns(regex_t** pregs, int countPregs);
#endif