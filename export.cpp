#include "export.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Export PPM with optional zoom (each cell becomes scale×scale pixels)
void exportPPM(const Grille& g, const Population& pop, int step, int scale) {
    const int width = TAILLEGRILLE * scale;
    const int height = TAILLEGRILLE * scale;

    ostringstream filename;
    filename << "frames/grille" << setfill('0') << setw(4) << step << ".ppm";
    ofstream out(filename.str());

    // PPM header (P3 = text format)
    out << "P3\n" << width << " " << height << "\n255\n";

    for (int lig = 0; lig < TAILLEGRILLE; ++lig) {
        for (int repeatY = 0; repeatY < scale; ++repeatY) {
            for (int col = 0; col < TAILLEGRILLE; ++col) {
                Coord c(lig, col);

                int r = 255, gCol = 255, b = 255;  // Default: white

                int val = g.getCase(c);
                if (val == FRAISE) {
                    r = 255; gCol = 0; b = 0;  // Red for strawberry
                } else if (val != VIDE) {
                    Animal a = pop.get(val);
                    if (a.getEspece() == Espece::Lapin) {
                        r = 0; gCol = 255; b = 0;  // Green for rabbit
                    } else {
                        r = 255; gCol = 128; b = 0;  // Orange for fox
                    }
                }

                for (int repeatX = 0; repeatX < scale; ++repeatX)
                    out << r << " " << gCol << " " << b << " ";
            }
            out << '\n';
        }
    }

    out.close();
}
