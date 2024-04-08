#include <errno.h>
#include <stdio.h>

#include "check_options.h"
#include "print_file.h"
void Grep(int argc, char *argv[]) {
  Options options = OptionsInit();
  Patterns patterns = PatternsInit();
  if ((!CheckOptions(argc, argv, &options, &patterns))) {
    GrepFiles(argc, argv, &options, &patterns);
  }
}
int main(int argc, char *argv[]) { Grep(argc, argv); }