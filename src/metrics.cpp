#include "metrics.h"

#include <iomanip>
#include <iostream>

void Stopwatch::start() {
    beginTime = std::chrono::steady_clock::now();
}

double Stopwatch::stopMs() {
    auto elapsed = std::chrono::steady_clock::now() - beginTime;
    return std::chrono::duration<double, std::milli>(elapsed).count();
}

Metrics::Metrics(double refreshSeconds)
    : refreshSeconds(refreshSeconds),
      totalFrames(0),
      totalFrameMs(0.0),
      totalUpdateMs(0.0),
      totalRenderMs(0.0),
      framesSinceRefresh(0),
      msSinceRefresh(0.0),
      displayedFps(0.0f) {}

void Metrics::beginFrame() {
    frameTimer.start();
}

void Metrics::endFrame(double updateMs, double renderMs) {
    double frameMs = frameTimer.stopMs();

    totalFrames++;
    totalFrameMs += frameMs;
    totalUpdateMs += updateMs;
    totalRenderMs += renderMs;

    framesSinceRefresh++;
    msSinceRefresh += frameMs;

    // El HUD se actualiza cada refreshSeconds para que el numero se pueda leer.
    if (msSinceRefresh >= refreshSeconds * 1000.0) {
        displayedFps = static_cast<float>(framesSinceRefresh * 1000.0 / msSinceRefresh);
        framesSinceRefresh = 0;
        msSinceRefresh = 0.0;
    }
}

double Metrics::averageFrameMs() const {
    return totalFrames == 0 ? 0.0 : totalFrameMs / static_cast<double>(totalFrames);
}

double Metrics::averageUpdateMs() const {
    return totalFrames == 0 ? 0.0 : totalUpdateMs / static_cast<double>(totalFrames);
}

double Metrics::averageRenderMs() const {
    return totalFrames == 0 ? 0.0 : totalRenderMs / static_cast<double>(totalFrames);
}

void Metrics::printSummary(int elementCount, int threadCount) const {
    if (totalFrames == 0) {
        std::cout << "No se midio ningun frame.\n";
        return;
    }

    double avgFrame = averageFrameMs();
    double avgFps = avgFrame > 0.0 ? 1000.0 / avgFrame : 0.0;

    std::cout << std::fixed << std::setprecision(3)
              << "\n--- Resumen de medicion ---\n"
              << "  elementos        : " << elementCount << "\n"
              << "  hilos            : " << threadCount << "\n"
              << "  frames medidos   : " << totalFrames << "\n"
              << "  frame promedio   : " << avgFrame << " ms\n"
              << "  update promedio  : " << averageUpdateMs() << " ms\n"
              << "  render promedio  : " << averageRenderMs() << " ms\n"
              << std::setprecision(2)
              << "  FPS promedio     : " << avgFps << "\n"
              << "  tiempo total     : " << totalFrameMs / 1000.0 << " s\n";
}
