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

int Animal::getFaim() const {
    return faim;
}


// === Predicats ===
bool Animal::meurt(int maxFaim) const {
    return espece == Espece::Renard && faim >= maxFaim;
}

bool Animal::seReproduit(int nbVoisinsVides, int seuil) const {
    return nbVoisinsVides >= seuil;
}

// === Modificateurs ===
void Animal::mange(int foodGain, int maxFaim) {
    faim -= foodGain;
    if (faim < 0) faim = 0;
    if (faim > maxFaim) faim = maxFaim;
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
