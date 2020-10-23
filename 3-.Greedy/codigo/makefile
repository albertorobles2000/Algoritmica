SRC = src
BIN = bin
CXX = g++
TSP = Ejer_viajante_de_comercio
CONTENEDORES = Ejer_3.4
CPPFLAGS = --std=c++11

all: $(TSP)/$(BIN)/cercania $(TSP)/$(BIN)/insercion $(TSP)/$(BIN)/propio $(CONTENEDORES)/$(BIN)/contenedor


# ************ Compilación de módulos ************

$(TSP)/$(BIN)/cercania: $(TSP)/$(SRC)/cercania.cpp
	g++ $(TSP)/$(SRC)/cercania.cpp -o $(TSP)/$(BIN)/cercania

$(TSP)/$(BIN)/insercion: $(TSP)/$(SRC)/insercion.cpp
	g++ $(TSP)/$(SRC)/insercion.cpp -o $(TSP)/$(BIN)/insercion

$(TSP)/$(BIN)/propio: $(TSP)/$(SRC)/propio.cpp
	g++ $(TSP)/$(SRC)/propio.cpp -o $(TSP)/$(BIN)/propio

$(CONTENEDORES)/$(BIN)/contenedor: $(CONTENEDORES)/$(SRC)/contenedor.cpp
	g++ $(CONTENEDORES)/$(SRC)/contenedor.cpp -o $(CONTENEDORES)/$(BIN)/contenedor
