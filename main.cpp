#include <iostream>
#include "coord.hpp"
#include "ensemble.hpp"

using namespace std;

int main() {
    Coord c(5, 7);
    cout << "Coordonnee: " << c.toString() << endl;
    cout << "Code de la coordonnee: " << c.toInt() << endl;

    Ensemble e;
    cout << "L'ensemble est-il vide ? " << (e.estVide() ? "Oui" : "Non") << endl;

    e.ajoute(c.toInt());
    cout << "Ajout d'une coordonnee." << endl;
    cout << "Ensemble: " << e << endl;

    Ensemble v = e.voisines();
    cout << "Voisines de l'ensemble : " << v << endl;

    int code = e.tire();
    Coord tirage(code);
    cout << "Coordonnee tiree: " << tirage.toString() << endl;

    cout << "L'ensemble est-il vide apres tirage ? " << (e.estVide() ? "Oui" : "Non") << endl;

    return 0;
}
