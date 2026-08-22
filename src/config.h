#pragma once

// Limites aceptados para los argumentos de linea de comandos.
constexpr int MIN_WINDOW_WIDTH = 640;
constexpr int MIN_WINDOW_HEIGHT = 480;
constexpr int MAX_WINDOW_SIDE = 7680;
constexpr int MAX_ELEMENT_COUNT = 5000000;
constexpr int MAX_THREAD_COUNT = 256;

// Parametros de ejecucion leidos de la linea de comandos.
struct Config {
    int elementCount = 300;
    int windowWidth = 1200;
    int windowHeight = 800;
    int threadCount = 0;   // 0 = dejar que OpenMP escoja
    unsigned seed = 0;     // 0 = derivar la semilla del reloj
    int frameLimit = 0;    // 0 = correr hasta que se cierre la ventana
};

enum class ParseResult {
    OK,
    HELP_REQUESTED,
    ERROR
};

// Llena config a partir de argv, validando cada valor.
ParseResult parseArgs(int argc, char* argv[], Config& config);

void printUsage(const char* programName);

// Imprime los parametros con los que va a correr el programa.
void printConfig(const Config& config, int activeThreads);
