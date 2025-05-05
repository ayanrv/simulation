#include "JeuSFML.hpp"
#include "jeu.hpp"
#include "Menu.hpp"
#include "../coord.hpp"
#include <sstream>
#include <iomanip>
#include <cstdlib>

const int CELL_SIZE = 30;
const int MARGIN = 64;

using namespace sf;
using namespace std;

JeuSFML::JeuSFML(double probLapin, double probRenard, int nbTours)
    : probLapin(probLapin), probRenard(probRenard), core(probLapin, probRenard), nbTours(nbTours), tourActuel(0) {
    chargerTextures();
    chargerFont("sfml/assets/VT323-Regular.ttf");

    fieldWidth = TAILLEGRILLE * CELL_SIZE;
    fieldHeight = TAILLEGRILLE * CELL_SIZE;

    baseWindowWidth = textures["wood"].getSize().x;
    baseWindowHeight = textures["wood"].getSize().y;

    stats.setFont(font);
    stats.setCharacterSize(36);
    stats.setFillColor(Color::White);
}

void JeuSFML::chargerTextures() {
    textures["wood"].loadFromFile("sfml/assets/woodbg.jpg");
    textures["background"].loadFromFile("sfml/assets/background.png");
    textures["fraise"].loadFromFile("sfml/assets/fraise.PNG");
    textures["phone"].loadFromFile("sfml/assets/phone.PNG");
    textures["logo"].loadFromFile("sfml/assets/pink_logo.PNG");

    textures["lapin_male1"].loadFromFile("sfml/assets/lapin_male1.PNG");
    textures["lapin_male2"].loadFromFile("sfml/assets/lapin_male2.PNG");
    textures["lapin_fem1"].loadFromFile("sfml/assets/lapin_fem1.PNG");
    textures["lapin_fem2"].loadFromFile("sfml/assets/lapin_fem2.PNG");

    textures["renard_male1"].loadFromFile("sfml/assets/renard_male1.PNG");
    textures["renard_male2"].loadFromFile("sfml/assets/renard_male2.PNG");
    textures["renard_fem1"].loadFromFile("sfml/assets/renard_fem1.PNG");
    textures["renard_fem2"].loadFromFile("sfml/assets/renard_fem2.PNG");
}

void JeuSFML::chargerFont(const string& path) {
    if (!font.loadFromFile(path)) {
        throw runtime_error("Font loading failed: " + path);
    }
}

void JeuSFML::renderBackground(RenderWindow& window) {
    Sprite wood(textures["wood"]);
    Vector2u winSize = window.getSize();
    wood.setScale(
        float(winSize.x) / wood.getTexture()->getSize().x,
        float(winSize.y) / wood.getTexture()->getSize().y
    );
    window.draw(wood);

    Sprite bg(textures["background"]);
    bg.setScale(
        float(fieldWidth) / bg.getTexture()->getSize().x,
        float(fieldHeight) / bg.getTexture()->getSize().y
    );
    bg.setPosition(MARGIN, MARGIN);
    window.draw(bg);

    RectangleShape border(Vector2f(fieldWidth + 4, fieldHeight + 4));
    border.setPosition(MARGIN - 2, MARGIN - 2);
    border.setFillColor(Color::Transparent);
    border.setOutlineThickness(4);
    border.setOutlineColor(Color(255, 192, 203));  // light pink
    window.draw(border);
}

void JeuSFML::renderLogo(RenderWindow& window) {
    Sprite logo(textures["logo"]);

    float shrink = 360.f;
    float scale = (logo.getTexture()->getSize().x - shrink) / logo.getTexture()->getSize().x;
    logo.setScale(scale, scale);

    float logoWidth = logo.getGlobalBounds().width;

    float rightBlockWidth = window.getSize().x - (MARGIN + fieldWidth + MARGIN);
    float rightBlockX = MARGIN + fieldWidth;

    float logoX = rightBlockX + (rightBlockWidth - logoWidth) / 2.f + 50.f;

    float logoY = MARGIN - 135.f;

    logo.setPosition(logoX, logoY);
    window.draw(logo);
}



void JeuSFML::renderFraises(RenderWindow& window) {
    for (const auto& f : core.getFraises()) {
        Sprite s(textures["fraise"]);
        s.setScale(
            1.2f * CELL_SIZE / s.getTexture()->getSize().x,
            1.2f * CELL_SIZE / s.getTexture()->getSize().y
        );
        s.setPosition(
            MARGIN + f.pos.getCol() * CELL_SIZE,
            MARGIN + f.pos.getLig() * CELL_SIZE
        );
        window.draw(s);
    }
}

