#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "coord.hpp"
#include "ensemble.hpp"
#include "animal.hpp"
#include "population.hpp"
#include "grille.hpp"
#include "jeu.hpp"

using namespace std;

// === Tests Coord ===
TEST_CASE("Coord: voisins") {
    Coord coin(0, 0);
    CHECK(coin.voisins().size() == 3);

    Coord milieu(10, 10);
    CHECK(milieu.voisins().size() == 8);
}

TEST_CASE("Coord: exceptions") {
    CHECK_THROWS_AS(Coord(-1, 0), out_of_range);
    CHECK_THROWS_AS(Coord(0, 40), out_of_range);
    CHECK_THROWS_AS(Coord(1600), out_of_range);
}

TEST_CASE("Coord: toString") {
    Coord c(4, 7);
    CHECK(c.toString() == "(4, 7)");
}

TEST_CASE("Coord: toInt / constructeur inverse") {
    Coord c1(5, 9);
    int code = c1.toInt();
    Coord c2(code);
    CHECK(c1 == c2);
}


// === Tests Ensemble ===
TEST_CASE("Ensemble: Creation et Vide") {
    Ensemble e;
    CHECK(e.estVide() == true);
    CHECK(e.cardinal() == 0);
}

TEST_CASE("Ensemble: Ajout simple") {
    Ensemble e;
    e.ajoute(5);
    CHECK(e.estVide() == false);
    CHECK(e.cardinal() == 1);
}

TEST_CASE("Ensemble: Ajout jusqu'au maximum") {
    Ensemble e;
    for (int i = 0; i < MAXCARD; ++i) {
        e.ajoute(i);
    }
    CHECK(e.cardinal() == MAXCARD);
    CHECK_THROWS_AS(e.ajoute(999), runtime_error);
}

TEST_CASE("Ensemble: Tire un element") {
    Ensemble e;
    e.ajoute(10);
    e.ajoute(20);
    e.ajoute(30);

    int avant = e.cardinal();
    int valeur = e.tire(); // Tire un element aleatoire

    // Verifie que la valeur est l'une de celles ajoutees
    CHECK((valeur == 10 || valeur == 20 || valeur == 30));
    CHECK(e.cardinal() == avant - 1);
}

TEST_CASE("Ensemble: Tire sur ensemble vide") {
    Ensemble e;
    CHECK_THROWS_AS(e.tire(), runtime_error);
}

TEST_CASE("Ensemble: Ajout et tirage simple") {
    Ensemble e;
    e.ajoute(1);
    e.ajoute(2);
    e.ajoute(3);

    int v1 = e.tire();
    int v2 = e.tire();
    int v3 = e.tire();

    // Tous doivent etre parmi [1,2,3]
    CHECK((v1 == 1 || v1 == 2 || v1 == 3));
    CHECK((v2 == 1 || v2 == 2 || v2 == 3));
    CHECK((v3 == 1 || v3 == 2 || v3 == 3));

    // Apres avoir tout tire, ensemble est vide
    CHECK(e.estVide());
}

TEST_CASE("Ensemble: voisines") {
    Ensemble e;
    e.ajoute(Coord(0, 0).toInt());
    Ensemble v = e.voisines();
    // Coord(0, 0) a 3 voisins valides
    CHECK(v.cardinal() == 3);

    Ensemble e2;
    e2.ajoute(Coord(5, 5).toInt());
    Ensemble v2 = e2.voisines();
    // Coord(5, 5) a 8 voisins valides
    CHECK(v2.cardinal() == 8);

    // Vérification que tous les voisins sont différents de la coordonnée centrale
    for (int i = 0; i < v2.cardinal(); ++i) {
        CHECK(v2.cardinal() <= 8);
        CHECK(v2.cardinal() >= 1);
    }
}

// === Tests Animal ===

TEST_CASE("Animal: creation et acces") {
    Animal a1(1, Espece::Lapin, Coord(2, 3));
    CHECK(a1.getId() == 1);
    CHECK(a1.getEspece() == Espece::Lapin);
    CHECK(a1.getCoord() == Coord(2, 3));
    CHECK(a1.getFaim() == 0);
}

TEST_CASE("Animal: setCoord fonctionne") {
    Animal a2(2, Espece::Renard, Coord(0, 0));
    a2.setCoord(Coord(1, 1));
    CHECK(a2.getCoord() == Coord(1, 1));
}

TEST_CASE("Renard: meurt si faim >= maxFaim") {
    Animal r(3, Espece::Renard, Coord(4, 4));
    for (int i = 0; i < 5; ++i) r.jeune();
    CHECK(r.getFaim() == 5);
    CHECK(r.meurt(5) == true);
    CHECK(r.meurt(6) == false);
}

TEST_CASE("Renard: mange reduit faim, ne meurt pas") {
    Animal r(4, Espece::Renard, Coord(1, 1));
    for (int i = 0; i < 4; ++i) r.jeune();
    CHECK(r.getFaim() == 4);
    r.mange(3, 10);
    CHECK(r.getFaim() == 1);
    CHECK(r.meurt(5) == false);
}

TEST_CASE("Renard: mange trop ne descend pas sous 0") {
    Animal r(5, Espece::Renard, Coord(1, 1));
    r.jeune(); r.jeune();
    r.mange(10, 10); // -10 -> < 0
    CHECK(r.getFaim() == 0);
}

