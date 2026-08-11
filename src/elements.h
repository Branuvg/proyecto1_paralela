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

class Element {
public:
    ElementType type;
    float x, y;
    float vx, vy;
    float radius;
    sf::Color color;
    bool alive;

    Element(ElementType t, float px, float py, float r, sf::Color c)
        : type(t), x(px), y(py), vx(0), vy(0), radius(r), color(c), alive(true) {}

    virtual ~Element() {}

    virtual void update(float dt, float screenWidth, float screenHeight) {}
    virtual void draw(sf::RenderWindow& window) {}
};

class Bubble : public Element {
public:
    Bubble(float px, float py)
        : Element(ElementType::BUBBLE, px, py, 5.0f, sf::Color::White) {}

    void update(float dt, float screenWidth, float screenHeight) override {
        y -= 30.0f * dt;
        if (y < 0) {
            alive = false;
        }
    }

    void draw(sf::RenderWindow& window) override {
        sf::CircleShape circle(radius);
        circle.setPosition(sf::Vector2f(x - radius, y - radius));
        circle.setFillColor(color);
        circle.setOutlineThickness(1.0f);
        circle.setOutlineColor(sf::Color::Blue);
        window.draw(circle);
    }
};

class Starfish : public Element {
public:
    Starfish(float px, float py)
        : Element(ElementType::STARFISH, px, py, 8.0f, sf::Color::Yellow) {}

    void update(float dt, float screenWidth, float screenHeight) override {
        x += vx * dt;
        y += vy * dt;

        if (x < radius) x = radius;
        if (x > screenWidth - radius) x = screenWidth - radius;
        if (y < radius) y = radius;
        if (y > screenHeight - radius) y = screenHeight - radius;
    }

    void draw(sf::RenderWindow& window) override {
        sf::CircleShape star(radius, 5);
        star.setPosition(sf::Vector2f(x - radius, y - radius));
        star.setFillColor(color);
        window.draw(star);
    }
};

class Turtle : public Element {
public:
    float angle;

    Turtle(float px, float py)
        : Element(ElementType::TURTLE, px, py, 10.0f, sf::Color::Green), angle(0) {}

    void update(float dt, float screenWidth, float screenHeight) override {
        const float SPEED = 50.0f;

        vx = SPEED * std::cos(angle);
        vy = SPEED * std::sin(angle);

        x += vx * dt;
        y += vy * dt;

        float waterLevel = screenHeight * 0.75f;

        if (x < radius || x > screenWidth - radius) {
            angle += M_PI / 3.0f;
        }
        if (y < radius || y > waterLevel - radius) {
            angle += M_PI / 3.0f;
        }
    }

    void draw(sf::RenderWindow& window) override {
        sf::CircleShape turtle(radius);
        turtle.setPosition(sf::Vector2f(x - radius, y - radius));
        turtle.setFillColor(color);
        window.draw(turtle);
    }
};

class Crab : public Element {
public:
    Crab(float px, float py)
        : Element(ElementType::CRAB, px, py, 12.0f, sf::Color::Red) {}

    void update(float dt, float screenWidth, float screenHeight) override {
        float waterLevel = screenHeight * 0.75f;

        x += vx * dt;
        y += vy * dt;

        if (x < radius) x = radius;
        if (x > screenWidth - radius) x = screenWidth - radius;
        if (y < waterLevel - radius) y = waterLevel - radius;
    }

    void draw(sf::RenderWindow& window) override {
        sf::CircleShape crab(radius);
        crab.setPosition(sf::Vector2f(x - radius, y - radius));
        crab.setFillColor(color);
        window.draw(crab);
    }
};
