#include "print_file.h"

void PrintFile(FILE *file, Options *options) {
  int c;
  int prevC1 = '\n';
  int lineNumber = 1;
  int countSqeezeBlank = 0;
  while ((c = fgetc(file)) != EOF) {
    int isCharacterChanged = 0;
    if (options->squeezeBlank) {
      if (c == '\n' && prevC1 == '\n')
        ++countSqeezeBlank;
      else {
        countSqeezeBlank = 0;
      }
      if (countSqeezeBlank > 1) continue;
    }
    if (options->numberNonBlank &&
        (c != '\n' && (prevC1 == '\n' || lineNumber == 1))) {
      printf("%6d\t", lineNumber);
      ++lineNumber;
    } else if (options->numberAll && !options->numberNonBlank &&
               (prevC1 == '\n' || lineNumber == 1)) {
      printf("%6d\t", lineNumber);
      ++lineNumber;
    }
    if (options->showTabs && c == '\t') {
      printf("^I");
      isCharacterChanged = 1;
    }
    if (options->showEndStr && c == '\n') {
      printf("$\n");
      isCharacterChanged = 1;
    }
    if (options->showNonPrintable && (c < 32 || c > 126)) {
      PrintNonPrintableChar(c, options);
    } else if (!isCharacterChanged) {
      printf("%c", c);
    }
    prevC1 = c;
  }
}

void PrintNonPrintableChar(int c, Options *options) {
  if (c >= 0 && c <= 31) {
    if ((c == '\n' && !options->showEndStr) ||
        (c == '\t' && !options->showTabs)) {
      printf("%c", c);
    } else if (c != '\n' && c != '\t') {
      printf("^%c", c + 64);
    }
  } else if (c == 127) {
    printf("^?");
  } else {
    printf("%c", c);
  }
}
int IsFileExist(char *fileName) {
  FILE *file;
  int isExist = 0;
  file = fopen(fileName, "rb");
  if (file) {
    fclose(file);
    isExist = 1;
  } else {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", fileName);
  }
  return isExist;
}
void PrintAllInputFiles(int argc, char *argv[], Options *options) {
  FILE *file;
  for (int i = optind; i < argc; i++) {
    if (!strcmp(argv[i], "-")) {
      rewind(stdin);
      PrintFile(stdin, options);
    } else if (IsFileExist(argv[i])) {
      file = fopen(argv[i], "rb");
      PrintFile(file, options);
      fclose(file);
    }
  }
  if (argc == 1 || optind == argc) {
    PrintFile(stdin, options);
  }
}