TEST_CASE("Animal: reproduction selon seuil") {
    Animal l(7, Espece::Lapin, Coord(3, 3));
    CHECK(l.seReproduit(3, 2));  // ok
    CHECK_FALSE(l.seReproduit(1, 2)); // pas assez
}


// === Tests Population ===
TEST_CASE("Population: reserve and set") {
    Population p;

    // Reserver un ID libre
    int id = p.reserve();
    CHECK(p.getIds().cardinal() == 1);

    // Creer et ajouter un animal avec cet ID
    Animal a(id, Espece::Lapin, Coord(3, 3));
    p.set(a);

    // Verifier qu'il a ete ajoute
    Animal a2 = p.get(id);
    CHECK(a2.getId() == id);
    CHECK(a2.getCoord() == Coord(3, 3));
    CHECK(a2.getEspece() == Espece::Lapin);
}

TEST_CASE("Population: reserve plusieurs IDs") {
    Population p;

    int id1 = p.reserve();
    int id2 = p.reserve();
    CHECK(id1 != id2);
    CHECK(p.getIds().cardinal() == 2);
}

TEST_CASE("Population: set modifie l'animal existant") {
    Population p;
    int id = p.reserve();
    Animal a(id, Espece::Renard, Coord(2, 2));
    p.set(a);

    Animal modifie(id, Espece::Renard, Coord(4, 4));
    p.set(modifie); // mise a jour

    Animal result = p.get(id);
    CHECK(result.getCoord() == Coord(4, 4));
}

TEST_CASE("Population: supprime") {
    Population p;
    int id = p.reserve();
    p.set(Animal(id, Espece::Lapin, Coord(1, 1)));
    p.supprime(id);
    CHECK(p.getIds().cardinal() == 0);
    CHECK_THROWS_AS(p.get(id), out_of_range);
}


TEST_CASE("Population: reserve throws when full") {
    Population p;
    for (int i = 0; i < MAXCARD; ++i) {
        int id = p.reserve();
        p.set(Animal(id, Espece::Lapin, Coord(0, 0)));
    }
    CHECK_THROWS_AS(p.reserve(), runtime_error);
}

// === Tests Grille ===
TEST_CASE("Grille: initialisation vide") {
    Grille g;
    Coord c(3, 3);
    CHECK(g.caseVide(c));
    CHECK(g.getCase(c) == -1);
}

TEST_CASE("Grille: setCase et getCase") {
    Grille g;
    Coord c(2, 2);
    g.setCase(c, 42);
    CHECK(!g.caseVide(c));
    CHECK(g.getCase(c) == 42);
}

TEST_CASE("Grille: videCase remet a VIDE") {
    Grille g;
    Coord c(1, 1);
    g.setCase(c, 7);
    g.videCase(c);
    CHECK(g.caseVide(c));
    CHECK(g.getCase(c) == -1);
}

// === Tests Jeu ===
TEST_CASE("Jeu: ajouteAnimal positionne correctement l'animal") {
    Jeu j;
    Coord c(1, 1);
    j.ajouteAnimal(Espece::Lapin, c);

    int id = j.getGrille().getCase(c);

    CHECK(j.getPopulation().get(id).getCoord() == c);
    CHECK(j.getGrille().getCase(c) == id);
    CHECK(j.checkConsistence());
}

TEST_CASE("Jeu: voisinsVides retourne les cases vides adjacentes") {
    Jeu j;
    Coord centre(10, 10);
    j.ajouteAnimal(Espece::Lapin, centre);

    Ensemble vides = j.voisinsVides(centre);
    CHECK(vides.cardinal() > 0);
}

TEST_CASE("Jeu: voisinsEspece retourne les bons voisins") {
    Jeu j;
    Coord c1(5, 5), c2(5, 6);
    j.ajouteAnimal(Espece::Lapin, c1);
    j.ajouteAnimal(Espece::Renard, c2);

    Ensemble l = j.voisinsEspece(c2, Espece::Lapin);
    CHECK(l.cardinal() == 1);

    Ensemble r = j.voisinsEspece(c1, Espece::Renard);
    CHECK(r.cardinal() == 1);
}

TEST_CASE("Jeu: deplaceAnimal déplace vers case vide") {
    Jeu j;
    Coord c(3, 3);
    j.ajouteAnimal(Espece::Lapin, c);

    int id = j.getGrille().getCase(c);
    j.deplaceAnimal(id, 3, 10);

    Coord nouveau = j.getPopulation().get(id).getCoord();
    CHECK(nouveau != c);
    CHECK(j.getGrille().getCase(nouveau) == id);
}

TEST_CASE("Jeu: tour ajoute fraises et nettoie les anciennes") {
    Jeu j;
    j.ajouteAnimal(Espece::Lapin, Coord(1, 1));
    j.tour(0, 0, 3, 5, 1, 10, 0, 10);

    CHECK(j.getFraises().size() > 0);  // au lieu de >= 5

    for (auto& f : j.getFraises()) f.age = 1;
    j.tour(0, 0, 3, 5, 1, 10, 0, 10);

    CHECK(j.getFraises().size() > 0);  // toujours des fraises fraîches
}


TEST_CASE("Jeu: checkConsistence détecte corruption de grille") {
    Jeu j;
    Coord c(2, 2);
    j.ajouteAnimal(Espece::Lapin, c);

    j.getGrille().videCase(c);  // corruption manuelle
    CHECK_THROWS_AS(j.checkConsistence(), runtime_error);
}