#pragma once

#include <SFML/Graphics.hpp>

constexpr float PI = 3.14159265358979f;

// Estado compartido de la escena que los elementos necesitan para actualizarse.
struct World {
    float width = 0.0f;
    float height = 0.0f;
    float waterLevel = 0.0f;     // y de la superficie en reposo; baja mientras el agua sube
    float wavePhase = 0.0f;
    float waveAmplitude = 0.0f;
    float waveFrequency = 0.0f;
    float timeOfDay = 0.0f;      // 0 a 1 ciclico: amanecer, dia, atardecer, noche
};

// y de la superficie del agua en una x dada, ya con el oleaje aplicado.
float waterSurfaceAt(const World& world, float x);

struct Bubble {
    float x = 0.0f, y = 0.0f;
    float riseSpeed = 0.0f;
    float baseRadius = 0.0f;
    float radius = 0.0f;
    bool magnified = false;      // lo marca el efecto lupa
    sf::Color color;
};

struct Starfish {
    float x = 0.0f, y = 0.0f;
    float angle = 0.0f;
    float spin = 0.0f;           // velocidad de rotacion en radianes por segundo
    float baseRadius = 0.0f;
    float radius = 0.0f;
    bool magnified = false;
    sf::Color color;
};

struct Turtle {
    float x = 0.0f, y = 0.0f;
    float vx = 0.0f, vy = 0.0f;
    float radius = 0.0f;
    sf::Color color;
};

void updateBubble(Bubble& bubble, float dt, const World& world);
void updateStarfish(Starfish& starfish, float dt, const World& world);
void updateTurtle(Turtle& turtle, float dt, const World& world);
