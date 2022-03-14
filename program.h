#ifndef PROGRAMA
#define PROGRAMA
#include "instruccion.h"
#include "tag.h"
#include <vector>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <exception>
#include <regex>
#include <set>

class Program {
 public:
  Program(std::string fileName){load(fileName);};
  ~Program(){};
  Instruction* getInstruction(int pc){return instructions_[pc];};
  bool reachHalt(int pc){return instructions_[pc]->getOp() == "halt";};
  int getLineByTag(std::string tag);
  std::string getFileName(){return fileName_;};
  int getNumberOfInstructions(){return instructions_.size();};
 private:
  void load(std::string fileName){std::string programStr = readFile(fileName); parseInstructions(programStr);};
  //void setTags(std::string);
  std::string readFile(std::string fileName);
  void parseInstructions(std::string);
  //bool existsTag(std::string);
  std::string getTagFromString(std::string data);
  std::vector<Instruction*> instructions_;
  std::vector<Tag*> tags_;
  std::string fileName_;
  std::set<std::string> normalOps_ = {"add","sub","mul","div","load","store","read","write"};
  std::set<std::string> jumpOps_ = {"jump","jgtz","jzero"};
};

#endif