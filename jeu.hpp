//But: Jeu encapsule Grille + Population et orchestre la simulation
#ifndef JEU_HPP
#define JEU_HPP

#include "grille.hpp"
#include "population.hpp"
#include "ensemble.hpp"

class Jeu {
private:
    Grille grille;
    Population population;

    struct Fraise {
        Coord pos;
        int age;
    };
    
    vector<Fraise> fraises;
    double probLapin;
    double probRenard;

public:
    // Constructeur par defaut (grille vide, population vide)
    Jeu();

    // Constructeur avec probabilites (remplissage aleatoire)
    Jeu(double probLapins, double probRenards);

    const vector<Jeu::Fraise>& getFraises() const;
    //for test
    vector<Fraise>& getFraises(); // version modifiable


    // Ajoute un animal de type e a la coordonnée c
    void ajouteAnimal(Espece e, const Coord& c);

    // Verifie que chaque animal est bien a sa position dans la grille
    bool verifieGrille() const;

    // Retourne les voisines vides d'une case
    Ensemble voisinsVides(const Coord& c) const;

    // Retourne les voisines contenant une espece donnee
    Ensemble voisinsEspece(const Coord& c, Espece e) const;

    Ensemble voisinsOpposeSexe(const Coord& c, Espece espece, Sexe sexe) const;

    // Deplace l’animal vers une case voisine vide (si possible)
    void deplaceAnimal(int id, int foodGain, int maxFaim);

    // Acces direct a la grille et la population cause they are private
    const Grille& getGrille() const;
    //for test
    Grille& getGrille(); // version modifiable
    const Population& getPopulation() const;

    //check if grille and population are working right along after each tour
    bool checkConsistence();
    // Effectue un tour de simulation complet : deplacement, reproduction, mort
    void tour(double probReproLapin, int minFreeLapin,
        int foodInit, int foodReprod, int foodGain, int maxFaim,
        double probReproRenard, int nbFraisesMax);
    /*
    TOUR:
    1. LAPINS
        a. Move (if possible)
        b. Try to reproduce
    2. RENARDS
        a. Try to eat a Lapin
        b. Else: move or stand still
        c. Increase faim
        d. Die if starving
        e. Reproduce if conditions met
    */
    void affiche(ostream& os) const;

    void reset();
};

#endif