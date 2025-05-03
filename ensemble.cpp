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
    }

    /*
    This code is needed so that stand() (randomness setting) is called only once 
    for the entire duration of the program. If you call it many times in a row, 
    the numbers may repeat, because the time hardly changes. This way we get more 
    authentic random numbers.
    */
    static bool isSeeded = false;
    if (!isSeeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        isSeeded = true;
    } // initialiser la graine de random une fois au debut 


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

bool Ensemble::contient(int val) const {
    for (int i = 0; i < card; ++i) {
        if (t[i] == val) return true;
    }
    return false;
}

void Ensemble::retire(int val) {
    for (int i = 0; i < card; ++i) {
        if (t[i] == val) {
            t[i] = t[card - 1];  // the deleted one changes for the last one 
            --card;
            return;
        }
    }
}

int Ensemble::get(int i) const {
    if (i < 0 || i >= card) {
        throw out_of_range("Index hors limites dans Ensemble::get()");
    }
    return t[i];
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