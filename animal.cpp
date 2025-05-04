#include "animal.hpp"

using namespace std;

// === Constructeur ===
Animal::Animal(int id, Espece espece, Coord position, Sexe sexe)
    : id(id), espece(espece), position(position), sexe(sexeAleatoire()), faim(0) {}

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

Sexe Animal::getSexe() const {
    return sexe;
}


// === Predicats ===
bool Animal::meurt(int maxFaim) const {
    return faim >= maxFaim;
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
    faim++; // Tous les animaux ont maintenant une faim progressive
}

Sexe Animal::sexeAleatoire() {
    return rand() % 2 ? Sexe::Female : Sexe::Male;
}

// === Affichage ===
void Animal::affiche(ostream& os) const {
    os << "[id=" << id
       << ", espece=" << (espece == Espece::Lapin ? "Lapin" : "Renard")
       << ", sexe=" << (sexe == Sexe::Male ? "M" : "F")
       << ", pos=" << position.toString()
       << ", faim=" << faim << "]";
}
