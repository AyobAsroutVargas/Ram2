#ifndef Tag_
#define Tag_
#include <string>

class Tag {
 public:
  Tag(std::string id, int line){id_ = id; line_ = line;};
  ~Tag(){};
  std::string getId(){return id_;};
  int getLine(){return line_;};
 private:
  std::string id_;
  int line_;
};

#endif