#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

class GameText
{
public:
    sf::Font fnt;
    sf::Text authorTxt;
    sf::Text enterTxt;
    sf::Text gameOverTxt;
    sf::Text scoreTxt;
    sf::Text bonusTxt;
    sf::Text levelTxt;
    sf::Text countTxt;
    sf::Text targetPointTxt;
    sf::RenderWindow* window;
    std::thread bonusThr;
    int* totalTime;
    int curTime;
    int delay = 2;

    GameText()
    {
        fnt.loadFromFile("font/comic.ttf");
        setText();
    }

    ~GameText()
    {
        if (bonusThr.joinable()) { bonusThr.detach(); }
    }

    void showBonus(int bonus, sf::Vector2f pos, bool fruit)
    {
        std::cout << "Bonus ***** " << bonus << std::endl;
        if (fruit) { bonusTxt.setFillColor(sf::Color::Green); }
        else
        {
            bonusTxt.setFillColor(sf::Color::Blue);
        }
        bonusTxt.setString(std::to_string(bonus));
        bonusTxt.setPosition(pos);
        curTime = time(0);
        curTime += delay;
        totalTime = &curTime;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!bonusThr.joinable())
        {
            std::cout << "TextThread created " << std::endl;
            bonusThr = std::thread(&GameText::show, this);
        }
    }

    void stopThread()
    {
        if (bonusThr.joinable()) { bonusThr.detach(); }
    }

    sf::Text getGhostNameTxt(std::string name)
    {
        sf::Text* txt = new sf::Text();
        txt->setFont(fnt);
        txt->setString(name);
        txt->setFillColor(sf::Color::White);
        txt->setCharacterSize(25);
        return *txt;
    }

private:

    void show()
    {
        while (true)
        {
            int x = time(0);
            if (*totalTime < x)
            {
                bonusTxt.setString("");
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void setText()
    {
        authorTxt.setFont(fnt);
        authorTxt.setString("Pacman Recreation\nDeveloped By Jan Enri Arquero");
        authorTxt.setCharacterSize(30);
        authorTxt.setPosition(sf::Vector2f(150, 400));

        enterTxt.setFont(fnt);
        enterTxt.setString("PRESS ENTER");
        enterTxt.setCharacterSize(64);
        enterTxt.setPosition(sf::Vector2f(150, 500));

        scoreTxt.setFont(fnt);
        scoreTxt.setString("SCORE:");
        scoreTxt.setCharacterSize(22);
        scoreTxt.setPosition(getGridPosXY(1, 0));

        levelTxt.setFont(fnt);
        levelTxt.setString("LEVEL:");
        levelTxt.setCharacterSize(18);
        levelTxt.setPosition(getGridPosXY(19, 24));

        countTxt.setFont(fnt);
        countTxt.setFillColor(sf::Color::Red);
        countTxt.setString("");
        countTxt.setCharacterSize(35);
        countTxt.setOutlineColor(sf::Color::White);
        countTxt.setOutlineThickness(2);
        countTxt.setPosition(getGridPosXY(11, 14));

        bonusTxt.setFont(fnt);
        bonusTxt.setString("");
        bonusTxt.setCharacterSize(26);
        bonusTxt.setFillColor(sf::Color::Blue);
        bonusTxt.setOutlineColor(sf::Color::White);
        bonusTxt.setOutlineThickness(2);

        gameOverTxt.setFont(fnt);
        gameOverTxt.setString("");
        gameOverTxt.setCharacterSize(35);
        gameOverTxt.setFillColor(sf::Color::Red);
        gameOverTxt.setOutlineColor(sf::Color::White);
        gameOverTxt.setOutlineThickness(2);
        gameOverTxt.setPosition(getGridPosXY(8, 14));

        targetPointTxt.setFont(fnt);
        targetPointTxt.setString("*");
        targetPointTxt.setCharacterSize(25);
        targetPointTxt.setFillColor(sf::Color::Green);
        targetPointTxt.setOutlineThickness(1);
    }

    sf::Vector2f getGridPosXY(float x, float y)
    {
        return sf::Vector2f(x * 30, y * 30);
    }
};
