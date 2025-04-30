#include "animal.hpp"

using namespace std;

// === Constructeur ===
Animal::Animal(int id, Espece espece, Coord position)
    : id(id), espece(espece), position(position), faim(0) {}

// === Accesseurs ===
int Animal::getId() const {
    return id;
}

Coord Animal::getCoord() const {
    return position;
}

void Animal::setCoord(Coord c) {
    position = c;
}

Espece Animal::getEspece() const {
    return espece;
}

// === Predicats ===
bool Animal::meurt() const {
    return espece == Espece::Renard && faim >= 5;
}

bool Animal::seReproduit(int nbVoisinsVides) const {
    return nbVoisinsVides >= 2;
}

// === Modificateurs ===
void Animal::mange() {
    faim = 0;
}

void Animal::jeune() {
    if (espece == Espece::Renard)
        faim++;
}

// === Affichage ===
void Animal::affiche(ostream& os) const {
    os << "[id=" << id
       << ", espece=" << (espece == Espece::Lapin ? "Lapin" : "Renard")
       << ", pos=" << position.toString()
       << ", faim=" << faim << "]";
}
