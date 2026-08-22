#include "simulation.h"

#include <algorithm>
#include <cmath>
#include <random>

namespace {

// Reparto de los N elementos entre los tres tipos.
constexpr float BUBBLE_SHARE = 0.55f;
constexpr float STARFISH_SHARE = 0.25f;

constexpr float DAY_SECONDS = 45.0f;    // cuanto dura un ciclo completo de cielo
constexpr float WAVE_SPEED = 1.6f;
constexpr float WATER_RISE_SECONDS = 20.0f;

constexpr float START_WATER_FRACTION = 0.60f;   // el agua empieza baja
constexpr float MAX_WATER_FRACTION = 0.26f;     // y sube hasta aca

}  // namespace

Simulation::Simulation(float width, float height) {
    world.width = width;
    world.height = height;
    world.waterLevel = height * START_WATER_FRACTION;
    world.waveAmplitude = std::max(4.0f, height * 0.012f);
    world.waveFrequency = 2.0f * PI / std::max(120.0f, width * 0.28f);

    maxWaterLevel = height * MAX_WATER_FRACTION;
    waterRiseSpeed = (world.waterLevel - maxWaterLevel) / WATER_RISE_SECONDS;
}

void Simulation::populate(int elementCount, unsigned seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> unit(0.0f, 1.0f);

    // Todo nace dentro del agua actual; lo que el agua descubra al subir queda libre.
    float spawnTop = world.waterLevel;
    float spawnDepth = world.height - spawnTop;

    int bubbleCount = static_cast<int>(elementCount * BUBBLE_SHARE);
    int starfishCount = static_cast<int>(elementCount * STARFISH_SHARE);
    int turtleCount = elementCount - bubbleCount - starfishCount;

    bubbles.clear();
    starfish.clear();
    turtles.clear();
    bubbles.reserve(bubbleCount);
    starfish.reserve(starfishCount);
    turtles.reserve(turtleCount);

    // Las burbujas nacen repartidas en la columna de agua para que no suban en bloque.
    for (int i = 0; i < bubbleCount; i++) {
        Bubble bubble;
        bubble.x = unit(rng) * world.width;
        bubble.y = spawnTop + unit(rng) * spawnDepth;
        bubble.riseSpeed = 25.0f + unit(rng) * 55.0f;
        bubble.baseRadius = 3.0f + unit(rng) * 5.0f;
        bubble.radius = bubble.baseRadius;
        bubble.color = sf::Color(static_cast<std::uint8_t>(170 + unit(rng) * 60),
                                 static_cast<std::uint8_t>(200 + unit(rng) * 45),
                                 255,
                                 static_cast<std::uint8_t>(140 + unit(rng) * 80));
        bubbles.push_back(bubble);
    }

    for (int i = 0; i < starfishCount; i++) {
        Starfish star;
        star.x = unit(rng) * world.width;
        star.y = spawnTop + unit(rng) * spawnDepth;
        star.angle = unit(rng) * 2.0f * PI;
        star.spin = (unit(rng) < 0.5f ? -1.0f : 1.0f) * (0.4f + unit(rng) * 1.6f);
        star.baseRadius = 7.0f + unit(rng) * 7.0f;
        star.radius = star.baseRadius;
        star.color = sf::Color(static_cast<std::uint8_t>(200 + unit(rng) * 55),
                               static_cast<std::uint8_t>(60 + unit(rng) * 130),
                               static_cast<std::uint8_t>(60 + unit(rng) * 90));
        starfish.push_back(star);
    }

    for (int i = 0; i < turtleCount; i++) {
        Turtle turtle;
        turtle.x = unit(rng) * world.width;
        turtle.y = spawnTop + unit(rng) * spawnDepth;

        // Direccion diagonal aleatoria proyectada sobre un angulo cualquiera.
        float angle = unit(rng) * 2.0f * PI;
        float speed = 35.0f + unit(rng) * 60.0f;
        turtle.vx = speed * std::cos(angle);
        turtle.vy = speed * std::sin(angle);

        turtle.radius = 8.0f + unit(rng) * 6.0f;
        turtle.color = sf::Color(static_cast<std::uint8_t>(30 + unit(rng) * 70),
                                 static_cast<std::uint8_t>(120 + unit(rng) * 100),
                                 static_cast<std::uint8_t>(40 + unit(rng) * 70));
        turtles.push_back(turtle);
    }
}

