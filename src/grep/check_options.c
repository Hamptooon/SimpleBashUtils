#include "check_options.h"

Options OptionsInit() {
  Options options = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  return options;
}
Patterns PatternsInit() {
  Patterns patterns = {0, NULL};
  return patterns;
}
int CheckOptions(int argc, char* argv[], Options* options, Patterns* patterns) {
  char** strPatterns = NULL;
  size_t strPatternsSize = 0;
  char** strPatternsE = NULL;
  size_t strPatternsESize = 0;
  int currentOption = getopt_long(argc, argv, "ie:vclnhsf:o", NULL, NULL);
  int isError = 0;

  while (!isError && currentOption != -1) {
    switch (currentOption) {
      case 'e':
        options->pattern = 1;
        if (strcmp(optarg, ".") == 0) {
          options->regexFlag += REG_NEWLINE;
        }
        AddPatternToStrArray(optarg, &strPatternsE, &strPatternsESize);
        break;
      case 'i':
        options->regexFlag += REG_ICASE;
        break;
      case 'v':
        options->invert = 1;
        break;
      case 'c':
        options->count = 1;
        break;
      case 'l':
        options->fileMatch = 1;
        break;
      case 'n':
        options->numberLine = 1;
        break;
      case 'h':
        options->notFileName = 1;
        break;
      case 's':
        options->notPrintFileError = 1;
        break;
      case 'f':
        options->filePatterns = 1;
        isError = AddPatternsFromFile(optarg, options, &strPatternsSize,
                                      &strPatterns);
        break;
      case 'o':
        options->printMatched = 1;
        break;
      default:
        isError = 1;
        continue;
        break;
    }
    currentOption = getopt_long(argc, argv, "ie:vclnhsf:o", NULL, NULL);
  }
  if (options->invert == 1) options->printMatched = 0;
  for (size_t i = 0; i < strPatternsSize; i++) {
    AddPattern(strPatterns[i], options, &(patterns->countPregs),
               &(patterns->pregs));
    free(strPatterns[i]);
  }
  for (size_t i = 0; i < strPatternsESize; i++) {
    AddPattern(strPatternsE[i], options, &(patterns->countPregs),
               &(patterns->pregs));
    free(strPatternsE[i]);
  }
  free(strPatterns);
  free(strPatternsE);

  return isError;
}
void AddPattern(char* patternStr, Options* options, int* countPregs,
                regex_t** pregs) {
  regex_t preg;
  if (regcomp(&preg, patternStr, options->regexFlag)) {
    fprintf(stderr, "failed to compile regex\n");
  }

  (*countPregs)++;

  *pregs = realloc(*pregs, (*countPregs) * sizeof(regex_t));

  (*pregs)[*countPregs - 1] = preg;
}

int AddPatternsFromFile(char* fileName, Options* options, size_t* arraySize,
                        char*** strArray) {
  int isError = 0;
  if (IsFileExist(fileName, 0)) {
    char* line = NULL;
    size_t length = 0;
    FILE* file = fopen(fileName, "rb");
    while (getline(&line, &length, file) != -1) {
      size_t len = strcspn(line, "\n");
      if (len > 0) {
        line[len] = '\0';
      }
      if (strcmp(line, ".") == 0) {
        options->regexFlag += REG_NEWLINE;
      }
      AddPatternToStrArray(line, strArray, arraySize);
    }
    fclose(file);
    free(line);
  } else {
    isError = 1;
  }
  return isError;
}

void AddPatternToStrArray(char* patternStr, char*** strArray,
                          size_t* arraySize) {
  *strArray = (char**)realloc(*strArray, (*arraySize + 1) * sizeof(char*));
  (*strArray)[*arraySize] =
      (char*)malloc((strlen(patternStr) + 1) * sizeof(char));
  strcpy((*strArray)[*arraySize], patternStr);

  (*arraySize)++;
}
void FreePatterns(regex_t** pregs, int countPregs) {
  for (int i = 0; i < countPregs; i++) {
    if (&((*pregs)[i]) != NULL) regfree(&((*pregs)[i]));
  }

  free(*pregs);

  *pregs = NULL;
}
int IsFileExist(char* fileName, int isNotPrintError) {
  FILE* file;
  int isExist = 0;
  file = fopen(fileName, "rb");
  if (file) {
    fclose(file);
    isExist = 1;
  } else if (!isNotPrintError) {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", fileName);
  }
  return isExist;
}