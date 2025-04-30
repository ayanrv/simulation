#include <iostream>
#include "coord.hpp"
#include "ensemble.hpp"
#include "animal.hpp"

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

    // Fonctionalite de la classe animal
    Animal lapin(1, Espece::Lapin, Coord(2, 3));
    Animal renard(2, Espece::Renard, Coord(4, 5));

    cout << "Lapin: ";
    lapin.affiche(cout);
    cout << endl;

    cout << "Renard: ";
    renard.affiche(cout);
    cout << endl;

    // Faire jeuner le renard plusieurs fois
    for (int i = 0; i < 5; ++i)
        renard.jeune();

    cout << "Renard apres 5 tours de jeune: ";
    renard.affiche(cout);
    cout << " — Meurt ? " << (renard.meurt() ? "Oui" : "Non") << endl;

    // Le lapin se reproduit ?
    int voisinsLibres = 3;
    cout << "Lapin se reproduit avec " << voisinsLibres << " voisins libres ? "
         << (lapin.seReproduit(voisinsLibres) ? "Oui" : "Non") << endl;

    return 0;
}
