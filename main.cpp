#include "ram.h"

int main(int argc, char* argv[]) {
  int debug = 0;
  if(argc < 4) {
    std::cout << "Usage: ./main <input tape> <output tape> <ram program> optional:<number of registers>\n";
    return 0;
  }
  std::string ramProgramFile(argv[1]);
  std::string inputTapeFile(argv[2]);
  std::string outputTapeFile(argv[3]);
  if(argc == 5) {
    debug = std::atoi(argv[4]);
  }
  RamMachineSimulator* ramMachine = new RamMachineSimulator(inputTapeFile, outputTapeFile, ramProgramFile, 10);
  RamMachineSimulator* ramMachineDebug = new RamMachineSimulator(inputTapeFile, outputTapeFile, ramProgramFile, 10);
  ramMachine->simulateProgram();
  switch (debug) {
    case 1:
      ramMachineDebug->simulateProgram(1);
      break;
    case 2:
      ramMachineDebug->simulateProgram(2);
      break;
  }
  return 0;
}