#ifndef PRINT_FILE_H_
#define PRINT_FILE_H_

#include "check_options.h"

void PrintNonPrintableChar(int character);

void GrepFiles(int argc, char* argv[], Options* options, Patterns* patterns);
void GrepFile(FILE* file, char* fileName, int isManyFiles, Options* options,
              Patterns* patterns);
void GrepPrintFile(FILE* file, char* fileName, int isManyFiles,
                   Options* options, Patterns* patterns);
void GrepPrintFileMatched(FILE* file, char* fileName, int isManyFiles,
                          Options* options, Patterns* patterns);
void GrepPrintMatchesWordsFile(FILE* file, char* fileName, int isManyFiles,
                               Options* options, Patterns* patterns);
void GrepPrintMathesWordsLine(Patterns* patterns, int currentIndexPreg,
                              char* fileName, int isManyFiles, int numberLine,
                              regex_t* preg, regmatch_t* match,
                              int* isPrintFileName, int* isPrintNumberLine,
                              char** currentLine);
int IsMatchLine(Patterns* patterns, regmatch_t* match, char* line);
int IsNotMatchLine(Patterns* patterns, regmatch_t* match, char* line);

void PrintLine(char* fileName, int isManyFiles, int numberLine, char* line,
               int lengthInt);
void PrintBaseMatchLine(Patterns* patterns, char* fileName, int isManyFiles,
                        regmatch_t* match, char* line, int numberLine,
                        int lengthInt);
void PrintInvertMatchLine(Patterns* patterns, char* fileName, int isManyFiles,
                          regmatch_t* match, char* line, int numberLine,
                          int lengthInt);
void ProcessCountMatched(Patterns* patterns, regmatch_t* match, char* line,
                         int* countMatched);
void ProcessCountMatchedInvert(Patterns* patterns, regmatch_t* match,
                               char* line, int* countMatched);
void PrintCountMatchLines(char* fileName, int isManyFiles, int countMatched,
                          int isFileMatch);
char* FormatMatchedString(const char* currentLine, regmatch_t* match);
void GrepPrintMathesWordsInPattern(Patterns* patterns, char* currentLine,
                                   int currentIndexPreg);
#endif