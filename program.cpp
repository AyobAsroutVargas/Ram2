#include "program.h"

int Program::getLineByTag(std::string tag) {
  for (int i = 0; i < tags_.size(); i++) {
    if (tags_[i]->getId() == tag) return tags_[i]->getLine();
  }
  return -1;
}

std::string Program::readFile(std::string fileName) {
  std::ifstream file;
  std::string fileData = "";
  fileName_ = fileName;
  file.open (fileName_.c_str());
  if (file.is_open()) {
    while (!file.eof()) {
      std::string temp;
      std::getline (file,temp);
      fileData += temp + "\n";
    }
    file.close();
  }
  else {
    std::cerr << "Could not open the file";
  }
  return fileData;
}

void  Program::parseInstructions(std::string fileData) {
  std::transform(fileData.begin(), fileData.end(), fileData.begin(), ::tolower); //Pasamos todo a minusculas
  bool error = false;
  int lineCounter = 0;
  char* input = (char*) fileData.c_str();
  char* token = std::strtok(input, "\n"); //Dividimos en filas
  std::string actualTag = "main";

  while (token != NULL && !error) { //Recorremos todo el string que contiene las instrucciones hasta el final o hasta que se produzca un error
    bool validInstruction = false;
    bool newTag = false;
    Instruction* tempInstruction = new Instruction();
    std::string lineString(token);
    std::string tag = getTagFromString(lineString); //Miramos si la linea en cuestion tiene un tag

    if( tag != "noTag") { //Si habia un tag cambiamos el tag actual y añadimos el tag a la lista de tags
      actualTag = tag;
      tags_.push_back(new Tag(tag, lineCounter));
      newTag = true;
    }
    tempInstruction->setTag(actualTag); //Seteamos el tag de la instruccion temporal
    //std::istringstream rawInstruct(lineString);
    fileData = token; //fileData contiene la linea actual del programa que estamos evaluando
    std::istringstream iss (fileData);
    std::string word;
    iss >> word; //Introducimos la siguiente palabra del programa a word
    if (newTag) iss >> word;
    if (word == ":") iss >> word; //En estas 2 lineas comprobamos si la palabra a introducir en word es un tag, si es asi pasamos a la siguiente.
    
    if(normalOps_.find(word) != normalOps_.end()) {//Si la palabra que contiene word pertenece a las operaciones normales
      tempInstruction->setOp(word); //Seteamos la operacion de la instruccion temporal.
      iss >> word; // Avanzamos a la siguiente palabra.
      if(word[0] == '*' || word[0] == '=') {
        tempInstruction->setType(word[0]);
        if (word.size() > 1) { 
          std::string operand = word.substr(1,word.size()-1);
          tempInstruction->setOperand(operand); //Seteamos el operando de la instruccion temporal
          validInstruction = true;
        } else { //si la siguiente palabra es de tamaño 1 quiere decir que es un numero
          iss >> word;
          if(std::regex_match (word, std::regex("[0-9]"))) {//Comprobamos que es un numero.
            tempInstruction->setOperand(word);//Seteamos el operando de la instruccion temporal.
            validInstruction = true;
          } else { //Si el operando es de 1 caracter y no es un numero entonces es una operacion no valida.
            std::cout << "At line " << lineCounter << " is an invalid instruction. Expected number and it get \"" << word << "\"." << "\n";
            error = true;
          }
        }
      } else if (std::regex_match (word, std::regex("[0-9]"))) { //Si la palabra que leemos es un numero entonces estamos leyendo un operano de una instruccion de tipo direccionamiento directo
        tempInstruction->setType('d');//Seteamos el tipo de instruccion a direccionamiento directo a la instruccion temporal
        tempInstruction->setOperand(word);//Seteamos el operando a la palabra
        validInstruction = true; 
      } else if (std::regex_match (word, std::regex("[0-9]\\[[0-9]]|[0-9]\\[=[0-9]]"))) {
        tempInstruction->setType('[');//Seteamos el tipo de instruccion a direccionamiento de vectores
        tempInstruction->setOperand(word.substr(0,1));//Seteamos el operando
        //tempInstruction->setPos(word.substr(0,1));
        if (std::regex_match (word, std::regex("[0-9]\\[[0-9]]"))) {
          tempInstruction->setPos("Indirect");
          tempInstruction->setIndirectPos(word.substr(2,1));
        } else if(std::regex_match (word, std::regex("[0-9]\\[=[0-9]]"))) {
          tempInstruction->setPos(word.substr(3,1));
        }
        validInstruction = true;
      }
    } else if (jumpOps_.find(word) != normalOps_.end()) {//Si la palabra es una instruccion de salto
      if(word[0] == '#'){
        token = std::strtok(NULL, "\n");
        continue;
      }
      tempInstruction->setOp(word);//Seteamos la operacion de la instruccion temporal
      iss >> word;
      tempInstruction->setOperand(word);//Seteamos el operadondo a la siguiente palabra
      validInstruction = true;
    } else if(word.find("halt") != std::string::npos) {//Si la instruccion que leemos es halt
      tempInstruction->setOp(word);//Seteamos la operacion a la instruccion temporal
      validInstruction = true;
    }

    if(validInstruction) {//Si hemos leido una instruccion valida
      // std::string tempString;
      // while(rawInstruct >> tempString) {
      //   tempInstruction->rawInstruction += tempString + " "; 
      // }
      //tempInstruction->rawInstruction = lineString;
      tempInstruction->setLine(lineCounter++);//Establecemos la linea de la instruccion temporal
      instructions_.push_back(tempInstruction);//Insertamos la instruccion temporal a las instrucciones del programa
    } 
    token = std::strtok(NULL, "\n");//Obtenemos la siguiente linea del programa, si es NULL es que ya hemos terminado de leer
  }
}

std::string Program::getTagFromString(std::string data) {
  std::string result = "noTag";
  if (data.find(":") != std::string::npos){//Si el string contiene ':' hay un tag en el
    for (int i = 0; i < data.size(); i++) {//Recorremos el string
      if (data[i] == ':'){//Si el caracter es ':' encontramos el string
        int it = i; //Creamos un iterador para volver hacia atras desde la posicion del ':'
        std::string tag;
        while(data[--it] == ' ' || data[it] == '\t' || data[it] == ':'); //Nos saltamos los espacios en blanco
        while(data[it] != ' '  && data[it] != '\t' && it >= 0) { //Volvemos hasta que se acaben los caracteres que conforman el tag
          tag.push_back(data[it--]);//Introducimos el caracter data[it] y restamos 1 a it
        }
      reverse(tag.rbegin(), tag.rend());//Le damos la vuelta al tag ya que lo leimos de derecha a izquierda
      result = tag;
      }
    }
  }
  return result; //Devolvemos el tag
}