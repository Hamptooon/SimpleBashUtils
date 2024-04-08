#include "print_file.h"

void GrepFile(FILE* file, char* fileName, int isManyFiles, Options* options,
              Patterns* patterns) {
  if (options->fileMatch) {
    GrepPrintFileMatched(file, fileName, isManyFiles, options, patterns);
  } else if (options->printMatched && !options->count) {
    GrepPrintMatchesWordsFile(file, fileName, isManyFiles, options, patterns);
  } else {
    GrepPrintFile(file, fileName, isManyFiles, options, patterns);
  }
}
void GrepPrintFile(FILE* file, char* fileName, int isManyFiles,
                   Options* options, Patterns* patterns) {
  char* line = 0;
  size_t length = 0;
  int lengthInt = 0;
  int numberLine = 0;
  int countMatched = 0;
  regmatch_t match;
  while ((lengthInt = getline(&line, &length, file)) != -1) {
    if (options->numberLine) ++numberLine;
    if (options->invert) {
      if (options->count) {
        ProcessCountMatchedInvert(patterns, &match, line, &countMatched);
      } else {
        PrintInvertMatchLine(patterns, fileName, isManyFiles, &match, line,
                             numberLine, lengthInt);
      }
    } else {
      if (options->count) {
        ProcessCountMatched(patterns, &match, line, &countMatched);
      } else {
        PrintBaseMatchLine(patterns, fileName, isManyFiles, &match, line,
                           numberLine, lengthInt);
      }
    }
  }

  if (options->count) {
    PrintCountMatchLines(fileName, isManyFiles, countMatched,
                         options->fileMatch);
  }
  free(line);
}
void GrepPrintMatchesWordsFile(FILE* file, char* fileName, int isManyFiles,
                               Options* options, Patterns* patterns) {
  char* line = 0;
  size_t length = 0;
  int numberLine = 0;
  regmatch_t match;
  while ((getline(&line, &length, file)) != -1) {
    if (options->numberLine) ++numberLine;

    int isPrintFileName = 0;
    int isPrintNumberLine = 0;
    char* currentLine = line;
    for (int i = 0; i < patterns->countPregs; i++) {
      GrepPrintMathesWordsLine(patterns, i, fileName, isManyFiles, numberLine,
                               &(patterns->pregs[i]), &match, &isPrintFileName,
                               &isPrintNumberLine, &currentLine);
    }
  }
  fclose(file);
  free(line);
}

void GrepPrintMathesWordsLine(Patterns* patterns, int currentIndexPreg,
                              char* fileName, int isManyFiles, int numberLine,
                              regex_t* preg, regmatch_t* match,
                              int* isPrintFileName, int* isPrintNumberLine,
                              char** currentLine) {
  while (!regexec(preg, *currentLine, 1, match, 0)) {
    if (isManyFiles && !(*isPrintFileName)) {
      printf("%s:", fileName);
      *isPrintFileName = 1;
    }
    if (numberLine != 0 && !(*isPrintNumberLine)) {
      printf("%d:", numberLine);
      *isPrintNumberLine = 1;
    }
    printf("%.*s\n", (int)(match->rm_eo - match->rm_so),
           &(*currentLine)[match->rm_so]);
    char* copyCurrentLine = strdup(*currentLine);
    char* currentFindPattern = FormatMatchedString(copyCurrentLine, match);
    GrepPrintMathesWordsInPattern(patterns, currentFindPattern,
                                  currentIndexPreg);
    free(currentFindPattern);
    free(copyCurrentLine);
    *currentLine = &(*currentLine)[match->rm_eo];
  }
}
char* FormatMatchedString(const char* currentLine, regmatch_t* match) {
  size_t length = match->rm_eo - match->rm_so;

  char* formattedString = malloc(length + 1);

  if (formattedString != NULL) {
    snprintf(formattedString, length + 1, "%.*s", (int)length,
             &currentLine[match->rm_so]);
  }

  return formattedString;
}
void GrepPrintMathesWordsInPattern(Patterns* patterns, char* currentLine,
                                   int currentIndexPreg) {
  regmatch_t match;
  for (int i = currentIndexPreg + 1; i < patterns->countPregs; i++) {
    while (!regexec(&(patterns->pregs[i]), currentLine, 1, &match, 0)) {
      printf("%.*s\n", (int)((&match)->rm_eo - (&match)->rm_so),
             &(currentLine)[(&match)->rm_so]);
      currentLine = &(currentLine)[(&match)->rm_eo];
    }
  }
}

