#ifndef PRINT_FILE_H_
#define PRINT_FILE_H_
#include <stdio.h>
#include <string.h>

#include "check_options.h"

void PrintNonPrintableChar(int c, Options *options);
int IsFileExist(char *filename);
void PrintFile(FILE *file, Options *options);
void PrintAllInputFiles(int argc, char *argv[], Options *options);
#endif