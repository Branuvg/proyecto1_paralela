#include <SFML/Graphics.hpp>

#include <chrono>
#include <iostream>
#include <random>

#include "config.h"
#include "metrics.h"
#include "renderer.h"
#include "simulation.h"

#ifdef _OPENMP
#include <omp.h>
#endif

namespace {

// Aplica el numero de hilos pedido y devuelve los que realmente quedaron activos.
int setupThreads(const Config& config) {
#ifdef _OPENMP
    if (config.threadCount > 0) {
        omp_set_num_threads(config.threadCount);
    }
    return omp_get_max_threads();
#else
    (void)config;
    return 1;
#endif
}

// Reparte los N elementos entre los tres tipos en posiciones aleatorias.
void spawnElements(Simulation& simulation, const Config& config, std::mt19937& rng) {
    std::uniform_real_distribution<float> randomX(0.0f, static_cast<float>(config.windowWidth));
    std::uniform_real_distribution<float> randomY(0.0f, static_cast<float>(config.windowHeight));

    for (int i = 0; i < config.elementCount; i++) {
        float x = randomX(rng);
        float y = randomY(rng);

        switch (i % 3) {
            case 0: simulation.addBubble(x, y); break;
            case 1: simulation.addStarfish(x, y); break;
            default: simulation.addTurtle(x, y); break;
        }
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    Config config;
    switch (parseArgs(argc, argv, config)) {
        case ParseResult::HELP_REQUESTED:
            printUsage(argv[0]);
            return 0;
        case ParseResult::ERROR:
            std::cerr << "\nUse " << argv[0] << " --help para ver las opciones.\n";
            return 1;
        case ParseResult::OK:
            break;
    }

    // Con semilla 0 se toma la del reloj, pero se guarda para poder repetir la prueba.
    if (config.seed == 0) {
        config.seed = static_cast<unsigned>(
            std::chrono::steady_clock::now().time_since_epoch().count());
    }

    int activeThreads = setupThreads(config);
    printConfig(config, activeThreads);

    auto width = static_cast<unsigned>(config.windowWidth);
    auto height = static_cast<unsigned>(config.windowHeight);

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(width, height)),
                            "Screensaver acuatico");

    // En modo medicion no se limitan los FPS: el vsync falsearia los tiempos.
    if (config.frameLimit == 0) {
        window.setFramerateLimit(60);
    }

    Simulation simulation(static_cast<float>(config.windowWidth),
                          static_cast<float>(config.windowHeight));
    Renderer renderer(static_cast<float>(config.windowWidth),
                      static_cast<float>(config.windowHeight));

    std::mt19937 rng(config.seed);
    spawnElements(simulation, config, rng);

    Metrics metrics;
    Stopwatch sectionTimer;
    sf::Clock deltaClock;
    int renderedFrames = 0;

    while (window.isOpen()) {
        metrics.beginFrame();

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (auto key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
            }

            // Agregar elementos con el mouse queda desactivado: N viene por argumento.
            // if (auto click = event->getIf<sf::Event::MouseButtonPressed>()) {
            //     simulation.addBubble(static_cast<float>(click->position.x),
            //                          static_cast<float>(click->position.y));
            // }
        }

        float dt = deltaClock.restart().asSeconds();

        sectionTimer.start();
        simulation.update(dt);
        double updateMs = sectionTimer.stopMs();

        sectionTimer.start();
        window.clear();
        renderer.render(window, simulation, simulation.getWaterLevel());
        window.display();
        double renderMs = sectionTimer.stopMs();

        metrics.endFrame(updateMs, renderMs);

        renderedFrames++;
        if (config.frameLimit > 0 && renderedFrames >= config.frameLimit) {
            window.close();
        }
    }

    metrics.printSummary(config.elementCount, activeThreads);
    return 0;
}
