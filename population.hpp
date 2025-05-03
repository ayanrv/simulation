#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <vector>
#include "animal.hpp"
#include "ensemble.hpp"
/*
We use Ensemble in the Population class because it stores a list of used animal IDs. 
Each animal has a Coord for its position, but we manage animals by their unique ID, 
not by their coordinates. 
So Ensemble is better suited than Coord for keeping track of IDs.
*/

using namespace std;

class Population {
private:
    vector<Animal> animaux;
    Ensemble idsUtilises;

public:
    // === Constructeur ===
    Population();

    // === Accesseurs ===
    Animal get(int id) const;
    /*
    We don’t need two versions of getId because id is a simple int that we never modify through a getter. 
    A single const int getId() const is enough for both read-only and regular access.
    */
    Ensemble getIds() const;

    // === Modificateurs ===
    int reserve();
    void set(const Animal& a);
    void supprime(int id);

    //for tests
    bool existe(int id) const;
};

#endif
