#ifndef INSTRUCCION
#define INSTRUCCION
#include <iostream>
#include <string>

class Instruction {
 public:
  Instruction(){line_ = -1; tag_ = ""; op_ = ""; type_ = '\0'; operand_ = ""; pos_ = "0"; indirectPos_ = "";};
  Instruction(int line, std::string tag, std::string op, char type, std::string operand);
  ~Instruction(){};
  int getLine(){return line_;};
  void setLine(int line){line_ = line;};
  std::string getTag(){return tag_;};
  void setTag(std::string tag){tag_ = tag;};
  std::string getOp(){return op_;};
  void setOp(std::string op){op_ = op;};
  char getType(){return type_;};
  void setType(char type){type_ = type;};
  std::string getOperand(){return operand_;};
  void setOperand(std::string operand){operand_ = operand;};
  std::string getPos(){return pos_;};
  void setPos(std::string pos){pos_ = pos;};
  std::string getIndirectPos(){return indirectPos_;};
  void setIndirectPos(std::string indirectPos){indirectPos_ = indirectPos;};
  void print();
  //std::string rawInstruction;
 private:
  int line_;
  std::string tag_;
  std::string op_;
  char type_;
  std::string operand_;
  std::string pos_;
  std::string indirectPos_;
};
#endif