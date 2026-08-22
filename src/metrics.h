#pragma once

#include <chrono>
#include <cstddef>

// Cronometro para medir una seccion del frame en milisegundos.
class Stopwatch {
public:
    void start();
    double stopMs();

private:
    std::chrono::steady_clock::time_point beginTime;
};

// Acumula tiempos de frame: alimenta el HUD de FPS y el reporte de speedup.
class Metrics {
public:
    explicit Metrics(double refreshSeconds = 0.5);

    void beginFrame();
    void endFrame(double updateMs, double renderMs);

    float fps() const { return displayedFps; }
    std::size_t frameCount() const { return totalFrames; }

    double averageFrameMs() const;
    double averageUpdateMs() const;
    double averageRenderMs() const;

    // Resumen final para la bitacora de pruebas del informe.
    void printSummary(int elementCount, int threadCount) const;

private:
    double refreshSeconds;   // cada cuanto se refresca el numero que ve el usuario
    Stopwatch frameTimer;

    std::size_t totalFrames;
    double totalFrameMs;
    double totalUpdateMs;
    double totalRenderMs;

    std::size_t framesSinceRefresh;
    double msSinceRefresh;
    float displayedFps;
};
