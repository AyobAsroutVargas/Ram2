#include <iostream>
#include "cinta.h"

void Tape::print() {
  std::cout << "Actual position: " << inx_ << "\n";
  std::cout << "content:\n";
  std::cout << "[";
  for (int i = 0; i < nodes_.size(); i++) {
    std::cout << " " << nodes_[i];
    if (i < nodes_.size()-1) {
      std::cout << ",";
    }
  }
  std::cout << "]\n";
}

