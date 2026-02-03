#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: fastlog <logfile>" << std::endl;
    return 1;
  }

  std::cout << "FastLog Processing: " << argv[1] << std::endl;
  // TODO: Connect Parser and Stats here
  return 0;
}