void JeuSFML::renderAnimaux(RenderWindow& window) {
    auto ids = core.getPopulation().getIds();
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.get(i);
        if (!core.getPopulation().existe(id)) continue;
        const Animal& a = core.getPopulation().get(id);
        Coord pos = a.getCoord();

        int variant = id % 2 + 1;
        string key = (a.getEspece() == Espece::Lapin)
            ? (a.getSexe() == Sexe::Male ? "lapin_male" : "lapin_fem") + to_string(variant)
            : (a.getSexe() == Sexe::Male ? "renard_male" : "renard_fem") + to_string(variant);

        Sprite s(textures[key]);
        s.setScale(
            1.25f * CELL_SIZE / s.getTexture()->getSize().x,
            1.25f * CELL_SIZE / s.getTexture()->getSize().y
        );
        s.setPosition(
            MARGIN + pos.getCol() * CELL_SIZE,
            MARGIN + pos.getLig() * CELL_SIZE
        );
        window.draw(s);
    }
}

void JeuSFML::renderStatistiques(RenderWindow& window) {
    Sprite phone(textures["phone"]);
    float phoneHeight = fieldHeight - 220;
    float phoneScale = phoneHeight / phone.getTexture()->getSize().y;
    phone.setScale(phoneScale, phoneScale);

    float phoneX = window.getSize().x - phone.getGlobalBounds().width - MARGIN - 60.f;
    float phoneY = MARGIN + textures["logo"].getSize().y * 0.3f + 30;
    phone.setPosition(phoneX, phoneY);
    window.draw(phone);

    int countL = 0, countR = 0, faimL = 0, faimR = 0;
    auto ids = core.getPopulation().getIds();
    for (int i = 0; i < ids.cardinal(); ++i) {
        int id = ids.get(i);
        if (!core.getPopulation().existe(id)) continue;
        const Animal& a = core.getPopulation().get(id);
        if (a.getEspece() == Espece::Lapin) {
            ++countL;
            faimL += a.getFaim();
        } else {
            ++countR;
            faimR += a.getFaim();
        }
    }

    int fraises = core.getFraises().size();
    int total = TAILLEGRILLE * TAILLEGRILLE;
    int empty = total - (countL + countR + fraises);

    stringstream ss;
    ss << "Tour " << tourActuel << "/" << nbTours << "\n\n";
    ss << "Lapins     : " << countL << "\n";
    ss << "Renards    : " << countR << "\n";
    ss << "Fraises    : " << fraises << "\n";
    ss << "Vides      : " << empty << "\n";
    if (countL > 0) ss << "Faim Lapins  : " << fixed << setprecision(1) << float(faimL) / countL << "\n";
    if (countR > 0) ss << "Faim Renards : " << fixed << setprecision(1) << float(faimR) / countR << "\n";
    if (tourActuel >= nbTours) {
        ss << "\nAppuyez sur R pour\nredemarrer";
    }
    

    stats.setString(ss.str());

    FloatRect bounds = stats.getLocalBounds();
    float xCenter = phoneX + phone.getGlobalBounds().width / 2.f - bounds.width / 2.f - 30.f;
    float yCenter = phoneY + phone.getGlobalBounds().height / 2.f - bounds.height / 2.f - 40.f;
    stats.setPosition(xCenter, yCenter);
    window.draw(stats);
}

void JeuSFML::boucle(RenderWindow& window) {
    window.setFramerateLimit(60);
    Clock clock;

    const double probReproLapin = 0.45;
    const int minFreeLapin = 2;
    const int foodInit = 7;
    const int foodReprod = 4;
    const int foodGain = 6;
    const int maxFaim = 12;
    const double probReproRenard = 0.3;
    const int nbFraisesMax = 150;

    View view(FloatRect(0, 0, baseWindowWidth, baseWindowHeight));
    window.setView(view);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::Resized) {
                view.setSize(event.size.width, event.size.height);
                view.setCenter(event.size.width / 2.f, event.size.height / 2.f);
                window.setView(view);
            }
    
            if (menu.isActive()) {
                menu.handleEvent(event, window);
            } else if (event.type == Event::KeyPressed && event.key.code == Keyboard::R) {
                core = Jeu(probLapin, probRenard);
                tourActuel = 0;
                clock.restart();
            }
        }
    
        window.clear();
    
        if (menu.isActive()) {
            menu.render(window);
        } else {
            if (tourActuel < nbTours && clock.getElapsedTime().asSeconds() > 0.5f) {
                core.tour(probReproLapin, minFreeLapin, foodInit, foodReprod,
                          foodGain, maxFaim, probReproRenard, nbFraisesMax);
                clock.restart();
                ++tourActuel;
            }
    
            renderBackground(window);
            renderLogo(window);
            renderFraises(window);
            renderAnimaux(window);
            renderStatistiques(window);
        }
    
        window.display();
    }    
}
