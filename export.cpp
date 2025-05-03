#include "export.hpp"
#include <fstream>   // for ofstream (file output)
#include <sstream>   // for ostringstream (filename building)
#include <iomanip>   // for setfill and setw (zero-padded numbers)

using namespace std;

// Export the grid as a PPM (Portable Pixmap) image file.
// Each cell is colored based on the species:
//   - White for empty
//   - Green for Lapin (rabbit)
//   - Red for Renard (fox)
// The filename is generated based on the step number: frames/grilleXXXX.ppm
void exportPPM(const Grille& g, const Population& pop, int step) {
    // Construct the filename with leading zeroes (e.g., grille0001.ppm)
    ostringstream filename;
    filename << "frames/grille" << setfill('0') << setw(4) << step << ".ppm";

    // Open file for writing
    ofstream out(filename.str());

    // Write the PPM header
    out << "P3\n"                         // Magic number for plain text PPM
        << TAILLEGRILLE << " " << TAILLEGRILLE << "\n"  // Image width and height
        << "255\n";                      // Max color value

    // Write pixel data row by row
    for (int lig = 0; lig < TAILLEGRILLE; ++lig) {
        for (int col = 0; col < TAILLEGRILLE; ++col) {
            Coord c(lig, col);

            if (g.caseVide(c)) {
                out << "255 255 255 ";  // White for empty cells
            } else {
                // Retrieve the animal occupying the cell
                Animal a = pop.get(g.getCase(c));

                if (a.getEspece() == Espece::Lapin)
                    out << "0 255 0 ";  // Green for Lapin
                else
                    out << "255 0 0 ";  // Red for Renard
            }
        }
        out << '\n'; // New line after each row
    }

    // Close the file
    out.close();
}
