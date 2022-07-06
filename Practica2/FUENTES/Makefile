# Carpetas de trabajo
BIN=./bin
SRC=./src

# Opciones de compilación
# -Wall muestra todas las advertencias
# -O2 optimización.
OPT=-Wall -O2

all:$(BIN)/aggBlx $(BIN)/aggCa $(BIN)/ageBlx $(BIN)/ageCa $(BIN)/am $(BIN)/localSearch

$(BIN)/aggBlx : $(SRC)/aggBlx.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/aggCa : $(SRC)/aggCa.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/ageBlx : $(SRC)/ageBlx.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/ageCa : $(SRC)/ageCa.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/am : $(SRC)/am.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/localSearch : $(SRC)/localSearch.cpp
	g++ $(OPT) $^ -o $@

clean:
	rm -f $(BIN)/*