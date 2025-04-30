#include "ensemble.hpp"
#include "coord.hpp"
#include <cstdlib>
#include <ctime>
using namespace std;

// === Constructeur ===
Ensemble::Ensemble() : card(0) {
    // Initialement, l'ensemble est vide, rien d'autre a faire
}

// === Methodes ===
bool Ensemble::estVide() const {
    return card == 0;
}

int Ensemble::cardinal() const {
    return card;
}

void Ensemble::ajoute(int val) {
    if (card >= MAXCARD) {
        throw runtime_error("Erreur: ensemble plein !");
    }
    // Empêcher les doublons
    for (int i = 0; i < card; ++i) {
        if (t[i] == val) return;
    }
    t[card] = val;
    ++card;
}

int Ensemble::tire() {
    if (estVide()) {
        throw runtime_error("Erreur: ensemble vide !");
    } // хаха сранд лол
    srand(time(nullptr)); // initialiser la graine de random a chaque appel
    int index = rand() % card; // tire un index valide
    int valeur = t[index];

    // Remplacer l'element tire par le dernier de l'ensemble
    t[index] = t[card - 1];
    --card;

    return valeur;
}

Ensemble Ensemble::voisines() const {
    Ensemble resultat;
    for (int i = 0; i < card; ++i) {
        Coord c(t[i]);
        vector<Coord> voisins = c.voisins();
        for (const Coord& v : voisins) {
            resultat.ajoute(v.toInt());
        }
    }
    return resultat;
}

void Ensemble::affiche(ostream& os) const {
    os << "{ ";
    for (int i = 0; i < card; ++i) {
        os << t[i];
        if (i < card - 1) {
            os << ", ";
        }
    }
    os << " }";
}

// === Operateur ===
ostream& operator<<(ostream& os, const Ensemble& e) {
    e.affiche(os);
    return os;
}