#include <SFML/Graphics.hpp>

#include <chrono>
#include <iostream>
#include <sstream>

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
    simulation.populate(config.elementCount, config.seed);

    Renderer renderer(static_cast<float>(config.windowWidth),
                      static_cast<float>(config.windowHeight));
    if (!renderer.loadFont()) {
        std::cout << "Aviso: no se encontro ninguna fuente, los FPS solo saldran "
                     "en el titulo de la ventana.\n\n";
    }

    Metrics metrics;
    Stopwatch sectionTimer;
    sf::Clock deltaClock;
    int renderedFrames = 0;
    float titleFps = -1.0f;

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
            //     ...
            // }
        }

        float dt = deltaClock.restart().asSeconds();

        sectionTimer.start();
        simulation.update(dt);
        double updateMs = sectionTimer.stopMs();

        sectionTimer.start();
        window.clear();
        renderer.render(window, simulation, metrics.fps());
        window.display();
        double renderMs = sectionTimer.stopMs();

        metrics.endFrame(updateMs, renderMs);

        // El titulo repite los FPS por si no hubo fuente disponible para el HUD.
        if (metrics.fps() != titleFps) {
            titleFps = metrics.fps();
            std::ostringstream title;
            title.precision(1);
            title << std::fixed << "Screensaver acuatico - " << titleFps << " FPS - N="
                  << config.elementCount;
            window.setTitle(title.str());
        }

        renderedFrames++;
        if (config.frameLimit > 0 && renderedFrames >= config.frameLimit) {
            window.close();
        }
    }

    metrics.printSummary(config.elementCount, activeThreads);
    return 0;
}
