# Carpetas de trabajo
BIN=./bin
SRC=./src

# Opciones de compilación
# -Wall muestra todas las advertencias
# -O2 optimización.
OPT=-Wall -O2

all: $(BIN)/charges $(BIN)/chargesBl $(BIN)/chargesMemetic $(BIN)/chargesMaxIter $(BIN)/chargesReset $(BIN)/chargesEuclidean

$(BIN)/charges : $(SRC)/charges.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/chargesBl : $(SRC)/chargesBl.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/chargesMemetic : $(SRC)/chargesMemetic.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/chargesMaxIter : $(SRC)/chargesMaxIter.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/chargesReset : $(SRC)/chargesReset.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/chargesEuclidean : $(SRC)/chargesEuclidean.cpp
	g++ $(OPT) $^ -o $@

clean:
	rm -f $(BIN)/*