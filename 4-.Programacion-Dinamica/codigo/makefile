BIN = bin
CPPFLAGS = --std=c++11 -O3

all: $(BIN)/TSP_bruta $(BIN)/TSP_progDin


# ************ Compilación de módulos ************

$(BIN)/TSP_bruta: ./TSP_bruta.cpp
	g++ $(CPPFLAGS) ./TSP_bruta.cpp -o ./$(BIN)/TSP_bruta

$(BIN)/TSP_progDin: ./TSP_progDin.cpp
	g++ $(CPPFLAGS) ./TSP_progDin.cpp -o ./$(BIN)/TSP_progDin
