# Carpetas de trabajo
BIN=./bin
SRC=./src
GRP=./graph

# Opciones de compilación
# -Wall muestra todas las advertencias
# -O2 optimización.
OPT=-Wall -O2

all:$(BIN)/knn $(BIN)/greedy  $(BIN)/localSearch

$(BIN)/knn : $(SRC)/knn.cpp
	g++ $(OPT) $^ -o $@

$(BIN)/greedy : $(SRC)/greedy.cpp
	g++ $(OPT) $^ -o $@


$(BIN)/localSearch : $(SRC)/localSearch.cpp
	g++ $(OPT) $^ -o $@

clean:
	rm -f $(BIN)/*

fullclean:
	rm -f $(BIN)/* $(GRP)/data/* $(GRP)/plots/*