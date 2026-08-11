#include "renderer.h"
#include "simulation.h"
#include "elements.h"
#include <ctime>

Renderer::Renderer(float width, float height)
    : screenWidth(width), screenHeight(height) {}

void Renderer::render(sf::RenderWindow& window, const Simulation& sim, float waterLevel) {
    drawBackground(window);
    drawSky(window, waterLevel);
    drawWater(window, waterLevel);
    drawElements(window, sim.getElements());
}

void Renderer::drawBackground(sf::RenderWindow& window) {
    sf::RectangleShape bg(sf::Vector2f(screenWidth, screenHeight));
    bg.setFillColor(sf::Color::White);
    window.draw(bg);
}

void Renderer::drawSky(sf::RenderWindow& window, float waterLevel) {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    int hour = timeinfo->tm_hour;

    sf::RectangleShape sky(sf::Vector2f(screenWidth, waterLevel));
    sky.setPosition(0, 0);
    sky.setFillColor(getSkyColor(hour));
    window.draw(sky);
}

void Renderer::drawWater(sf::RenderWindow& window, float waterLevel) {
    sf::RectangleShape water(sf::Vector2f(screenWidth, screenHeight - waterLevel));
    water.setPosition(0, waterLevel);
    water.setFillColor(sf::Color::Blue);
    window.draw(water);
}

void Renderer::drawElements(sf::RenderWindow& window, const std::vector<std::shared_ptr<Element>>& elements) {
    for (const auto& element : elements) {
        element->draw(window);
    }
}

sf::Color Renderer::getSkyColor(int hour) {
    if (hour >= 6 && hour < 11) {
        return sf::Color(135, 206, 235);
    } else if (hour >= 11 && hour < 19) {
        return sf::Color(255, 150, 100);
    } else {
        return sf::Color(30, 30, 60);
    }
}
