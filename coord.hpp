#ifndef COORD_HPP
#define COORD_HPP

#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

using namespace std;

const int TAILLEGRILLE = 40; // Taille de la grille carree

class Coord {
private:
    int lig;
    int col;

public:
    // === Constructeurs ===
    Coord(int l, int c);     // Cree une coordonnee a partir de (ligne, colonne)
    Coord(int code);         // Cree une coordonnee a partir d'un entier code

    // === Accesseurs ===
    int getLig() const;
    int getCol() const;

    // === Fonctions utilitaires ===
    int toInt() const;                    // Encode la coordonnee en un entier unique
    std::string toString() const;         // Retourne la coordonnee sous forme de texte
    std::vector<Coord> voisins() const;   // Retourne la liste des voisins valides

    // === Opérateurs ===
    bool operator==(const Coord& autre) const;
    bool operator!=(const Coord& autre) const;

    // === Affichage ===
    friend ostream& operator<<(std::ostream& os, const Coord& c);
};

#endif
