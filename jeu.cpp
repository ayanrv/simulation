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
    Animal a(id, e, c, Animal::sexeAleatoire());
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

void Jeu::deplaceAnimal(int id) {
    if (!population.existe(id)) return;

    Animal a = population.get(id);
    Coord pos = a.getCoord();
    vector<Coord> voisins = pos.voisins();

    // === 1. Chercher la nourriture prioritairement (lapin → fraise, renard → lapin)
    for (const Coord& v : voisins) {
        if (!grille.caseVide(v)) {
            int contenu = grille.getCase(v);
            if (a.getEspece() == Espece::Lapin && contenu == FRAISE) {
                grille.videCase(pos);
                a.setCoord(v);
                population.set(a);
                grille.videCase(v); // mange fraise
                grille.setCase(v, id);
                return;
            }
            if (a.getEspece() == Espece::Renard && contenu != FRAISE && population.get(contenu).getEspece() == Espece::Lapin) {
                grille.videCase(pos);
                grille.videCase(v); // mange lapin
                population.supprime(contenu);
                a.setCoord(v);
                population.set(a);
                grille.setCase(v, id);
                return;
            }
        }
    }

    // === 2. Sinon, aller vers une case vide aleatoire
    Ensemble libres = voisinsVides(pos);
    if (!libres.estVide()) {
        Coord cible(libres.tire());
        grille.videCase(pos);
        a.setCoord(cible);
        population.set(a);
        grille.setCase(cible, id);
    } else {
        population.set(a); // juste mettre a jour faim ou autre
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

    // === 1. Verification population → grille
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.get(i);

        // Verifie que l'animal existe vraiment
        if (!population.existe(id)) {
            throw runtime_error("Erreur: ID " + to_string(id) +
                                " est dans l'ensemble mais pas dans la population.");
        }

        const Animal& a = population.get(id);
        Coord c = a.getCoord();

        // Verifie que la grille contient bien cet animal a sa position
        if (grille.getCase(c) != id) {
            throw runtime_error("Erreur: animal ID " + to_string(id) +
                                " est a " + c.toString() +
                                " mais la grille contient " + to_string(grille.getCase(c)));
        }
    }

    // === 2. Verification grille → population
    for (int lig = 0; lig < TAILLEGRILLE; ++lig) {
        for (int col = 0; col < TAILLEGRILLE; ++col) {
            Coord c(lig, col);
            int id = grille.getCase(c);

            if (id == VIDE || id == FRAISE) continue;

            // Verifie que l'identifiant existe bien dans la population
            if (!population.existe(id)) {
                throw runtime_error("Erreur: la grille contient ID " + to_string(id) +
                                    " a " + c.toString() +
                                    " mais la population ne le contient pas.");
            }

            // Verifie la position reelle de l'animal
            if (population.get(id).getCoord().toInt() != c.toInt()) {
                throw runtime_error("Erreur: grille[" + c.toString() + "] = " + to_string(id) +
                                    ", mais l'animal est a " + population.get(id).getCoord().toString());
            }
        }
    }

    return true;
}

void Jeu::tour(double probReproLapin, int minFreeLapin,
    int foodInit, int foodReprod, int foodGain, int maxFaim,
    double probReproRenard, int nbFraisesMax) {
// === Phase 0 : Génération de fraises proportionnelle aux lapins ===
int nbLapins = 0;
Ensemble ids = population.getIds();
for (int i = 0; i < ids.cardinal(); ++i) {
if (population.existe(ids.get(i)) && population.get(ids.get(i)).getEspece() == Espece::Lapin)
 ++nbLapins;
}

double p = 0.4;
int nbFraisesParTour = std::min(nbFraisesMax, static_cast<int>(nbLapins * p));
int generated = 0, tries = 0;
while (generated < nbFraisesParTour && tries < 10 * nbFraisesParTour) {
Coord c(rand() % TAILLEGRILLE, rand() % TAILLEGRILLE);
if (grille.caseVide(c)) {
 grille.setCase(c, FRAISE);
 ++generated;
}
++tries;
}

// === Phase Lapins ===
Ensemble idsLapins;
for (int i = 0; i < ids.cardinal(); ++i) {
int id = ids.get(i);
if (population.existe(id) && population.get(id).getEspece() == Espece::Lapin)
 idsLapins.ajoute(id);
}

for (int i = 0; i < idsLapins.cardinal(); ++i) {
int id = idsLapins.get(i);
if (!population.existe(id)) continue;

Animal l = population.get(id);
Coord pos = l.getCoord();

// Vie
if (grille.getCase(pos) == FRAISE) {
 l.mange(foodGain, maxFaim);
 grille.videCase(pos);
} else {
 l.jeune();
}

population.set(l);
deplaceAnimal(id); // mange ou déplace si possible

// Reproduction
Animal l2 = population.get(id);  // après mouvement
Coord nouvPos = l2.getCoord();
Ensemble voisins = voisinsOpposeSexe(nouvPos, Espece::Lapin, l2.getSexe());
Ensemble vides = voisinsVides(nouvPos);

if (!voisins.estVide() && l2.getFaim() <= foodReprod && l2.seReproduit(vides.cardinal(), minFreeLapin)) {
 double r = static_cast<double>(rand()) / RAND_MAX;
 if (r < probReproLapin && !vides.estVide()) {
     Coord naissance(vides.tire());
     ajouteAnimal(Espece::Lapin, naissance);
 }
}
}

// === Phase Renards ===
Ensemble idsRenards;
for (int i = 0; i < ids.cardinal(); ++i) {
int id = ids.get(i);
if (population.existe(id) && population.get(id).getEspece() == Espece::Renard)
 idsRenards.ajoute(id);
}

for (int i = 0; i < idsRenards.cardinal(); ++i) {
int id = idsRenards.get(i);
if (!population.existe(id)) continue;

Animal r = population.get(id);
Coord pos = r.getCoord();

// Vie
bool aMange = false;
for (const Coord& v : pos.voisins()) {
 if (!grille.caseVide(v)) {
     int contenu = grille.getCase(v);
     if (contenu != FRAISE && population.get(contenu).getEspece() == Espece::Lapin) {
         population.supprime(contenu);
         grille.videCase(v);
         grille.videCase(pos);
         r.setCoord(v);
         r.mange(foodGain, maxFaim);
         population.set(r);
         grille.setCase(v, id);
         aMange = true;
         break;
     }
 }
}

if (!aMange) {
 r.jeune();
 population.set(r);
 deplaceAnimal(id);
}

// Mort de faim
if (r.meurt(maxFaim)) {
 grille.videCase(r.getCoord());
 population.supprime(id);
 continue;
}

// Reproduction
Animal r2 = population.get(id);
Coord nouvPos = r2.getCoord();
Ensemble voisins = voisinsOpposeSexe(nouvPos, Espece::Renard, r2.getSexe());
Ensemble vides = voisinsVides(nouvPos);

if (!voisins.estVide() && r2.getFaim() <= foodReprod && r2.seReproduit(vides.cardinal(), 1)) {
 double rr = static_cast<double>(rand()) / RAND_MAX;
 if (rr < probReproRenard && !vides.estVide()) {
     Coord naissance = vides.tire();
     Animal nouveau(population.reserve(), Espece::Renard, naissance, Animal::sexeAleatoire());
     for (int i = 0; i < foodInit; ++i) nouveau.jeune();
     population.set(nouveau);
     grille.setCase(naissance, nouveau.getId());
 }
}
}

checkConsistence();
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


