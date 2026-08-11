#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

enum class ElementType {
    BUBBLE,
    STARFISH,
    TURTLE,
    CRAB
};

struct Element {
    ElementType type;
    float x, y;
    float vx, vy;
    float radius;
    sf::Color color;
    bool alive;

    Element(ElementType t, float px, float py, float r, sf::Color c)
        : type(t), x(px), y(py), vx(0), vy(0), radius(r), color(c), alive(true) {}

    virtual ~Element() = default;

    virtual void update(float dt, float screenWidth, float screenHeight) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};

struct Bubble : public Element {
    Bubble(float px, float py)
        : Element(ElementType::BUBBLE, px, py, 5.0f, sf::Color::White) {}

    void update(float dt, float screenWidth, float screenHeight) override;
    void draw(sf::RenderWindow& window) override;
};

struct Starfish : public Element {
    Starfish(float px, float py)
        : Element(ElementType::STARFISH, px, py, 8.0f, sf::Color::Yellow) {}

    void update(float dt, float screenWidth, float screenHeight) override;
    void draw(sf::RenderWindow& window) override;
};

struct Turtle : public Element {
    float angle;

    Turtle(float px, float py)
        : Element(ElementType::TURTLE, px, py, 10.0f, sf::Color::Green), angle(0) {}

    void update(float dt, float screenWidth, float screenHeight) override;
    void draw(sf::RenderWindow& window) override;
};

struct Crab : public Element {
    Crab(float px, float py)
        : Element(ElementType::CRAB, px, py, 12.0f, sf::Color::Red) {}

    void update(float dt, float screenWidth, float screenHeight) override;
    void draw(sf::RenderWindow& window) override;
};
