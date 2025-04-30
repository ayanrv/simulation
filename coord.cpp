#include "coord.hpp"
using namespace std;

// === Constructeurs ===

// Cree une coordonnee (ligne, colonne) apres verification des bornes
Coord::Coord(int l, int c) : lig(l), col(c) {
    if (l < 0 || l >= TAILLEGRILLE || c < 0 || c >= TAILLEGRILLE)
        throw out_of_range("Coordonnees hors de portee");
}

// Cree une coordonnee a partir d'un code entier
Coord::Coord(int code) {
    if (code < 0 || code >= TAILLEGRILLE * TAILLEGRILLE)
        throw out_of_range("Code de coordonnee invalide");
    lig = code / TAILLEGRILLE;
    col = code % TAILLEGRILLE;
}

// === Accesseurs ===
int Coord::getLig() const {
    return lig;
}
int Coord::getCol() const {
    return col;
}

// Retourne l'indice unique associe a la coordonnee
int Coord::toInt() const {
    return lig * TAILLEGRILLE + col;
}

// Convertir la coordonnee en chaine de caracteres
// just for the test
string Coord::toString() const {
    return "(" + to_string(lig) + ", " + to_string(col) + ")";
}

// Retourne les voisins valides
vector<Coord> Coord::voisins() const {
    vector<Coord> result;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue; // ne pas prendre soi-meme
            int newL = lig + i;
            int newC = col + j;
            if (newL >= 0 && newL < TAILLEGRILLE && newC >= 0 && newC < TAILLEGRILLE) {
                result.push_back(Coord(newL, newC));
            }
        }
    }
    return result;
}

// === Operateurs ===
bool Coord::operator==(const Coord& autre) const {
    return lig == autre.lig && col == autre.col;
}
bool Coord::operator!=(const Coord& autre) const {
    return lig != autre.lig || col != autre.col;
}

// === Affichage ===
ostream& operator<<(ostream& os, const Coord& c) {
    os << "(" << c.getLig() << ", " << c.getCol() << ")";
    return os;
}
