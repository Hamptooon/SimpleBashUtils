#include "check_options.h"
#include "print_file.h"

void Cat(int argc, char *argv[]) {
  Options options = OptionsInit();
  if ((!CheckOptions(argc, argv, &options))) {
    PrintAllInputFiles(argc, argv, &options);
  }
}
int main(int argc, char *argv[]) { Cat(argc, argv); }
