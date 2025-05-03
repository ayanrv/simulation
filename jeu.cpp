#include "jeu.hpp"
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <vector>
#include <iomanip>

using namespace std;

Jeu::Jeu() : grille(), population() {}

// Constructeur avec probabilites : remplit la grille aleatoirement
Jeu::Jeu(double probLapins, double probRenards) : grille(), population() {
    srand(time(nullptr));
    int compteur = 0;

    for (int lig = 0; lig < TAILLEGRILLE && compteur < MAXCARD; ++lig) {
        for (int col = 0; col < TAILLEGRILLE && compteur < MAXCARD; ++col) {
            double r = static_cast<double>(rand()) / RAND_MAX;
            Coord c(lig, col);

            if (r < probLapins) {
                ajouteAnimal(Espece::Lapin, c);
                ++compteur;
            } else if (r < probLapins + probRenards) {
                ajouteAnimal(Espece::Renard, c);
                ++compteur;
            }
        }
    }
}



// Ajoute un animal a la grille et a la population
void Jeu::ajouteAnimal(Espece e, const Coord& c) {
    if (!grille.caseVide(c)) {
        throw runtime_error("Case deja occupee");
    }
    int id = population.reserve();
    Animal a(id, e, c);
    population.set(a);
    grille.setCase(c, id);
}

// Verifie que chaque animal est bien a la position indiquee par la grille
bool Jeu::verifieGrille() const {
    Ensemble ids = population.getIds();
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.get(i);
        const Animal& a = population.get(id);
        if (grille.getCase(a.getCoord()) != id) return false;
    }
    return true;
}

// Retourne les voisines vides d'une case
Ensemble Jeu::voisinsVides(const Coord& c) const {
    Ensemble res;
    vector<Coord> voisins = c.voisins();
    for (const Coord& v : voisins) {
        if (grille.caseVide(v)) {
            res.ajoute(v.toInt());
        }
    }
    return res;
}

// Retourne les voisines contenant une espece specifique
Ensemble Jeu::voisinsEspece(const Coord& c, Espece e) const {
    Ensemble res;
    vector<Coord> voisins = c.voisins();
    for (const Coord& v : voisins) {
        if (!grille.caseVide(v)) {
            int id = grille.getCase(v);
            if (population.get(id).getEspece() == e) {
                res.ajoute(v.toInt());
            }
        }
    }
    return res;
}

// Deplace un animal vers une case vide voisine si possible
void Jeu::deplaceAnimal(int id) {
    if (!population.existe(id)) return;

    Animal a = population.get(id);
    Ensemble libres = voisinsVides(a.getCoord());

    if (!libres.estVide()) {
        Coord nouvelle(libres.tire());
        grille.videCase(a.getCoord());
        a.setCoord(nouvelle);
        population.set(a);
        grille.setCase(nouvelle, id);
    }
}

// Acces a la grille
const Grille& Jeu::getGrille() const {
    return grille;
}

// Acces a la population
const Population& Jeu::getPopulation() const {
    return population;
}

bool Jeu::checkConsistence() const {
    Ensemble ids = population.getIds();

    // 1. Verification population → grille
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.get(i);
        if (!population.existe(id)) {
            throw runtime_error("Erreur: ID " + to_string(id) + " existe dans l'ensemble mais pas dans la population.");
        }

        const Animal& a = population.get(id);
        Coord c = a.getCoord();

        if (grille.getCase(c) != id) {
            throw runtime_error("Erreur: animal ID " + to_string(id) +
                                " se trouve en " + c.toString() +
                                ", mais la grille contient " +
                                to_string(grille.getCase(c)));
        }
    }

    // 2. Verification grille → population
    for (int lig = 0; lig < TAILLEGRILLE; ++lig) {
        for (int col = 0; col < TAILLEGRILLE; ++col) {
            Coord c(lig, col);
            int id = grille.getCase(c);

            if (id != VIDE) {
                if (!population.existe(id)) {
                    throw runtime_error("Erreur: grille contient ID " + to_string(id) +
                                        " a " + c.toString() +
                                        ", mais la population ne le contient pas.");
                }

                if (population.get(id).getCoord().toInt() != c.toInt()) {
                    throw runtime_error("Erreur: grille[" + c.toString() + "] = " + to_string(id) +
                                        ", mais l'animal ID " + to_string(id) +
                                        " est cense etre a " + population.get(id).getCoord().toString());
                }
            }
        }
    }

    return true;
}

