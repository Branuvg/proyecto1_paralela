#include "elements.h"

#include <algorithm>
#include <cmath>

namespace {

constexpr float MAGNIFY_SCALE = 1.9f;   // cuanto crece un elemento bajo la lupa
constexpr float MAGNIFY_SPEED = 7.0f;   // que tan rapido llega a ese tamano

// Acerca el radio a su objetivo: crece si esta bajo la lupa, si no vuelve al base.
float relaxRadius(float radius, float baseRadius, bool magnified, float dt) {
    float target = magnified ? baseRadius * MAGNIFY_SCALE : baseRadius;
    float step = std::min(1.0f, MAGNIFY_SPEED * dt);
    return radius + (target - radius) * step;
}

}  // namespace

float waterSurfaceAt(const World& world, float x) {
    return world.waterLevel +
           world.waveAmplitude * std::sin(world.waveFrequency * x + world.wavePhase);
}

void updateBubble(Bubble& bubble, float dt, const World& world) {
    bubble.y -= bubble.riseSpeed * dt;
    bubble.radius = relaxRadius(bubble.radius, bubble.baseRadius, bubble.magnified, dt);

    // Al cruzar la superficie la burbuja se recicla en el fondo, sobre la misma x.
    if (bubble.y + bubble.radius < waterSurfaceAt(world, bubble.x)) {
        bubble.y = world.height + bubble.radius;
    }
}

void updateStarfish(Starfish& starfish, float dt, const World& world) {
    starfish.angle += starfish.spin * dt;
    if (starfish.angle > 2.0f * PI) {
        starfish.angle -= 2.0f * PI;
    } else if (starfish.angle < 0.0f) {
        starfish.angle += 2.0f * PI;
    }

    starfish.radius = relaxRadius(starfish.radius, starfish.baseRadius, starfish.magnified, dt);
    (void)world;  // la estrella gira sobre su eje, no se traslada
}

void updateTurtle(Turtle& turtle, float dt, const World& world) {
    turtle.x += turtle.vx * dt;
    turtle.y += turtle.vy * dt;

    // Rebote contra los bordes laterales: se invierte la componente horizontal.
    if (turtle.x - turtle.radius < 0.0f) {
        turtle.x = turtle.radius;
        turtle.vx = -turtle.vx;
    } else if (turtle.x + turtle.radius > world.width) {
        turtle.x = world.width - turtle.radius;
        turtle.vx = -turtle.vx;
    }

    // Arriba rebota contra la superficie del agua y abajo contra el fondo.
    float surface = waterSurfaceAt(world, turtle.x) + turtle.radius;
    if (turtle.y < surface) {
        turtle.y = surface;
        turtle.vy = -turtle.vy;
    } else if (turtle.y + turtle.radius > world.height) {
        turtle.y = world.height - turtle.radius;
        turtle.vy = -turtle.vy;
    }
}
