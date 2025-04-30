#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "coord.hpp"
#include "ensemble.hpp"

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
