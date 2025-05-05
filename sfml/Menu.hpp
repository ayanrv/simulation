#ifndef MENU_HPP
#define MENU_HPP
#include <SFML/Graphics.hpp>

using namespace sf;

class Menu {
public:
    Menu();
    void loadAssets();
    void render(RenderWindow& window);
    void handleEvent(const Event& event, const RenderWindow& window);
    bool isActive() const;

private:
    Texture skyTex, startTex;
    Sprite skySprite, startButton;
    bool active = true;
};

#endif