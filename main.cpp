#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "coord.hpp"
#include "ensemble.hpp"
#include "animal.hpp"
#include "population.hpp"
#include "grille.hpp"
#include "export.hpp"
#include "jeu.hpp"

using namespace std;

int main() {
    // === Coord ===
    // cout << "--- Class Coord ---" << endl;
    // Coord c(5, 7);
    // cout << "Coordonnee: " << c.toString() << endl;
    // cout << "Code de la coordonnee: " << c.toInt() << endl;

    // === Ensemble ===
    // cout << "\n--- Class Ensemble ---" << endl;
    // Ensemble e;
    // cout << "L'ensemble est-il vide ? " << (e.estVide() ? "Oui" : "Non") << endl;
    // e.ajoute(c.toInt());
    // cout << "Ajout d'une coordonnee." << endl;
    // cout << "Ensemble: " << e << endl;

    // Ensemble v = e.voisines();
    // cout << "Voisines de l'ensemble : " << v << endl;

    // int code = e.tire();
    // Coord tirage(code);
    // cout << "Coordonnee tiree: " << tirage.toString() << endl;
    // cout << "L'ensemble est-il vide apres tirage ? " << (e.estVide() ? "Oui" : "Non") << endl;

    // === Animal ===
    // cout << "\n--- Class Animal ---" << endl;
    // Animal lapin1(1, Espece::Lapin, Coord(2, 3));
    // Animal renard1(2, Espece::Renard, Coord(4, 5));

    // cout << "-- Animaux --" << endl;
    // cout << "Lapin: ";
    // lapin1.affiche(cout); cout << endl;
    // cout << "Renard: ";
    // renard1.affiche(cout); cout << endl;

    // for (int i = 0; i < 5; ++i)
    //     renard1.jeune();
    // cout << "Renard apres 5 tours de jeune: ";
    // renard1.affiche(cout);
    // cout << " — Meurt ? " << (renard1.meurt() ? "Oui" : "Non") << endl;

    // cout << "Lapin se reproduit avec 3 voisins libres ? "
    //      << (lapin1.seReproduit(3) ? "Oui" : "Non") << endl;

    // === Population ===
    // cout << "\n--- Class Population ---" << endl;
    // Population pop1;

    // // Réserver et ajouter des animaux
    // int id1 = pop1.reserve();
    // int id2 = pop1.reserve();
    // int id3 = 99; // test set() avec ID non réservé

    // Animal lapin(id1, Espece::Lapin, Coord(1, 1));
    // Animal renard(id2, Espece::Renard, Coord(2, 2));
    // Animal intrus(id3, Espece::Renard, Coord(3, 3));

    // pop1.set(lapin);
    // pop1.set(renard);

    // cout << "Population après ajout:" << endl;
    // pop1.get(id1).affiche(cout); cout << endl;
    // pop1.get(id2).affiche(cout); cout << endl;

    // cout << "L'animal avec ID " << id1 << " existe ? " << (pop1.existe(id1) ? "Oui" : "Non") << endl;
    // cout << "ID " << id3 << " existe ? " << (pop1.existe(id3) ? "Oui" : "Non") << endl;

    // Tentative d'ajout avec un ID non réservé => doit appeler ajoute
    // try {
    //     pop1.set(intrus);
    //     cout << "Ajout de l'intrus avec ID non réservé: OK" << endl;
    // } catch (const exception& e) {
    //     cout << "Erreur attendue: " << e.what() << endl;
    // }

    // cout << "Population après ajout de l'intrus:" << endl;
    // for (int id : {id1, id2, id3}) {
    //     if (pop1.existe(id)) {
    //         pop1.get(id).affiche(cout);
    //         cout << endl;
    //     }
    // }

    // Suppression d’un animal
    // pop1.supprime(id2);
    // cout << "Après suppression de l'animal " << id2 << ":" << endl;
    // cout << "Existe ? " << (pop1.existe(id2) ? "Oui" : "Non") << endl;

    // // Tentative d'accès
    // try {
    //     pop1.get(id2).affiche(cout);
    // } catch (const exception& e) {
    //     cout << "Erreur attendue après suppression: " << e.what() << endl;
    // }

    // cout << "\n--- Simulation GRILLE ---" << endl;

    // // === Parametres ===
    // const int nbLapins = 50;
    // const int nbRenards = 10;
    // const int total = nbLapins + nbRenards;

    // if (total >= TAILLEGRILLE * TAILLEGRILLE) {
    //     cerr << "Erreur: trop d'animaux pour la grille." << endl;
    //     return 1;
    // }

    // // === Initialisation ===
    // Population pop;
    // Grille g;

    // // Tous les indices possibles (0 à 1600)
    // vector<int> coords;
    // for (int lig = 0; lig < TAILLEGRILLE; ++lig) {
    //     for (int col = 0; col < TAILLEGRILLE; ++col) {
    //         coords.push_back(Coord(lig, col).toInt());
    //     }
    // }

    // // Mélange
    // srand(time(nullptr));
    // for (size_t i = coords.size() - 1; i > 0; --i) {
    //     size_t j = rand() % (i + 1);
    //     swap(coords[i], coords[j]);
    //     }

    // // Place les lapins
    // for (int i = 0; i < nbLapins; ++i) {
    //     Coord c(coords[i]);
    //     int id = pop.reserve();
    //     Animal l(id, Espece::Lapin, c);
    //     pop.set(l);
    //     g.setCase(c, id);
    // }

    // // Place les renards
    // for (int i = nbLapins; i < nbLapins + nbRenards; ++i) {
    //     Coord c(coords[i]);
    //     int id = pop.reserve();
    //     Animal r(id, Espece::Renard, c);
    //     pop.set(r);
    //     g.setCase(c, id);
    // }

    // // === Affichage grille et stats ===
    // int countL = 0, countR = 0;

    // cout << "\nGrille (L = Lapin, R = Renard, . = vide):\n";
    // for (int lig = 0; lig < TAILLEGRILLE; ++lig) {
    //     for (int col = 0; col < TAILLEGRILLE; ++col) {
    //         Coord c(lig, col);
    //         if (g.caseVide(c)) {
    //             cout << " \033[37m.\033[0m"; // white dot ANSI code
    //         } else {
    //             Animal a = pop.get(g.getCase(c));
    //             if (a.getEspece() == Espece::Lapin) {
    //                 cout << " \033[32mL\033[0m"; // green Lapin ANSI code
    //                 ++countL;
    //             } else {
    //                 cout << " \033[31mR\033[0m"; // red Renard ANSI code
    //                 ++countR;
    //             }
    //         }
    //     }
    //     cout << '\n';
    // }

    // cout << "\nStatistiques:\n";
    // cout << "Lapins  : " << countL << endl;
    // cout << "Renards : " << countR << endl;
    // cout << "Cases vides : " << (TAILLEGRILLE * TAILLEGRILLE - countL - countR) << endl;
    
    // // === Export graphique en couleur ===
    // int step = 0;
    // exportPPM(g, pop, step); // sauvegarde frames/grille0000.ppm

    // // Affichage clair du chemin
    // cout << "Image exportee : frames/grille"
    //      << setfill('0') << setw(4) << step << ".ppm" << endl;


    // === Paramètres de simulation ===
    const int nbTours = 20;
    const double probLapinInit = 0.22;
    const double probRenardInit = 0.07;

    const double probReproLapin = 0.45;
    const int minFreeLapin = 2;

    const int foodInit = 7;         // ⬆ plus de nourriture initiale
    const int foodReprod = 4;       // ⬇ reproduction plus facile
    const int foodGain = 6;         // ⬆ gain de nourriture
    const int maxFaim = 12;         // ⬆ tolérance à la faim

    const double probReproRenard = 0.3;
    const int nbFraisesMax = 150;   // ⬆ plus de fraises autorisées

    // === Initialisation du jeu ===
    Jeu jeu(probLapinInit, probRenardInit);
    cout << "--- Simulation (0) ---" << endl;
    jeu.affiche(cout);
    exportPPM(jeu.getGrille(), jeu.getPopulation(), 0);

    // === Boucle de simulation ===
    for (int step = 1; step <= nbTours; ++step) {
        cout << "\n--- Simulation (" << step << ") ---" << endl;

        try {
            jeu.tour(probReproLapin, minFreeLapin,
                     foodInit, foodReprod, foodGain, maxFaim,
                     probReproRenard, nbFraisesMax);
        } catch (const exception& e) {
            cerr << "[Erreur] Une exception a ete levee: " << e.what() << endl;
            break;
        }

        jeu.affiche(cout);
        exportPPM(jeu.getGrille(), jeu.getPopulation(), step);
    }

    cout << "\nSimulation terminee. Images enregistrees dans le dossier 'frames/'." << endl;

    // === Génération automatique GIF ===
    cout << "\nTentative de génération d'une animation GIF avec ImageMagick..." << endl;
    int result = system("convert -delay 10 -loop 0 frames/grille*.ppm animation.gif");

    if (result == 0) {
        cout << "GIF genere avec succes - animation.gif (in terminal: xdg-open animation.gif)" << endl;
    } else {
        cerr << "Echec de la commande 'convert'. Assurez-vous qu'ImageMagick est installe." << endl;
    }


    return 0;
}
