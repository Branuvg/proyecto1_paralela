#pragma once

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <optional>
#include <vector>

#include "elements.h"

class Simulation;

// Segmentos con que se aproxima cada figura. Al ser fijos por tipo, el bloque de
// vertices de cada elemento arranca siempre en un indice conocido y no se traslapa.
constexpr int BUBBLE_SEGMENTS = 10;
constexpr int STARFISH_POINTS = 5;
constexpr int TURTLE_SHELL_SEGMENTS = 10;
constexpr int TURTLE_HEAD_SEGMENTS = 6;

constexpr std::size_t BUBBLE_VERTEX_COUNT = BUBBLE_SEGMENTS * 3;
constexpr std::size_t STARFISH_VERTEX_COUNT = STARFISH_POINTS * 2 * 3;
constexpr std::size_t TURTLE_VERTEX_COUNT = (TURTLE_SHELL_SEGMENTS + TURTLE_HEAD_SEGMENTS) * 3;

class Renderer {
public:
    Renderer(float width, float height);

    // Busca una fuente para el HUD. Si no encuentra ninguna los FPS salen en el titulo.
    bool loadFont();
    bool hasFont() const { return hudText.has_value(); }

    void render(sf::RenderWindow& window, const Simulation& simulation, float fps);

private:
    void buildSky(const World& world);
    void buildWater(const World& world);
    void buildBubbles(const std::vector<Bubble>& bubbles);
    void buildStarfish(const std::vector<Starfish>& starfish);
    void buildTurtles(const std::vector<Turtle>& turtles);
    void drawHud(sf::RenderWindow& window, const Simulation& simulation, float fps);

    float screenWidth, screenHeight;

    sf::VertexArray skyVertices;
    sf::VertexArray waterVertices;
    sf::VertexArray bubbleVertices;
    sf::VertexArray starfishVertices;
    sf::VertexArray turtleVertices;

    sf::Font font;
    std::optional<sf::Text> hudText;
};
