#ifndef CHECK_OPTIONS_H_
#define CHECK_OPTIONS_H_
#include <getopt.h>
#include <stdio.h>

typedef struct {
  int numberNonBlank, showEndStr, numberAll, squeezeBlank, showTabs,
      showNonPrintable;
} Options;

Options OptionsInit();
int isOption(char *str);
int CheckOptions(int argc, char *argv[], Options *options);

#endif