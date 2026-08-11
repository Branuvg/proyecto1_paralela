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
        circle.setFillColor(sf::Color(200, 220, 255, 180));
        circle.setOutlineThickness(2.0f);
        circle.setOutlineColor(sf::Color::Cyan);
        window.draw(circle);

        sf::CircleShape shine(radius / 3);
        shine.setPosition(sf::Vector2f(x - radius/3, y - radius/2));
        shine.setFillColor(sf::Color::White);
        window.draw(shine);
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
        star.setFillColor(sf::Color(255, 220, 80));
        star.setOutlineThickness(1.5f);
        star.setOutlineColor(sf::Color(255, 200, 0));
        window.draw(star);

        sf::CircleShape center(radius / 2);
        center.setPosition(sf::Vector2f(x - radius/2, y - radius/2));
        center.setFillColor(sf::Color(255, 150, 0));
        window.draw(center);
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
        sf::CircleShape shell(radius);
        shell.setPosition(sf::Vector2f(x - radius, y - radius));
        shell.setFillColor(sf::Color(50, 150, 50));
        shell.setOutlineThickness(2.0f);
        shell.setOutlineColor(sf::Color(30, 100, 30));
        window.draw(shell);

        sf::CircleShape head(radius / 3);
        float headX = x + radius * std::cos(angle);
        float headY = y + radius * std::sin(angle);
        head.setPosition(sf::Vector2f(headX - radius/3, headY - radius/3));
        head.setFillColor(sf::Color(100, 180, 100));
        window.draw(head);
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
        sf::CircleShape body(radius);
        body.setPosition(sf::Vector2f(x - radius, y - radius));
        body.setFillColor(sf::Color(220, 80, 30));
        body.setOutlineThickness(2.0f);
        body.setOutlineColor(sf::Color(180, 50, 0));
        window.draw(body);

        sf::CircleShape claw1(radius / 2);
        claw1.setPosition(sf::Vector2f(x - radius, y - radius));
        claw1.setFillColor(sf::Color(200, 60, 20));
        window.draw(claw1);

        sf::CircleShape claw2(radius / 2);
        claw2.setPosition(sf::Vector2f(x + radius / 2, y - radius));
        claw2.setFillColor(sf::Color(200, 60, 20));
        window.draw(claw2);
    }
};
