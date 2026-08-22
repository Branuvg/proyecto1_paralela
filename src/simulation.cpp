#include "simulation.h"
#include <cstdlib>

Simulation::Simulation(float width, float height)
    : screenWidth(width), screenHeight(height) {
    lastHour = -1;
}

Simulation::~Simulation() = default;

void Simulation::update(float dt) {
    updateElements(dt);
    checkCollisions();
    spawnCrabIfHourChanged();

    elements.erase(
        std::remove_if(elements.begin(), elements.end(),
                      [](const std::shared_ptr<Element>& e) { return !e->alive; }),
        elements.end()
    );
}

void Simulation::updateElements(float dt) {
    for (size_t i = 0; i < elements.size(); ++i) {
        elements[i]->update(dt, screenWidth, screenHeight);
    }
}

void Simulation::checkCollisions() {
    const float COLLISION_DISTANCE = 25.0f;

    for (size_t i = 0; i < elements.size(); ++i) {
        for (size_t j = i + 1; j < elements.size(); ++j) {
            auto& e1 = elements[i];
            auto& e2 = elements[j];

            float dx = e2->x - e1->x;
            float dy = e2->y - e1->y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist < COLLISION_DISTANCE) {
                if (e1->type == ElementType::BUBBLE && e2->type == ElementType::STARFISH) {
                    e1->alive = false;
                } else if (e1->type == ElementType::STARFISH && e2->type == ElementType::BUBBLE) {
                    e2->alive = false;
                } else if (e1->type == ElementType::TURTLE && e2->type == ElementType::TURTLE) {
                    auto* t1 = dynamic_cast<Turtle*>(e1.get());
                    auto* t2 = dynamic_cast<Turtle*>(e2.get());
                    if (t1 && t2) {
                        t1->angle -= M_PI / 3.0f;
                        t2->angle += M_PI / 3.0f;
                    }
                }
            }
        }
    }
}

void Simulation::spawnCrabIfHourChanged() {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    int currentHour = timeinfo->tm_hour;

    if (currentHour != lastHour) {
        lastHour = currentHour;
        addCrab(screenWidth / 2.0f, screenHeight / 2.0f);
    }
}

void Simulation::addBubble(float x, float y) {
    elements.push_back(std::make_shared<Bubble>(x, y));
}

void Simulation::addStarfish(float x, float y) {
    elements.push_back(std::make_shared<Starfish>(x, y));
}

void Simulation::addTurtle(float x, float y) {
    elements.push_back(std::make_shared<Turtle>(x, y));
}

void Simulation::addCrab(float x, float y) {
    elements.push_back(std::make_shared<Crab>(x, y));
}

const std::vector<std::shared_ptr<Element>>& Simulation::getElements() const {
    return elements;
}
