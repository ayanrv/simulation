#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include "coord.hpp"
#include <iostream>

using namespace std;

// === Type d'espece ===
enum class Espece { Lapin, Renard };

// === Sexe biologique ===
enum class Sexe { Male, Female };

class Animal {
private:
    int id;
    Espece espece;
    Coord position;
    Sexe sexe;
    int faim; // Niveau de faim

public:
    // === Constructeur ===
    Animal(int id, Espece espece, Coord position, Sexe sexe);

    // === Accesseurs ===
    int getId() const;
    Coord getCoord() const;
    void setCoord(Coord c);
    Espece getEspece() const;
    //to check conditions for reproduction (Renards)
    int getFaim() const;
    Sexe getSexe() const;

    // === Predicats ===
    bool meurt(int maxFaim) const;
    bool seReproduit(int nbVoisinsVides, int seuil) const;
    static Sexe sexeAleatoire();

    // === Modificateurs ===
    void mange(int foodGain, int maxFaim); // Remet la faim à 0
    void jeune(); // Incrémente la faim

    // === Affichage ===
    void affiche(ostream& os) const;
};

#endif
