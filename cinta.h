#ifndef CINTA
#define CINTA
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

class Tape {
 protected:
  std::vector<int> nodes_;
  int inx_;
  std::string file_name_;
 public:
  Tape(): inx_(0){};
  ~Tape(){};
  void clear(){nodes_.clear(); inx_ = 0;};
  int getInx() {return inx_;};
  void print();
  std::string getFileName(){return file_name_;};
};

#endif