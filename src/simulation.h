#pragma once

#include "elements.h"
#include <memory>
#include <vector>
#include <ctime>

class Simulation {
public:
    Simulation(float width, float height);
    ~Simulation();

    void update(float dt);
    void addBubble(float x, float y);
    void addStarfish(float x, float y);
    void addTurtle(float x, float y);
    void addCrab(float x, float y);

    const std::vector<std::shared_ptr<Element>>& getElements() const;

    float getWaterLevel() const { return screenHeight * 0.20f; }

private:
    float screenWidth, screenHeight;
    std::vector<std::shared_ptr<Element>> elements;

    void updateElements(float dt);
    void checkCollisions();
    void spawnCrabIfHourChanged();

    int lastHour;
};