void Simulation::update(float dt) {
    // Primero las interacciones, que solo leen posiciones, y despues el movimiento.
    advanceWorld(dt);
    applyMagnifier();
    resolveTurtleCollisions();
    updateElements(dt);
}

void Simulation::advanceWorld(float dt) {
    world.timeOfDay += dt / DAY_SECONDS;
    if (world.timeOfDay >= 1.0f) {
        world.timeOfDay -= std::floor(world.timeOfDay);
    }

    world.wavePhase += WAVE_SPEED * dt;
    if (world.wavePhase > 2.0f * PI) {
        world.wavePhase -= 2.0f * PI;
    }

    // El agua sube hasta el nivel maximo y ahi se queda.
    world.waterLevel = std::max(maxWaterLevel, world.waterLevel - waterRiseSpeed * dt);
}

void Simulation::applyMagnifier() {
    // Dos pasadas separadas: cada una escribe solo su propio arreglo, de modo que
    // ningun elemento tiene que modificar a otro.
    int bubbleCount = static_cast<int>(bubbles.size());
    int starfishCount = static_cast<int>(starfish.size());

    for (int i = 0; i < bubbleCount; i++) {
        Bubble& bubble = bubbles[i];
        bubble.magnified = false;
        for (int j = 0; j < starfishCount; j++) {
            const Starfish& star = starfish[j];
            float dx = star.x - bubble.x;
            float dy = star.y - bubble.y;
            float reach = bubble.radius + star.radius;
            if (dx * dx + dy * dy < reach * reach) {
                bubble.magnified = true;
                break;
            }
        }
    }

    for (int i = 0; i < starfishCount; i++) {
        Starfish& star = starfish[i];
        star.magnified = false;
        for (int j = 0; j < bubbleCount; j++) {
            const Bubble& bubble = bubbles[j];
            float dx = bubble.x - star.x;
            float dy = bubble.y - star.y;
            float reach = bubble.radius + star.radius;
            if (dx * dx + dy * dy < reach * reach) {
                star.magnified = true;
                break;
            }
        }
    }
}

void Simulation::resolveTurtleCollisions() {
    // Cada tortuga calcula su propia velocidad leyendo solo posiciones ajenas.
    int turtleCount = static_cast<int>(turtles.size());

    for (int i = 0; i < turtleCount; i++) {
        Turtle& turtle = turtles[i];

        for (int j = 0; j < turtleCount; j++) {
            if (i == j) {
                continue;
            }
            const Turtle& other = turtles[j];

            float dx = turtle.x - other.x;
            float dy = turtle.y - other.y;
            float distanceSquared = dx * dx + dy * dy;
            float reach = turtle.radius + other.radius;

            if (distanceSquared >= reach * reach || distanceSquared < 1e-6f) {
                continue;
            }

            float distance = std::sqrt(distanceSquared);
            float nx = dx / distance;
            float ny = dy / distance;

            // Solo rebota si venia acercandose, para que no quede pegada vibrando.
            float approach = turtle.vx * nx + turtle.vy * ny;
            if (approach < 0.0f) {
                turtle.vx -= 2.0f * approach * nx;
                turtle.vy -= 2.0f * approach * ny;
            }
        }
    }
}

void Simulation::updateElements(float dt) {
    int bubbleCount = static_cast<int>(bubbles.size());
    for (int i = 0; i < bubbleCount; i++) {
        updateBubble(bubbles[i], dt, world);
    }

    int starfishCount = static_cast<int>(starfish.size());
    for (int i = 0; i < starfishCount; i++) {
        updateStarfish(starfish[i], dt, world);
    }

    int turtleCount = static_cast<int>(turtles.size());
    for (int i = 0; i < turtleCount; i++) {
        updateTurtle(turtles[i], dt, world);
    }
}
