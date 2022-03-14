CXX = g++
CXXFLAGS = -std=c++14 -g -Wall
DEPS = cinta-entrada.h cinta-salida.h cinta.h instruccion.h memoria.h program.h ram.h tag.h 
OBJ = main.o cinta-entrada.o cinta-salida.o cinta.o program.o ram.o memoria.o instruccion.o

%.o: %.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

main: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean
clean:
	rm main *.o