# Carpetas de trabajo
BIN=./bin
SRC=./src

# Opciones de compilación
# -Wall muestra todas las advertencias
# -O2 optimización.
OPT=-Wall -O2

all: $(BIN)/localSearch $(BIN)/bmb $(BIN)/ils $(BIN)/es $(BIN)/ils-es 

$(BIN)/localSearch : $(SRC)/localSearch.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/bmb : $(SRC)/bmb.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/ils : $(SRC)/ils.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/es : $(SRC)/es.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/ils-es : $(SRC)/ils-es.cpp
	g++ $(OPT) $^ -o $@

clean:
	rm -f $(BIN)/*