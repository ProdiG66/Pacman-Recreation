#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>

class Fruit
{
public:
    Fruit(sf::Texture* texture)
    {
        fruitSpr.setTexture(*texture);
        setGridPos(11, 13);
        setLevel(1);
    }

    ~Fruit() {}

    sf::Sprite getSrpite()
    {
        return fruitSpr;
    }

    void setVisible(bool vis)
    {
        visible = vis;
    }

    void start()
    {
        if (!fruitThr.joinable())
        {
            fruitThr = std::thread(&Fruit::loop, this);
            std::cout << "Fruit thread start" << std::endl;
        }
    }

    void stop()
    {
        if (fruitThr.joinable())
        {
            fruitThr.detach();
            std::cout << "Fruit thread stop" << std::endl;
        }
    }

    void setLevel(int lvl)
    {
        switch (lvl)
        {
        case 3:
        case 4:
            level = 3;
            break;
        case 5:
        case 6:
            level = 4;
            break;
        case 7:
        case 8:
            level = 5;
            break;
        case 9:
        case 10:
            level = 6;
            break;
        case 11:
        case 12:
            level = 7;
            break;
        default:
            level = lvl;
            break;
        }

        if (lvl >= 13) { level = 8; }
        fruitSpr.setTextureRect(sf::IntRect(24 * (level - 1), ofsY, 24, 28));
    }

    bool getVisible() { return visible; }

    sf::Vector2f getPosition() { return fruitSpr.getPosition(); }

private:
    sf::Sprite fruitSpr;
    int ofsX = 0, ofsY = 6 * 30;
    int level = 0;
    int delay = 30;
    bool visible = false;
    int curTime;
    std::thread fruitThr;

    void loop()
    {
        curTime = time(0);
        curTime += delay;
        while (fruitThr.joinable())
        {
            if (curTime < time(0))
            {
                setVisible(!visible);
                curTime = time(0);
                curTime += delay;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        setVisible(false);
    }

    void setGridPos(int x, int y)
    {
        fruitSpr.setPosition(sf::Vector2f(x * 30, y * 30));
    }
};
