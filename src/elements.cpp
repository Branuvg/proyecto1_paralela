#include "elements.h"

void Bubble::update(float dt, float screenWidth, float screenHeight) {
    y -= 30.0f * dt;

    if (y < 0) {
        alive = false;
    }
}

void Bubble::draw(sf::RenderWindow& window) {
    sf::CircleShape circle(radius);
    circle.setPosition(x - radius, y - radius);
    circle.setFillColor(color);
    circle.setOutlineThickness(1.0f);
    circle.setOutlineColor(sf::Color::Blue);
    window.draw(circle);
}

void Starfish::update(float dt, float screenWidth, float screenHeight) {
    x += vx * dt;
    y += vy * dt;

    if (x < radius) x = radius;
    if (x > screenWidth - radius) x = screenWidth - radius;
    if (y < radius) y = radius;
    if (y > screenHeight - radius) y = screenHeight - radius;
}

void Starfish::draw(sf::RenderWindow& window) {
    sf::CircleShape star(radius, 5);
    star.setPosition(x - radius, y - radius);
    star.setFillColor(color);
    window.draw(star);
}

void Turtle::update(float dt, float screenWidth, float screenHeight) {
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

void Turtle::draw(sf::RenderWindow& window) {
    sf::CircleShape turtle(radius);
    turtle.setPosition(x - radius, y - radius);
    turtle.setFillColor(color);
    window.draw(turtle);
}

void Crab::update(float dt, float screenWidth, float screenHeight) {
    float waterLevel = screenHeight * 0.75f;

    x += vx * dt;
    y += vy * dt;

    if (x < radius) x = radius;
    if (x > screenWidth - radius) x = screenWidth - radius;
    if (y < waterLevel - radius) y = waterLevel - radius;
}

void Crab::draw(sf::RenderWindow& window) {
    sf::CircleShape crab(radius);
    crab.setPosition(x - radius, y - radius);
    crab.setFillColor(color);
    window.draw(crab);
}
