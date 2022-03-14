#include "instruccion.h"

void Instruction::print() {
  std::string output = "";
  output += op_ + " ";
  if(type_ != '[') {
    if(type_ != 'd') {
      output += type_ + operand_;
    } else {
      output += operand_;
    }
  } else {
    output += "[";
    if (pos_ != "indirect"){
      output += "=";
    }
    output += operand_ + "]";
  }
  std::cout << "Instruction: " << output << "\n";
}