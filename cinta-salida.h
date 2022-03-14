#ifndef CINTA_SALIDA
#define CINTA_SALIDA
#include "cinta.h"

class OutputTape : public Tape {
 public:
  OutputTape(std::string fileName){file_name_ = fileName;};
  ~OutputTape(){clear();};
  void write(int v){nodes_.push_back(v); inx_++;};
  void saveToFile();
  void reset(std::string fileName){nodes_.clear(); inx_ = 0; file_name_ = fileName;};
  void reset(){nodes_.clear(); inx_ = 0;};
};

#endif