#include "animal.hpp"
#include "doctest.h"

using namespace std;

Animal::Animal(int id, const Coord &position, Espece espece): id(id), position(position), espece(espece) {}

int Animal::getId() const {
    return id;
}

Coord Animal::getPosition() const {
    return position;
}

Espece Animal::getEspece() const {
    return espece;
}

void Animal::deplacer(const Coord &nouvellePos) {
    position = nouvellePos;
	
}

TEST_CASE("Test de la classe Animal") {
    Coord posInitiale(5, 5);
    // Creation d'un Lapin avec id = 1
    Animal a(1, posInitiale, Lapin);
    CHECK(a.getId() == 1);
    CHECK(a.getPosition() == posInitiale);
    CHECK(a.getEspece() == Lapin);

    // Deplacement
    Coord posNouvelle(6, 7);
    a.deplacer(posNouvelle);
    CHECK(a.getPosition() == posNouvelle);
}
