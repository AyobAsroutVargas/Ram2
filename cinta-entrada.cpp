#include "cinta-entrada.h"

void InputTape::load(std::string fileName) {
  inx_ = 0;
  std::fstream file;
  int num;
  file_name_ = fileName;
  file.open(file_name_.c_str(), std::ios::in);
  if(file.is_open()) {
      nodes_.clear();
    while(file >> num){
      nodes_.push_back(num);
    }
    file.close();
  } else {
    std::cerr << "Could not open the file\n";
  }
}

int InputTape::read() {
  if (inx_ < nodes_.size()) return nodes_[inx_++];
  else return 0;
}