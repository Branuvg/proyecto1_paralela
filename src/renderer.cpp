#include "renderer.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <sstream>

#include "simulation.h"

namespace {

constexpr int WATER_COLUMNS = 96;         // resolucion horizontal del oleaje
constexpr float STARFISH_INNER_RATIO = 0.42f;

struct SkyPalette {
    sf::Color top;
    sf::Color horizon;
};

// Amanecer, dia, atardecer y noche: el cielo interpola entre estas cuatro paletas.
const SkyPalette SKY_PALETTES[4] = {
    {sf::Color(255, 150, 120), sf::Color(255, 205, 165)},
    {sf::Color(95, 165, 235), sf::Color(180, 220, 245)},
    {sf::Color(235, 95, 60), sf::Color(255, 170, 90)},
    {sf::Color(15, 20, 55), sf::Color(45, 55, 105)}
};

sf::Color lerpColor(const sf::Color& from, const sf::Color& to, float t) {
    auto mix = [t](std::uint8_t a, std::uint8_t b) {
        return static_cast<std::uint8_t>(a + (b - a) * t);
    };
    return sf::Color(mix(from.r, to.r), mix(from.g, to.g), mix(from.b, to.b),
                     mix(from.a, to.a));
}

// Escoge la paleta segun la hora del dia y mezcla con la siguiente.
SkyPalette skyPaletteAt(float timeOfDay) {
    float scaled = timeOfDay * 4.0f;
    int index = static_cast<int>(scaled) % 4;
    int next = (index + 1) % 4;
    float t = scaled - std::floor(scaled);

    return {lerpColor(SKY_PALETTES[index].top, SKY_PALETTES[next].top, t),
            lerpColor(SKY_PALETTES[index].horizon, SKY_PALETTES[next].horizon, t)};
}

void setVertex(sf::VertexArray& target, std::size_t index, float x, float y,
               const sf::Color& color) {
    target[index].position = sf::Vector2f(x, y);
    target[index].color = color;
}

// Escribe un circulo como abanico de triangulos a partir de firstVertex.
void writeCircle(sf::VertexArray& target, std::size_t firstVertex, float cx, float cy,
                 float radius, int segments, const sf::Color& color) {
    float step = 2.0f * PI / static_cast<float>(segments);

    for (int s = 0; s < segments; s++) {
        float a0 = step * static_cast<float>(s);
        float a1 = step * static_cast<float>(s + 1);
        std::size_t base = firstVertex + static_cast<std::size_t>(s) * 3;

        setVertex(target, base, cx, cy, color);
        setVertex(target, base + 1, cx + radius * std::cos(a0), cy + radius * std::sin(a0), color);
        setVertex(target, base + 2, cx + radius * std::cos(a1), cy + radius * std::sin(a1), color);
    }
}

// Escribe una estrella de 'points' puntas rotada un angulo, tambien como abanico.
void writeStar(sf::VertexArray& target, std::size_t firstVertex, float cx, float cy,
               float radius, float angle, int points, const sf::Color& color) {
    int corners = points * 2;
    float step = 2.0f * PI / static_cast<float>(corners);
    float innerRadius = radius * STARFISH_INNER_RATIO;

    for (int s = 0; s < corners; s++) {
        float a0 = angle + step * static_cast<float>(s);
        float a1 = angle + step * static_cast<float>(s + 1);
        float r0 = (s % 2 == 0) ? radius : innerRadius;
        float r1 = (s % 2 == 0) ? innerRadius : radius;
        std::size_t base = firstVertex + static_cast<std::size_t>(s) * 3;

        setVertex(target, base, cx, cy, color);
        setVertex(target, base + 1, cx + r0 * std::cos(a0), cy + r0 * std::sin(a0), color);
        setVertex(target, base + 2, cx + r1 * std::cos(a1), cy + r1 * std::sin(a1), color);
    }
}

}  // namespace

Renderer::Renderer(float width, float height)
    : screenWidth(width), screenHeight(height) {
    skyVertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
    skyVertices.resize(4);

    waterVertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
    waterVertices.resize((WATER_COLUMNS + 1) * 2);

    bubbleVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    starfishVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    turtleVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
}

bool Renderer::loadFont() {
    const char* candidates[] = {
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/Library/Fonts/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"
    };

    for (const char* path : candidates) {
        // Se verifica antes de abrir para que SFML no imprima un error por cada
        // candidato que no exista en esta plataforma.
        std::error_code error;
        if (!std::filesystem::exists(path, error)) {
            continue;
        }

        if (font.openFromFile(path)) {
            hudText.emplace(font, "", 16);
            hudText->setFillColor(sf::Color::White);
            hudText->setOutlineColor(sf::Color(0, 0, 0, 180));
            hudText->setOutlineThickness(2.0f);
            hudText->setPosition(sf::Vector2f(10.0f, 8.0f));
            return true;
        }
    }
    return false;
}

