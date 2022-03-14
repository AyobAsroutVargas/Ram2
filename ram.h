#ifndef RAM
#define RAM
#include "cinta-entrada.h"
#include "cinta-salida.h"
#include "memoria.h"
#include "program.h"
#include "instruccion.h"

class RamMachineSimulator {
 public:
  RamMachineSimulator(std::string in, std::string out, std::string program, int size);
  ~RamMachineSimulator();
  void simulateProgram(int debug = 0);
 private:
  InputTape* inputTape_;
  OutputTape* outputTape_;
  Memory* memory_;
  Program* program_;
  int pc_;
  bool error_;
};

#endif