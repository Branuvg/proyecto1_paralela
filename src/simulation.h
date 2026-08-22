#pragma once

#include <vector>

#include "elements.h"

class Simulation {
public:
    Simulation(float width, float height);

    // Reparte elementCount entre los tres tipos y los coloca aleatoriamente.
    void populate(int elementCount, unsigned seed);

    void update(float dt);

    const std::vector<Bubble>& getBubbles() const { return bubbles; }
    const std::vector<Starfish>& getStarfish() const { return starfish; }
    const std::vector<Turtle>& getTurtles() const { return turtles; }
    const World& getWorld() const { return world; }

private:
    void advanceWorld(float dt);
    void applyMagnifier();            // efecto lupa entre burbujas y estrellas
    void resolveTurtleCollisions();   // solo ajusta velocidades, no posiciones
    void updateElements(float dt);

    World world;
    float maxWaterLevel;   // y mas alta que alcanza el agua, donde se detiene
    float waterRiseSpeed;

    std::vector<Bubble> bubbles;
    std::vector<Starfish> starfish;
    std::vector<Turtle> turtles;
};
