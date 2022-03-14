#ifndef REGISTER
#define REGISTER
#include <vector>
#include <string>

class Register {
 public:
  virtual std::string regType() = 0;
  virtual int getReg() = 0;
  virtual void setReg(int data) = 0;
};

class IntReg : public Register {
 public:
  std::string regType(){return "int";};
  int getReg(){return reg_;};
  void setReg(int data){reg_ = data;};
  int reg_;
};

class VectorReg : public Register {
 public:
  std::string regType(){return "vector";};
  std::vector<int> reg_;
};

#endif