#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include "coord.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

enum Espece {
    Lapin,
    Renard
};

class Animal {
	private:
	    int id;
	    Coord position;
	    Espece espece;
	
	public:
	    Animal(int id, const Coord &position, Espece espece);
	
	    int getId() const;
	    Coord getPosition() const;
	    Espece getEspece() const;
	
	    void deplacer(const Coord &nouvellePos);
};

#endif
