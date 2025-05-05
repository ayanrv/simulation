#include "Menu.hpp"

using namespace sf;

Menu::Menu() {
    loadAssets();
}

void Menu::loadAssets() {
    skyTex.loadFromFile("sfml/assets/sky.PNG");
    startTex.loadFromFile("sfml/assets/start.PNG");

    skySprite.setTexture(skyTex);
    startButton.setTexture(startTex);

    startButton.setScale(0.5f, 0.5f);
}

void Menu::render(sf::RenderWindow& window) {
    Vector2u winSize = window.getSize();

    skySprite.setScale(
        float(winSize.x) / skyTex.getSize().x,
        float(winSize.y) / skyTex.getSize().y
    );

    startButton.setPosition(
        winSize.x / 2.f - startButton.getGlobalBounds().width / 2.f,
        winSize.y / 2.f - startButton.getGlobalBounds().height / 2.f
    );

    window.draw(skySprite);
    window.draw(startButton);
}

void Menu::handleEvent(const Event& event, const RenderWindow& window) {
    if (event.type == Event::MouseButtonPressed) {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        if (startButton.getGlobalBounds().contains(mousePos)) {
            active = false;
        }
    }
}

bool Menu::isActive() const {
    return active;
}
