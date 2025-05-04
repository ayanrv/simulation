#ifndef GRILLE_HPP
#define GRILLE_HPP

#include "coord.hpp"
#include "population.hpp"
#include <iostream>

// Convention pour case vide
const int VIDE = -1;

const int FRAISE = -2;

// TAILLEGRILLE est 40

class Grille {
private:
    int grille[TAILLEGRILLE][TAILLEGRILLE]; // Contient les identifiants ou VIDE

public:
    // === Constructeur ===
    // Initialise toutes les cases a VIDE
    Grille();

    // === Accesseurs ===
    bool caseVide(const Coord& c) const;
    // Retourne l'identifiant de l'animal present dans la case
    int getCase(const Coord& c) const;
    bool contientFraise(const Coord&) const;

    // === Modificateurs ===
    // Place un animal dans une case
    void setCase(const Coord& c, int idAnimal);
    void videCase(const Coord& c);
    void placeFraise(const Coord&);

    void mangeFraise(const Coord&);

    // Affiche la grille avec L / R / . en fonction du contenu
    void affiche(const Population& pop, std::ostream& os) const;
};

#endif
