#ifndef CINTA_ENTRADA
#define CINTA_ENTRADA
#include "cinta.h"

class InputTape : public Tape {
 public:
  InputTape(std::string fileName){load(fileName);};
  ~InputTape(){clear();};
  void load(std::string fileName);
  void reset(std::string fileName){load(fileName);};
  void reset(){inx_ = 0;};
  int read();
  bool hasNext(){return inx_ < nodes_.size();};
};

#endif