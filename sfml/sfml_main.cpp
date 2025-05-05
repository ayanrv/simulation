#include <SFML/Graphics.hpp>
#include "JeuSFML.hpp"

using namespace sf;

int main() {
    Texture wood;
    if (!wood.loadFromFile("sfml/assets/woodbg.jpg")) {
        return 1;
    }

    Vector2u woodSize = wood.getSize();

    RenderWindow window(VideoMode(woodSize.x, woodSize.y), "Renards VS Lapins - Mayasha", Style::Default);
    window.setFramerateLimit(60);

    JeuSFML jeu(0.22, 0.07, 50);
    jeu.boucle(window);

    return 0;
}
