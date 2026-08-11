#include "renderer.h"
#include "simulation.h"
#include "elements.h"
#include <ctime>
#include <cmath>

Renderer::Renderer(float width, float height)
    : screenWidth(width), screenHeight(height) {}

void Renderer::render(sf::RenderWindow& window, const Simulation& sim, float waterLevel) {
    drawBackground(window);
    drawSky(window, waterLevel);
    drawWater(window, waterLevel);
    drawWavePattern(window, waterLevel);
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
    sky.setPosition(sf::Vector2f(0.0f, 0.0f));
    sky.setFillColor(getSkyColor(hour));
    window.draw(sky);
}

void Renderer::drawWater(sf::RenderWindow& window, float waterLevel) {
    sf::RectangleShape water(sf::Vector2f(screenWidth, screenHeight - waterLevel));
    water.setPosition(sf::Vector2f(0.0f, waterLevel));
    water.setFillColor(sf::Color(30, 144, 200));
    window.draw(water);
}

void Renderer::drawWavePattern(sf::RenderWindow& window, float waterLevel) {
    static float waveOffset = 0.0f;
    waveOffset += 0.05f;

    sf::Color waveColor(100, 180, 230);
    const float waveAmplitude = 8.0f;
    const float waveFrequency = 0.03f;

    for (float x = 0; x < screenWidth; x += 40) {
        float waveY = waterLevel + std::sin(x * waveFrequency + waveOffset) * waveAmplitude;

        sf::CircleShape wave(5);
        wave.setPosition(sf::Vector2f(x, waveY - 5));
        wave.setFillColor(waveColor);
        wave.setOutlineThickness(0.5f);
        wave.setOutlineColor(sf::Color::Cyan);
        window.draw(wave);
    }
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
