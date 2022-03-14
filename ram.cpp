#include "ram.h"

#define INDIRECT '*'
#define DIRECT 'd'
#define IMMEDIATE '='
#define VECTOR '['

RamMachineSimulator::RamMachineSimulator(std::string in, std::string out, std::string program, int size) {
  pc_ = 0;
  inputTape_ = new InputTape(in);
  outputTape_ = new OutputTape(out);
  program_ = new Program(program);
  memory_ = new Memory(size);
  error_ = false;
}

RamMachineSimulator::~RamMachineSimulator() {
  delete inputTape_;
  delete outputTape_;
  delete program_;
  delete memory_;
}

void RamMachineSimulator::simulateProgram(int debug) {
  bool halt = false;
  bool indirect = false;
  int instruction_count = 0;
  while (!halt && !error_) {//Ejecutamos el programa hasta que lleguemos a una instruccion halt o se produzca un error.
    bool salto = false;
    Instruction* actualInstruction = program_->getInstruction(pc_); //Leemos la instruccion actual
    if(debug == 2) {
      actualInstruction->print();
      memory_->print();
      std::cout << "Cinta de entrada:\n";
      inputTape_->print();
      std::cout << "Cinta de salida:\n";
      outputTape_->print();
    }
    
    if(actualInstruction->getOp() == "read") {//Si la instruccion es de lectura
      int inputTapeData = inputTape_->read();//Leemos de la cinta de entrada.
      switch (actualInstruction->getType()) {//vemos el tipo de direccionamiento usado
        case INDIRECT:
          //Si el direccionamiento es indirecto escribimos en el registro al que apunta el contenido del registro cuyo numeroes el operando
          if(std::stoi(actualInstruction->getOperand()) == 0) {
            std::cout << "No se puede leer el registo 0\n";
            error_ = true;
            break;
          }
          memory_->write(inputTapeData, memory_->read(std::stoi(actualInstruction->getOperand())));
          break;
        case DIRECT:
          //Si el direccionamiento es directo escribimos en el registro cuyo numero es el operando
          if(std::stoi(actualInstruction->getOperand()) == 0) {
            std::cout << "No se puede leer el registo 0\n";
            error_ = true;
            break;
          }
          memory_->write(inputTapeData, std::stoi(actualInstruction->getOperand()));
          break;
        case IMMEDIATE:
          std::cout << "Direccionamiento inmediato no tiene sentido con instruccion de lectura\n";
          error_ = true;
          break;
        case VECTOR:
        if(std::stoi(actualInstruction->getOperand()) == 0) {
            std::cout << "No se puede leer el registo 0\n";
            error_ = true;
            break;
          }
          if(actualInstruction->getPos() == "Indirect") {
            indirect = true;
            actualInstruction->setPos(std::to_string(memory_->read(std::stoi(actualInstruction->getIndirectPos()))));
          }
          memory_->write(inputTapeData, std::stoi(actualInstruction->getOperand()), std::stoi(actualInstruction->getPos()));
          if(indirect) program_->getInstruction(pc_)->setPos("Indirect");
          break;
        default:
          std::cout << "No se reconoce el tipo de instruccion READ\n";
          error_ = true;
          break;
      }
    } else if (actualInstruction->getOp() == "write") {//Si la instruccion es de escritura
      switch (actualInstruction->getType()) {
        case INDIRECT:
          if(std::stoi(actualInstruction->getOperand()) == 0) {
            std::cout << "No se puede escribir en el registo 0\n";
            error_ = true;
            break;
          }
          outputTape_->write(memory_->read(memory_->read(std::stoi(actualInstruction->getOperand()))));
          break;
        case DIRECT:
          if(std::stoi(actualInstruction->getOperand()) == 0) {
            std::cout << "No se puede escribir en el registo 0\n";
            error_ = true;
            break;
          }
          outputTape_->write(memory_->read(std::stoi(actualInstruction->getOperand())));
          break;
        case IMMEDIATE:
          if(std::stoi(actualInstruction->getOperand()) == 0) {
            std::cout << "No se puede escribir en el registo 0\n";
            error_ = true;
            break;
          }
          outputTape_->write(std::stoi(actualInstruction->getOperand()));
          break;
        case VECTOR:
          if(std::stoi(actualInstruction->getOperand()) == 0) {
            std::cout << "No se puede escribir el registo 0\n";
            error_ = true;
            break;
          }
          if(actualInstruction->getPos() == "Indirect") {
            indirect = true;
            actualInstruction->setPos(std::to_string(memory_->read(std::stoi(actualInstruction->getIndirectPos()))));
          }
          outputTape_->write(memory_->read(std::stoi(actualInstruction->getOperand()), std::stoi(actualInstruction->getPos())));
          if(indirect) program_->getInstruction(pc_)->setPos("Indirect");
          break;
        default:
          std::cout << "No se reconoce el tipo de instruccion WRITE\n";
          error_ = true;
          break;
      }
    } else if (actualInstruction->getOp() == "load") {//Si la instruccion es de carga
      switch (actualInstruction->getType()) {
        case INDIRECT:
          memory_->write(memory_->read(memory_->read(std::stoi(actualInstruction->getOperand()))), 0);
          break;
        case DIRECT:
          memory_->write(memory_->read(std::stoi(actualInstruction->getOperand())), 0);
          break;
        case IMMEDIATE:
          memory_->write(std::stoi(actualInstruction->getOperand()), 0);
          break;
        case VECTOR:
          if(actualInstruction->getPos() == "Indirect") {
            indirect = true;
            actualInstruction->setPos(std::to_string(memory_->read(std::stoi(actualInstruction->getIndirectPos()))));
          }
          memory_->write(memory_->read(std::stoi(actualInstruction->getOperand()), std::stoi(actualInstruction->getPos())),0);
          if(indirect) program_->getInstruction(pc_)->setPos("Indirect");
          break;
        default:
          std::cout << "No se reconoce el tipo de instruccion LOAD\n";
          error_ = true;
          break;
      }
    } else if (actualInstruction->getOp() == "store") {//Si la instruccion es de guardado
      switch (actualInstruction->getType()) {
        case INDIRECT:
          memory_->write(memory_->read(0), memory_->read(std::stoi(actualInstruction->getOperand())));
          break;
        case DIRECT:
          memory_->write(memory_->read(0), std::stoi(actualInstruction->getOperand()));
          break;
        case IMMEDIATE:
          std::cout << "No se puede usar direccionamiento inmediato en instrucciones STORE\n";
          error_ = true;
          break;
        case VECTOR:
          if(actualInstruction->getPos() == "Indirect") {
            indirect = true;
            actualInstruction->setPos(std::to_string(memory_->read(std::stoi(actualInstruction->getIndirectPos()))));
          }
          memory_->write(memory_->read(0), std::stoi(actualInstruction->getOperand()), std::stoi(actualInstruction->getPos()));
          if(indirect) program_->getInstruction(pc_)->setPos("Indirect");
          break;
        default:
          std::cout << "No se reconoce el tipo de instruccion STORE\n";
          error_ = true;
          break;
      }
    } else if (actualInstruction->getOp() == "add") {//Si la instruccion es de suma
      switch (actualInstruction->getType()) {
        case INDIRECT:
          memory_->write(memory_->read(0) + memory_->read(memory_->read(std::stoi(actualInstruction->getOperand()))), 0);
          break;
        case DIRECT:
          memory_->write(memory_->read(0) + memory_->read(std::stoi(actualInstruction->getOperand())), 0);
          break;
        case IMMEDIATE:
          memory_->write(memory_->read(0) + std::stoi(actualInstruction->getOperand()), 0);
          break;
        case VECTOR:
          if(actualInstruction->getPos() == "Indirect") {
            indirect = true;
            actualInstruction->setPos(std::to_string(memory_->read(std::stoi(actualInstruction->getIndirectPos()))));
          }
          memory_->write(memory_->read(0) + memory_->read(std::stoi(actualInstruction->getOperand()), std::stoi(actualInstruction->getPos())), 0);
          if(indirect) program_->getInstruction(pc_)->setPos("Indirect");
          break;
        default:
          std::cout << "No se reconoce el tipo de instruccion ADD\n";
          error_ = true;
          break;
      }
    } else if (actualInstruction->getOp() == "sub") {//Si la instruccion es de resta
      switch (actualInstruction->getType()) {
        case INDIRECT:
          memory_->write(memory_->read(0) - memory_->read(memory_->read(std::stoi(actualInstruction->getOperand()))), 0);
          break;
        case DIRECT:
          memory_->write(memory_->read(0) - memory_->read(std::stoi(actualInstruction->getOperand())), 0);
          break;
        case IMMEDIATE:
          memory_->write(memory_->read(0) - std::stoi(actualInstruction->getOperand()), 0);
          break;
        case VECTOR:
          if(actualInstruction->getPos() == "Indirect") {
            indirect = true;
            actualInstruction->setPos(std::to_string(memory_->read(std::stoi(actualInstruction->getIndirectPos()))));
          }
          memory_->write(memory_->read(0) - memory_->read(std::stoi(actualInstruction->getOperand()), std::stoi(actualInstruction->getPos())), 0);
          if(indirect) program_->getInstruction(pc_)->setPos("Indirect");
          break;
        default:
          std::cout << "No se reconoce el tipo de instruccion SUB\n";
          error_ = true;
          break;
      }
    } else if (actualInstruction->getOp() == "mul") {//Si la instruccion es de multiplicacion
      switch (actualInstruction->getType()) {
        case INDIRECT:
          memory_->write(memory_->read(0) * memory_->read(memory_->read(std::stoi(actualInstruction->getOperand()))), 0);
          break;
        case DIRECT:
          memory_->write(memory_->read(0) * memory_->read(std::stoi(actualInstruction->getOperand())), 0);
          break;
        case IMMEDIATE:
          memory_->write(memory_->read(0) * std::stoi(actualInstruction->getOperand()), 0);
          break;
        case VECTOR:
          if(actualInstruction->getPos() == "Indirect") {
            indirect = true;
            actualInstruction->setPos(std::to_string(memory_->read(std::stoi(actualInstruction->getIndirectPos()))));
          }
          memory_->write(memory_->read(0) * memory_->read(std::stoi(actualInstruction->getOperand()), std::stoi(actualInstruction->getPos())), 0);
          if(indirect) program_->getInstruction(pc_)->setPos("Indirect");
          break;
        default:
          std::cout << "No se reconoce el tipo de instruccion MUL\n";
          error_ = true;
          break;
      }
    } else if (actualInstruction->getOp() == "div") {//Si la instruccion es de division
      switch (actualInstruction->getType()) {
        case INDIRECT:
          memory_->write(memory_->read(0) / memory_->read(memory_->read(std::stoi(actualInstruction->getOperand()))), 0);
          break;
        case DIRECT:
          memory_->write(memory_->read(0) / memory_->read(std::stoi(actualInstruction->getOperand())), 0);
          break;
        case IMMEDIATE:
          memory_->write(memory_->read(0) / std::stoi(actualInstruction->getOperand()), 0);
          break;
          case VECTOR:
          if(actualInstruction->getPos() == "Indirect") {
            indirect = true;
            actualInstruction->setPos(std::to_string(memory_->read(std::stoi(actualInstruction->getIndirectPos()))));
          }
          memory_->write(memory_->read(0) / memory_->read(std::stoi(actualInstruction->getOperand()), std::stoi(actualInstruction->getPos())), 0);
          if(indirect) program_->getInstruction(pc_)->setPos("Indirect");
          break;
        default:
          std::cout << "No se reconoce el tipo de instruccion DIV\n";
          error_ = true;
          break;
      }
    } else if (actualInstruction->getOp() == "jump") {//Si la instruccion es de salto
      salto = true;
      pc_ = program_->getLineByTag (actualInstruction->getOperand());
      if (pc_ == -1) {
        std::cout << "El Tag " << actualInstruction->getOperand () << " no se encuentra en el programa.\n";
        error_ = true;
      }
    } else if (actualInstruction->getOp() == "jzero") {//Si la instruccion es de salto condicional(R0 == 0)
      if(memory_->read(0) == 0) {
        salto = true;
        pc_ = program_->getLineByTag (actualInstruction->getOperand());
        if (pc_ == -1) {
          std::cout << "El Tag " << actualInstruction->getOperand () << " no se encuentra en el programa.\n";
          error_ = true;
        }
      }
    } else if (actualInstruction->getOp() == "jgtz") {//Si la instruccion es de salto condicional(R0 > 0)
      if(memory_->read(0) > 0) {
        salto = true;
        pc_ = program_->getLineByTag (actualInstruction->getOperand());
        if (pc_ == -1) {
          std::cout << "El Tag " << actualInstruction->getOperand () << " no se encuentra en el programa.\n";
          error_ = true;
        }
      }
    } else if (actualInstruction->getOp() == "halt") {//Si la instruccion es de fin de programa
      halt = true;
    }

    if(error_) {
      std::cout << "Error at line " << pc_ << "\n";
      break;
    }
    instruction_count++;
    if(!salto) pc_++;
  }
  outputTape_->saveToFile();
  if(debug == 1 || debug == 2) {
    std::cout << "Instructions executed: " << instruction_count << "\n";
  }
}