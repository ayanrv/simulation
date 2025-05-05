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
            if (id != FRAISE && population.get(id).getEspece() == e) {
                res.ajoute(v.toInt());
            }
        }
    }
    return res;
}

// Retourne les voisines contenant un animal de même espèce mais de sexe opposé
Ensemble Jeu::voisinsOpposeSexe(const Coord& c, Espece espece, Sexe sexe) const {
    Ensemble res;
    vector<Coord> voisins = c.voisins();
    for (const Coord& v : voisins) {
        if (!grille.caseVide(v)) {
            int id = grille.getCase(v);
            if (id != FRAISE) {
                const Animal& voisin = population.get(id);
                if (voisin.getEspece() == espece && voisin.getSexe() != sexe) {
                    res.ajoute(v.toInt());
                }
            }
        }
    }
    return res;
}

void Jeu::deplaceAnimal(int id, int foodGain, int maxFaim) {
    if (!population.existe(id)) return;

    Animal a = population.get(id);
    Coord pos = a.getCoord();
    vector<Coord> voisins = pos.voisins();

    for (const Coord& v : voisins) {
        if (!grille.caseVide(v)) {
            int contenu = grille.getCase(v);

            // 🍓 Lapin mange fraise
            if (a.getEspece() == Espece::Lapin && contenu == FRAISE) {
                grille.videCase(pos);
                a.setCoord(v);
                a.mange(foodGain, maxFaim);
                population.set(a);
                grille.videCase(v);
                grille.setCase(v, id);

                // Supprimer fraise de la liste
                for (auto it = fraises.begin(); it != fraises.end(); ++it) {
                    if (it->pos.toInt() == v.toInt()) {
                        fraises.erase(it);
                        break;
                    }
                }
                return;
            }

            // 🦊 Renard mange lapin
            if (a.getEspece() == Espece::Renard &&
                contenu != FRAISE &&
                population.existe(contenu) &&
                population.get(contenu).getEspece() == Espece::Lapin) {
                grille.videCase(pos);
                grille.videCase(v);
                population.supprime(contenu);
                a.setCoord(v);
                a.mange(foodGain, maxFaim);
                population.set(a);
                grille.setCase(v, id);
                return;
            }
        }
    }

    // Aucun repas → déplacement simple
    a.jeune();
    Ensemble libres = voisinsVides(pos);
    if (!libres.estVide()) {
        Coord cible(libres.tire());
        grille.videCase(pos);
        a.setCoord(cible);
        population.set(a);
        grille.setCase(cible, id);
    } else {
        population.set(a);
    }
}

// Acces a la grille
const Grille& Jeu::getGrille() const {
    return grille;
}

Grille& Jeu::getGrille() {
    return grille;
}


// Acces a la population
const Population& Jeu::getPopulation() const {
    return population;
}

bool Jeu::checkConsistence() {
    Ensemble ids = population.getIds();

    // === 1. Vérification population → grille
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.get(i);

        if (!population.existe(id)) {
            throw runtime_error("Erreur: ID " + to_string(id) +
                                " est dans l'ensemble mais pas dans la population.");
        }

        const Animal& a = population.get(id);
        Coord c = a.getCoord();

        if (grille.getCase(c) != id) {
            throw runtime_error("Erreur: animal ID " + to_string(id) +
                                " est a " + c.toString() +
                                " mais la grille contient " + to_string(grille.getCase(c)));
        }
    }

    // === 2. Vérification grille → population
    for (int lig = 0; lig < TAILLEGRILLE; ++lig) {
        for (int col = 0; col < TAILLEGRILLE; ++col) {
            Coord c(lig, col);
            int val = grille.getCase(c);

            if (val == VIDE || val == FRAISE) continue;

            if (!population.existe(val)) {
                throw runtime_error("Erreur: grille contient ID " + to_string(val) +
                                    " a " + c.toString() +
                                    " mais la population ne le contient pas.");
            }

            if (population.get(val).getCoord().toInt() != c.toInt()) {
                throw runtime_error("Erreur: grille[" + c.toString() + "] = " + to_string(val) +
                                    ", mais l'animal est a " + population.get(val).getCoord().toString());
            }
        }
    }

    // === 3. Nettoyage des fraises incohérentes
    for (auto it = fraises.begin(); it != fraises.end();) {
        if (grille.getCase(it->pos) != FRAISE) {
            it = fraises.erase(it); // fraise disparue de la grille, on la retire aussi
        } else {
            ++it;
        }
    }

    return true;
}



