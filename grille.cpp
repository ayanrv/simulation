#include "grille.hpp"
using namespace std;

// Constructeur : met toutes les cases a VIDE
Grille::Grille() { //it is very important to initialize constructor or else it will mess with content inside
    for (int i = 0; i < TAILLEGRILLE; ++i) {
        for (int j = 0; j < TAILLEGRILLE; ++j) {
            grille[i][j] = VIDE; // i - line and j - column, your code confused that
        }
    }
}

//in your code larg and haut weren't declared and it is better to use Coord because it checks the interval and sizes itself 

// Retourne vrai si la case est vide
bool Grille::caseVide(const Coord& c) const {
    return grille[c.getLig()][c.getCol()] == VIDE;
}

// Retourne l'identifiant de l'animal a la case donnee
int Grille::getCase(const Coord& c) const {
    return grille[c.getLig()][c.getCol()];
}

// Place un identifiant dans une case
void Grille::setCase(const Coord& c, int idAnimal) {
    grille[c.getLig()][c.getCol()] = idAnimal;
}

// Vide la case (met VIDE)
void Grille::videCase(const Coord& c) {
    grille[c.getLig()][c.getCol()] = VIDE;
}

// Affiche la grille sur la sortie donnee
// Affiche un L pour un lapin, un R pour un renard, et . pour une case vide
// Color terminal output (ANSI escape codes)
void Grille::affiche(const Population& pop, ostream& os) const {
    for (int i = 0; i < TAILLEGRILLE; ++i) {
        for (int j = 0; j < TAILLEGRILLE; ++j) {
            int id = grille[i][j];

            if (id == VIDE) {
                os << " \033[37m.\033[0m";  // white
            } else {
                try {
                    Animal a = pop.get(id);
                    if (a.getEspece() == Espece::Lapin)
                        os << " \033[32mL\033[0m";  // green
                    else
                        os << " \033[31mR\033[0m";  // red
                } catch (...) {
                    os << " \033[33m?\033[0m"; // yellow for error
                }
            }
        }
        os << '\n';
    }
}