void Renderer::render(sf::RenderWindow& window, const Simulation& simulation, float fps) {
    const World& world = simulation.getWorld();

    buildSky(world);
    buildWater(world);
    buildBubbles(simulation.getBubbles());
    buildStarfish(simulation.getStarfish());
    buildTurtles(simulation.getTurtles());

    window.draw(skyVertices);
    window.draw(waterVertices);
    window.draw(turtleVertices);
    window.draw(starfishVertices);
    window.draw(bubbleVertices);

    drawHud(window, simulation, fps);
}

void Renderer::buildSky(const World& world) {
    SkyPalette palette = skyPaletteAt(world.timeOfDay);

    setVertex(skyVertices, 0, 0.0f, 0.0f, palette.top);
    setVertex(skyVertices, 1, screenWidth, 0.0f, palette.top);
    setVertex(skyVertices, 2, 0.0f, screenHeight, palette.horizon);
    setVertex(skyVertices, 3, screenWidth, screenHeight, palette.horizon);
}

void Renderer::buildWater(const World& world) {
    // El borde superior sigue la senoidal; el inferior queda pegado al fondo.
    const sf::Color surfaceColor(120, 200, 240, 235);
    const sf::Color deepColor(10, 70, 130, 255);
    float columnWidth = screenWidth / static_cast<float>(WATER_COLUMNS);

    for (int c = 0; c <= WATER_COLUMNS; c++) {
        float x = columnWidth * static_cast<float>(c);
        std::size_t base = static_cast<std::size_t>(c) * 2;

        setVertex(waterVertices, base, x, waterSurfaceAt(world, x), surfaceColor);
        setVertex(waterVertices, base + 1, x, screenHeight, deepColor);
    }
}

void Renderer::buildBubbles(const std::vector<Bubble>& bubbles) {
    bubbleVertices.resize(bubbles.size() * BUBBLE_VERTEX_COUNT);

    int count = static_cast<int>(bubbles.size());
    for (int i = 0; i < count; i++) {
        const Bubble& bubble = bubbles[i];
        writeCircle(bubbleVertices, static_cast<std::size_t>(i) * BUBBLE_VERTEX_COUNT,
                    bubble.x, bubble.y, bubble.radius, BUBBLE_SEGMENTS, bubble.color);
    }
}

void Renderer::buildStarfish(const std::vector<Starfish>& starfish) {
    starfishVertices.resize(starfish.size() * STARFISH_VERTEX_COUNT);

    int count = static_cast<int>(starfish.size());
    for (int i = 0; i < count; i++) {
        const Starfish& star = starfish[i];
        writeStar(starfishVertices, static_cast<std::size_t>(i) * STARFISH_VERTEX_COUNT,
                  star.x, star.y, star.radius, star.angle, STARFISH_POINTS, star.color);
    }
}

void Renderer::buildTurtles(const std::vector<Turtle>& turtles) {
    turtleVertices.resize(turtles.size() * TURTLE_VERTEX_COUNT);

    int count = static_cast<int>(turtles.size());
    for (int i = 0; i < count; i++) {
        const Turtle& turtle = turtles[i];
        std::size_t base = static_cast<std::size_t>(i) * TURTLE_VERTEX_COUNT;

        writeCircle(turtleVertices, base, turtle.x, turtle.y, turtle.radius,
                    TURTLE_SHELL_SEGMENTS, turtle.color);

        // La cabeza va adelante, en la direccion en que se mueve la tortuga.
        float speed = std::sqrt(turtle.vx * turtle.vx + turtle.vy * turtle.vy);
        float dirX = speed > 0.0f ? turtle.vx / speed : 1.0f;
        float dirY = speed > 0.0f ? turtle.vy / speed : 0.0f;
        float headRadius = turtle.radius * 0.45f;
        sf::Color headColor(static_cast<std::uint8_t>(std::min(255, turtle.color.r + 45)),
                            static_cast<std::uint8_t>(std::min(255, turtle.color.g + 45)),
                            static_cast<std::uint8_t>(std::min(255, turtle.color.b + 45)));

        writeCircle(turtleVertices, base + TURTLE_SHELL_SEGMENTS * 3,
                    turtle.x + dirX * turtle.radius, turtle.y + dirY * turtle.radius,
                    headRadius, TURTLE_HEAD_SEGMENTS, headColor);
    }
}

void Renderer::drawHud(sf::RenderWindow& window, const Simulation& simulation, float fps) {
    if (!hudText.has_value()) {
        return;
    }

    std::size_t total = simulation.getBubbles().size() + simulation.getStarfish().size() +
                        simulation.getTurtles().size();

    std::ostringstream line;
    line.precision(1);
    line << std::fixed << "FPS: " << fps << "   elementos: " << total;

    hudText->setString(line.str());
    window.draw(*hudText);
}
