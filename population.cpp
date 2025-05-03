// population.cpp
#include "population.hpp"
#include <stdexcept>

using namespace std;

// === Constructeur ===
Population::Population() : animaux(), idsUtilises() {}

// === Accesseurs ===
Animal Population::get(int id) const {
    for (const Animal& a : animaux) {
        if (a.getId() == id)
            return a;
    }
    throw out_of_range("L'animal avec l'id donne n'est pas trouve");
}

Ensemble Population::getIds() const {
    return idsUtilises;
}

// === Modificateurs ===
int Population::reserve() {
    if (idsUtilises.cardinal() >= MAXCARD)
        throw runtime_error("Plus d'identifiants disponibles!");

    for (int i = 0; i < MAXCARD; ++i) {
        if (!idsUtilises.contient(i)) {
            idsUtilises.ajoute(i);
            return i;
        }
    }

    throw runtime_error("Plus d'identifiants disponibles!");
}

void Population::set(const Animal& a) {
    for (Animal& existant : animaux) {
        if (existant.getId() == a.getId()) {
            existant = a;
            return;
        }
    }
    if (!idsUtilises.contient(a.getId())) {
        if (idsUtilises.cardinal() >= MAXCARD)
            throw runtime_error("Plus d'identifiants disponibles!");
        idsUtilises.ajoute(a.getId());
    }

    animaux.push_back(a);
}


void Population::supprime(int id) {
    for (size_t i = 0; i < animaux.size(); ++i) {
        if (animaux[i].getId() == id) {
            animaux.erase(animaux.begin() + i);
            idsUtilises.retire(id);
            return;
        }
    }
}


bool Population::existe(int id) const {
    for (const Animal& a : animaux) {
        if (a.getId() == id)
            return true;
    }
    return false;
}
