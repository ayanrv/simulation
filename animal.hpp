#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include "coord.hpp"
#include <iostream>

using namespace std;

enum class Espece { Lapin, Renard };

class Animal {
private:
    int id;
    Espece espece;
    Coord position;
    int faim; // Niveau de faim

public:
    // === Constructeur ===
    Animal(int id, Espece espece, Coord position);

    // === Accesseurs ===
    int getId() const;
    Coord getCoord() const;
    void setCoord(Coord c);
    Espece getEspece() const;

    // === Predicats ===
    bool meurt() const;
    bool seReproduit(int nbVoisinsVides) const;

    // === Modificateurs ===
    void mange(); // Remet la faim à 0
    void jeune(); // Incrémente la faim

    // === Affichage ===
    void affiche(ostream& os) const;
};

#endif
