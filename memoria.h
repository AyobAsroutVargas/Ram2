#ifndef MEMORIA
#define MEMORIA

#include <vector>
#include <string>
#include <iostream>

class Memory {
 public:
  Memory(int size = 100);
  ~Memory(){};
  void write(int data,int reg, int pos = 0);
  int read(int reg, int pos = 0){return registers_[reg][pos];};
  void clear(){registers_.clear();};
  void print();
 private:
  std::vector<std::vector<int>> registers_;
};

#endif