void Jeu::tour(double probReproLapin, int minFreeLapin,
    int foodInit, int foodReprod, int foodGain, int maxFaim,
    double probReproRenard, int nbFraisesMax) {

   // === Vieillissement fraises ===
   for (auto it = fraises.begin(); it != fraises.end();) {
        if (--(it->age) <= 0) {
            grille.videCase(it->pos);
            it = fraises.erase(it);
        } else {
            ++it;
        }
    }

    // === Génération de fraises ===
    int nbLapins = 0;
    Ensemble ids = population.getIds();
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.get(i);
        if (population.existe(id) &&
            population.get(id).getEspece() == Espece::Lapin)
            ++nbLapins;
    }

    int nbFraisesCibles = min(nbFraisesMax, static_cast<int>(nbLapins * 0.95));
    int aGenerer = max(5, nbFraisesCibles - static_cast<int>(fraises.size()));
    int generated = 0, tries = 0;

    while (generated < aGenerer && tries < 50 * aGenerer) {
        Coord c(rand() % TAILLEGRILLE, rand() % TAILLEGRILLE);
        if (grille.caseVide(c)) {
            grille.setCase(c, FRAISE);
            fraises.push_back({c, 5});
            ++generated;
        }
        ++tries;
    }

    // === Lapins ===
    Ensemble idsLapins;
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.get(i);
        if (population.existe(id) &&
            population.get(id).getEspece() == Espece::Lapin)
            idsLapins.ajoute(id);
    }

    for (int i = 0; i < idsLapins.cardinal(); ++i) {
        int id = idsLapins.get(i);
        if (!population.existe(id)) continue;

        deplaceAnimal(id, foodGain, maxFaim);
        if (!population.existe(id)) continue;

        Animal l = population.get(id);
        Coord pos = l.getCoord();
        Ensemble partenaires = voisinsOpposeSexe(pos, Espece::Lapin, l.getSexe());
        Ensemble vides = voisinsVides(pos);

        if (!partenaires.estVide() &&
            l.getFaim() <= foodReprod &&
            l.seReproduit(vides.cardinal(), minFreeLapin)) {
            double r = (double)rand() / RAND_MAX;
            if (r < probReproLapin && !vides.estVide()) {
                Coord naissance(vides.tire());
                ajouteAnimal(Espece::Lapin, naissance);
            }
        }
    }

    // === Renards ===
    Ensemble idsRenards;
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.get(i);
        if (population.existe(id) &&
            population.get(id).getEspece() == Espece::Renard)
            idsRenards.ajoute(id);
    }

    for (int i = 0; i < idsRenards.cardinal(); ++i) {
        int id = idsRenards.get(i);
        if (!population.existe(id)) continue;

        deplaceAnimal(id, foodGain, maxFaim);
        if (!population.existe(id)) continue;

        Animal r = population.get(id);
        if (r.meurt(maxFaim)) {
            grille.videCase(r.getCoord());
            population.supprime(id);
            continue;
        }

        Coord pos = r.getCoord();
        Ensemble partenaires = voisinsOpposeSexe(pos, Espece::Renard, r.getSexe());
        Ensemble vides = voisinsVides(pos);

        if (!partenaires.estVide() &&
            r.getFaim() <= foodReprod &&
            r.seReproduit(vides.cardinal(), 1)) {
            double rr = (double)rand() / RAND_MAX;
            if (rr < probReproRenard && !vides.estVide()) {
                Coord naissance(vides.tire());
                Animal nouveau(population.reserve(), Espece::Renard, naissance);
                for (int i = 0; i < foodInit; ++i) nouveau.jeune();
                population.set(nouveau);
                grille.setCase(naissance, nouveau.getId());
            }
        }
    }

    checkConsistence();
}


const vector<Jeu::Fraise>& Jeu::getFraises() const {
    return fraises;
}

vector<Jeu::Fraise>& Jeu::getFraises() {
    return fraises;
}

void Jeu::affiche(ostream& os) const {
    grille.affiche(population, os);

    int countL = 0, countR = 0, countFraises = 0;
    int totalFaimLapins = 0, lapinsAvecFaim = 0;
    int totalFaimRenards = 0, renardsAvecFaim = 0;

    Ensemble ids = population.getIds();
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.get(i);
        if (!population.existe(id)) continue;

        const Animal& a = population.get(id);
        if (a.getEspece() == Espece::Lapin) {
            ++countL;
            totalFaimLapins += a.getFaim();
            ++lapinsAvecFaim;
        } else if (a.getEspece() == Espece::Renard) {
            ++countR;
            totalFaimRenards += a.getFaim();
            ++renardsAvecFaim;
        }
    }

    for (int lig = 0; lig < TAILLEGRILLE; ++lig) {
        for (int col = 0; col < TAILLEGRILLE; ++col) {
            if (grille.getCase(Coord(lig, col)) == FRAISE) {
                ++countFraises;
            }
        }
    }

    int totalCases = TAILLEGRILLE * TAILLEGRILLE;
    int occupied = countL + countR;
    int empty = totalCases - occupied - countFraises;

    os << "\nStatistiques:\n";
    os << "Lapins    : " << countL << '\n';
    os << "Renards   : " << countR << '\n';
    os << "Fraises   : " << countFraises << '\n';
    os << "Vides     : " << empty << '\n';

    if (lapinsAvecFaim > 0) {
        double faimLapin = totalFaimLapins / double(lapinsAvecFaim);
        os << "Faim moyenne des lapins  : " << fixed << setprecision(2) << faimLapin << '\n';
    }
    
    if (renardsAvecFaim > 0) {
        double faimRenard = totalFaimRenards / double(renardsAvecFaim);
        os << "Faim moyenne des renards : " << fixed << setprecision(2) << faimRenard << '\n';
    }    
}

void Jeu::reset() {
    grille = Grille();
    population = Population();
    fraises.clear();
    srand(time(nullptr));
}
