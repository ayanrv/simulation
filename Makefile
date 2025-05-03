CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SRC = coord.cpp ensemble.cpp animal.cpp population.cpp grille.cpp export.cpp
OBJ = coord.o ensemble.o animal.o population.o grille.o export.o

main: main.cpp $(OBJ)
	$(CXX) $(CXXFLAGS) -o main main.cpp $(SRC)

test: test.cpp $(OBJ)
	$(CXX) $(CXXFLAGS) -o test test.cpp $(SRC)

clean:
	rm -f *.o main test