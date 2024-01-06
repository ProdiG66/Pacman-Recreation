#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "starter.h"

int main() {
    constexpr int winWidth = 23 * 30;
    constexpr int winHeight = 25 * 30;

    sf::Texture sprTexture;
    if (!sprTexture.loadFromFile("texture/PMSprites.png")) {
        std::cerr << "Error loading texture" << std::endl;
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), "PacMan-Recreation", sf::Style::Default);
    window.setActive(false);

    std::unique_ptr<Starter> starter = std::make_unique<Starter>(&window, &sprTexture);

    return 0;
}