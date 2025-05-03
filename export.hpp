#ifndef EXPORT_HPP
#define EXPORT_HPP

#include <string>
#include "grille.hpp"
#include "population.hpp"

using namespace std;

// Export de la grille au format PPM (Lapin = vert, Renard = rouge, Vide = blanc)
void exportPPM(const Grille& g, const Population& pop, int step, int scale = 10);

#endif
