#include "check_options.h"

Options OptionsInit() {
  Options options = {0, 0, 0, 0, 0, 0};
  return options;
}
int CheckOptions(int argc, char *argv[], Options *options) {
  struct option optionsFullNames[] = {{"number-nonblank", 0, NULL, 'b'},
                                      {"number", 0, NULL, 'n'},
                                      {"squeeze-blank", 0, NULL, 's'},
                                      {0, 0, 0, 0}};
  int currentOption =
      getopt_long(argc, argv, "+bevEnstT", optionsFullNames, NULL);

  int isError = 0;
  while (!isError && currentOption != -1) {
    switch (currentOption) {
      case 'b':
        options->numberNonBlank = 1;
        break;
      case 'e':
        options->showEndStr = 1;
        options->showNonPrintable = 1;
        break;
      case 'n':
        options->numberAll = 1;
        break;
      case 's':
        options->squeezeBlank = 1;
        break;
      case 't':
        options->showTabs = 1;
        options->showNonPrintable = 1;
        break;
      case 'v':
        options->showNonPrintable = 1;
        break;
      case 'E':
        options->showEndStr = 1;
        break;
      case 'T':
        options->showTabs = 1;
        break;
      default:
        isError = 1;
        break;
    }
    currentOption =
        getopt_long(argc, argv, "+bevEnstT", optionsFullNames, NULL);
  }
  return isError;
}
