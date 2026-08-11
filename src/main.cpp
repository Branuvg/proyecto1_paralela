#include <SFML/Graphics.hpp>
#include "simulation.h"
#include "renderer.h"

int main() {
    const float WINDOW_WIDTH = 1200.0f;
    const float WINDOW_HEIGHT = 800.0f;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Marine Simulation");
    window.setFramerateLimit(60);

    Simulation simulation(WINDOW_WIDTH, WINDOW_HEIGHT);
    Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT);

    simulation.addBubble(200, 300);
    simulation.addBubble(400, 250);
    simulation.addBubble(600, 400);

    simulation.addStarfish(300, 500);
    simulation.addStarfish(700, 550);

    simulation.addTurtle(150, 450);
    simulation.addTurtle(900, 480);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                float x = static_cast<float>(event.mouseButton.x);
                float y = static_cast<float>(event.mouseButton.y);

                if (y < simulation.getWaterLevel()) {
                    simulation.addStarfish(x, y);
                } else {
                    simulation.addBubble(x, y);
                }
            }
        }

        float dt = clock.restart().asSeconds();
        simulation.update(dt);

        window.clear();
        renderer.render(window, simulation, simulation.getWaterLevel());
        window.display();
    }

    return 0;
}
