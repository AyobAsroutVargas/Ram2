#include "cinta-salida.h"

void OutputTape::saveToFile() {
  std::ofstream outfile;
  outfile.open(file_name_.c_str());

  for(int i = 0; i < nodes_.size(); i++) {
    outfile << nodes_[i] << " ";
  }
  outfile.close();
}