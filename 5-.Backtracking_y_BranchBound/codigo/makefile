SRC = src
BIN = bin
CXX = g++
CPPFLAGS = -O2 --std=c++11

all: $(BIN)/TSP_Backtracking $(BIN)/TSP_BranchNBound $(BIN)/itv 


# ************ Compilación de módulos ************

$(BIN)/TSP_Backtracking: $(SRC)/TSP_Backtracking.cpp
	g++ $(SRC)/TSP_Backtracking.cpp -o $(BIN)/TSP_Backtracking

$(BIN)/TSP_BranchNBound: $(SRC)/TSP_BranchNBound.cpp
	g++ $(SRC)/TSP_BranchNBound.cpp -o $(BIN)/TSP_BranchNBound

$(BIN)/itv: $(SRC)/itv.cpp
	g++ $(SRC)/itv.cpp -o $(BIN)/itv

