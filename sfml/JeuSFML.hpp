#ifndef JEUSFML_HPP
#define JEUSFML_HPP

#include "../jeu.hpp"
#include "Menu.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

using namespace sf;
using namespace std;

class JeuSFML {
    public:
        // Constructeur : initialise le jeu avec proba init et nb de tours
        JeuSFML(double probLapin, double probRenard, int nbTours);
    
        // Lance la boucle principale
        void boucle(RenderWindow& window);

        double probLapin;
        double probRenard;

    private:
        // Chargement des assets
        void chargerTextures();
        void chargerFont(const string& path);
    
        // Rendu visuel
        void renderBackground(RenderWindow& window);
        void renderFraises(RenderWindow& window);
        void renderAnimaux(RenderWindow& window);
        void renderStatistiques(RenderWindow& window);
        void renderLogo(RenderWindow& window);
    
        // Composants internes
        Jeu core;
        map<string, Texture> textures;
        Font font;
        Text stats;
    
        // Paramètres de simulation
        int nbTours;
        int tourActuel;

        unsigned int baseWindowWidth;
        unsigned int baseWindowHeight;
        int fieldWidth, fieldHeight;

        Menu menu;
    };
    

#endif
