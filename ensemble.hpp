#ifndef ENSEMBLE_HPP
#define ENSEMBLE_HPP

#include <iostream>
#include <stdexcept>

const int MAXCARD = 1600; // taille maximale d'un ensemble

class Ensemble {
private:
    int t[MAXCARD]; // tableau d'entiers
    int card;       // nombre d'elements significatifs dans t

public:
    // === Constructeur ===
    Ensemble(); // initialise un ensemble vide

    // === Methodes ===
    bool estVide() const;         // renvoie true si l'ensemble est vide
    int cardinal() const;         // renvoie le nombre d'elements
    void ajoute(int val);         // ajoute un entier a l'ensemble
    int tire();                   // retire un element au hasard
    
    // for population and tests
    bool contient(int val) const;
    void retire(int val);

    //for jeu
    int get(int i) const;

    void affiche(std::ostream& os) const; // affiche les elements significatifs

    Ensemble voisines() const;
    
    // === Operateur ===
    friend std::ostream& operator<<(std::ostream& os, const Ensemble& e);
};

#endif