void Jeu::tour(double probReproLapin, int minFreeLapin,
    int foodInit, int foodReprod, int foodGain, int maxFaim, double probReproRenard) {
    // === Phase 1: Lapons bougent et se reproduisent
    Ensemble idsLapins;
    Ensemble ids = population.getIds();
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.get(i);
        if (!population.existe(id)) continue;
        if (population.get(id).getEspece() == Espece::Lapin)
        idsLapins.ajoute(id);
    }

    for (int i = 0; i < idsLapins.cardinal(); ++i) {
        int id = idsLapins.get(i);
        if (!population.existe(id)) continue;

        Animal l = population.get(id);
        Ensemble libres = voisinsVides(l.getCoord());

        if (!libres.estVide()) {
            Coord cible(libres.tire());
            grille.videCase(l.getCoord());
            l.setCoord(cible);
            population.set(l);
            grille.setCase(cible, id);
        }

        // reproduction
        libres = voisinsVides(l.getCoord());
        if (l.seReproduit(libres.cardinal(), minFreeLapin)) {
            double r = (double)rand() / RAND_MAX;
            if (r < probReproLapin && !libres.estVide()) {
                Coord naissance(libres.tire());
                ajouteAnimal(Espece::Lapin, naissance);
            }
        }
    }

    // === Phase 2: Renards bougent, mangent, meurent, se reproduisent
    Ensemble idsRenards;
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.get(i);
        if (!population.existe(id)) continue;
        if (population.get(id).getEspece() == Espece::Renard)
        idsRenards.ajoute(id);
    }

    for (int i = 0; i < idsRenards.cardinal(); ++i) {
        int id = idsRenards.get(i);
        if (!population.existe(id)) continue;

        Animal r = population.get(id);
        Coord pos = r.getCoord();
        Ensemble lapins = voisinsEspece(pos, Espece::Lapin);

        if (!lapins.estVide()) {
            Coord cible(lapins.tire());
            int idLapin = grille.getCase(cible);
            population.supprime(idLapin);
            grille.videCase(cible);
            grille.videCase(pos);

            r.setCoord(cible);
            r.mange(foodGain, maxFaim);
            population.set(r);
            grille.setCase(cible, id);
        } else {
            r.jeune();  // faim += 1
            Ensemble libres = voisinsVides(pos);
            if (!libres.estVide()) {
                Coord cible(libres.tire());
                grille.videCase(pos);
                r.setCoord(cible);
                population.set(r);
                grille.setCase(cible, id);
            } else {
                population.set(r); // juste mettre a jour faim
            }
        }

        if (r.meurt(maxFaim)) {
            grille.videCase(r.getCoord());
            population.supprime(id);
            continue;
        }

        // Reproduction renard
        Ensemble vides = voisinsVides(r.getCoord());
        if (r.seReproduit(vides.cardinal(), 1) && r.getFaim() <= foodReprod) {
            double rr = (double)rand() / RAND_MAX;
            if (rr < probReproRenard && !vides.estVide()) {
                Coord naissance(vides.tire());
                Animal nouveau(population.reserve(), Espece::Renard, naissance);
                for (int i = 0; i < foodInit; ++i) nouveau.jeune(); // faim = foodInit
                population.set(nouveau);
                grille.setCase(naissance, nouveau.getId());
            }
        }
    }
    // === Verification globale
    checkConsistence();
}

void Jeu::affiche(ostream& os) const {
    grille.affiche(population, os);

    // Statistiques
    int countL = 0, countR = 0;
    int totalFaim = 0;
    int renardsAvecFaim = 0;

    Ensemble ids = population.getIds();
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.get(i);
        if (!population.existe(id)) continue;

        const Animal& a = population.get(id);
        if (a.getEspece() == Espece::Lapin) {
            ++countL;
        } else {
            ++countR;
            totalFaim += a.getFaim();
            ++renardsAvecFaim;
        }
    }

    int totalCases = TAILLEGRILLE * TAILLEGRILLE;
    int empty = totalCases - countL - countR;

    os << "\nStatistiques:\n";
    os << "Lapins   : " << countL << '\n';
    os << "Renards  : " << countR << '\n';
    os << "Vides    : " << empty << '\n';

    if (renardsAvecFaim > 0) { // To see Renard's health (faim) status
        double moyenneFaim = static_cast<double>(totalFaim) / renardsAvecFaim;
        os << "Faim moyenne des renards : " << fixed << setprecision(2) << moyenneFaim << '\n';
    }
}

