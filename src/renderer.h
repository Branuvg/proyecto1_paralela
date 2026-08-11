#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Simulation;
class Element;

class Renderer {
public:
    Renderer(float width, float height);

    void render(sf::RenderWindow& window, const Simulation& sim, float waterLevel);

private:
    float screenWidth, screenHeight;

    void drawBackground(sf::RenderWindow& window);
    void drawWater(sf::RenderWindow& window, float waterLevel);
    void drawSky(sf::RenderWindow& window, float waterLevel);
    void drawElements(sf::RenderWindow& window, const std::vector<std::shared_ptr<Element>>& elements);

    sf::Color getSkyColor(int hour);
};