void GrepPrintFileMatched(FILE* file, char* fileName, int isManyFiles,
                          Options* options, Patterns* patterns) {
  char* line = 0;
  size_t length = 0;
  int countMatched = 0;
  int isMatchFile = 0;
  int isNotMatchFile = 0;
  regmatch_t match;
  while ((getline(&line, &length, file)) != -1) {
    if (isMatchFile == 0) {
      isMatchFile = IsMatchLine(patterns, &match, line);
    }
    if (isNotMatchFile == 0) {
      isNotMatchFile = IsNotMatchLine(patterns, &match, line);
    }

    if (options->invert) {
      if (options->count) {
        ProcessCountMatchedInvert(patterns, &match, line, &countMatched);
      }

    } else {
      if (options->count) {
        ProcessCountMatched(patterns, &match, line, &countMatched);
      }
    }
  }
  if (options->count) {
    PrintCountMatchLines(fileName, isManyFiles, countMatched,
                         options->fileMatch);
  }
  if (options->invert && isNotMatchFile) {
    printf("%s\n", fileName);
  } else if (!options->invert && isMatchFile) {
    printf("%s\n", fileName);
  }

  free(line);
}
int IsMatchLine(Patterns* patterns, regmatch_t* match, char* line) {
  int isMatchLine = 0;
  for (int i = 0; i < patterns->countPregs; i++) {
    if (!regexec(&(patterns->pregs[i]), line, 1, match, 0)) {
      isMatchLine = 1;
      break;
    }
  }
  return isMatchLine;
}
int IsNotMatchLine(Patterns* patterns, regmatch_t* match, char* line) {
  int isNotMatchLine = 0;
  for (int i = 0; i < patterns->countPregs; i++) {
    if (regexec(&(patterns->pregs[i]), line, 1, match, 0)) {
      isNotMatchLine = 1;
      break;
    }
  }
  return isNotMatchLine;
}
void PrintBaseMatchLine(Patterns* patterns, char* fileName, int isManyFiles,
                        regmatch_t* match, char* line, int numberLine,
                        int lengthInt) {
  for (int i = 0; i < patterns->countPregs; i++) {
    if (!regexec(&(patterns->pregs[i]), line, 1, match, 0)) {
      PrintLine(fileName, isManyFiles, numberLine, line, lengthInt);
      break;
    }
  }
}
void PrintInvertMatchLine(Patterns* patterns, char* fileName, int isManyFiles,
                          regmatch_t* match, char* line, int numberLine,
                          int lengthInt) {
  int isMatchedLine = 0;
  for (int i = 0; i < patterns->countPregs; i++) {
    if (!regexec(&(patterns->pregs[i]), line, 1, match, 0)) {
      isMatchedLine = 1;
      break;
    }
  }
  if (!isMatchedLine) {
    PrintLine(fileName, isManyFiles, numberLine, line, lengthInt);
  }
}
void ProcessCountMatched(Patterns* patterns, regmatch_t* match, char* line,
                         int* countMatched) {
  for (int i = 0; i < patterns->countPregs; i++) {
    if (!regexec(&(patterns->pregs[i]), line, 1, match, 0)) {
      ++(*countMatched);
      break;
    }
  }
}
void ProcessCountMatchedInvert(Patterns* patterns, regmatch_t* match,
                               char* line, int* countMatched) {
  int isMatchedLine = 0;
  for (int i = 0; i < patterns->countPregs; i++) {
    if (!regexec(&(patterns->pregs[i]), line, 1, match, 0)) {
      isMatchedLine = 1;
      break;
    }
  }
  if (!isMatchedLine) {
    ++(*countMatched);
  }
}
void PrintCountMatchLines(char* fileName, int isManyFiles, int countMatched,
                          int isFileMatch) {
  if (isManyFiles) {
    printf("%s:", fileName);
  }
  if (isFileMatch && countMatched > 0) {
    printf("%d\n", 1);
  } else {
    printf("%d\n", countMatched);
  }
}
void PrintLine(char* fileName, int isManyFiles, int numberLine, char* line,
               int lengthInt) {
  if (isManyFiles) {
    printf("%s:", fileName);
  }
  if (numberLine != 0) {
    printf("%d:", numberLine);
  }
  printf("%s", line);
  if (line[lengthInt - 1] != '\n') {
    printf("\n");
  }
}

void GrepFiles(int argc, char* argv[], Options* options, Patterns* patterns) {
  if (argc == 1 ||
      (!argv[optind] && !options->pattern && !options->filePatterns)) {
    fprintf(stderr, "Usage: s21_grep [OPTION]... PATTERNS [FILE]...\n");
  } else {
    FILE* file;
    int isManyFiles = 0;
    int filesCount = 0;
    if (options->pattern == 0 && options->filePatterns == 0) {
      AddPattern(argv[optind], options, &(patterns->countPregs),
                 &(patterns->pregs));
    }
    for (int i = (options->pattern == 1 || options->filePatterns == 1)
                     ? optind
                     : optind + 1;
         i < argc; i++)
      ++filesCount;

    if (filesCount >= 2) isManyFiles = options->notFileName ? 0 : 1;

    for (int i = (options->pattern || options->filePatterns) == 1 ? optind
                                                                  : optind + 1;
         i < argc; i++) {
      if (!strcmp(argv[i], "-")) {
        rewind(stdin);
        GrepFile(stdin, "(standart-input)", isManyFiles, options, patterns);

      } else if (IsFileExist(argv[i], options->notPrintFileError)) {
        file = fopen(argv[i], "rb");
        GrepFile(file, argv[i], isManyFiles, options, patterns);
        fclose(file);
      }
    }
    if ((optind + 1 == argc && !options->pattern && !options->filePatterns) ||
        (optind == argc && patterns->pregs &&
         (options->pattern || options->filePatterns))) {
      GrepFile(stdin, "(standart-input)", isManyFiles, options, patterns);
    }
    FreePatterns(&(patterns->pregs), patterns->countPregs);
  }
}
