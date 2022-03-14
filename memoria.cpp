#include "memoria.h"

Memory::Memory(int size) {
  registers_.resize(size);
  // for (int  i = 0; i < registers_.size(); i++) {
  //   registers_[i].resize(size);
  // }
}

void Memory::write(int data,int reg, int pos) {
  while(registers_[reg].size() <= pos) {
    registers_[reg].push_back(0);
  }
  registers_[reg][pos] = data;
}

void Memory::print() {
  std::cout << "memory:\n";
  for(int i = 0; i<registers_.size(); i++) {
    std::cout << "R" << i << ": ";
    for(int j = 0; j<registers_[i].size(); j++) {
      std::cout << registers_[i][j] << " ";
    }
    std::cout << "\n";
  }
}