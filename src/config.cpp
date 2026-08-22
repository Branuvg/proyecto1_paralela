#include "config.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace {

// Convierte texto a entero rechazando basura, vacios y desbordes.
bool toInt(const char* text, long& out) {
    if (text == nullptr || *text == '\0') {
        return false;
    }
    errno = 0;
    char* end = nullptr;
    long value = std::strtol(text, &end, 10);
    if (errno == ERANGE || end == text || *end != '\0') {
        return false;
    }
    out = value;
    return true;
}

// Lee el valor que sigue a una bandera y verifica que este dentro del rango.
bool readIntArg(int argc, char* argv[], int& index, const char* flag,
                long minValue, long maxValue, long& out) {
    if (index + 1 >= argc) {
        std::cerr << "Error: la bandera " << flag << " necesita un valor.\n";
        return false;
    }
    const char* text = argv[++index];
    if (!toInt(text, out)) {
        std::cerr << "Error: " << flag << " recibio \"" << text
                  << "\", que no es un numero entero valido.\n";
        return false;
    }
    if (out < minValue || out > maxValue) {
        std::cerr << "Error: " << flag << " debe estar entre " << minValue
                  << " y " << maxValue << " (recibio " << out << ").\n";
        return false;
    }
    return true;
}

}  // namespace

ParseResult parseArgs(int argc, char* argv[], Config& config) {
    for (int i = 1; i < argc; i++) {
        const char* arg = argv[i];
        long value = 0;

        if (std::strcmp(arg, "--help") == 0 || std::strcmp(arg, "-h") == 0) {
            return ParseResult::HELP_REQUESTED;
        } else if (std::strcmp(arg, "--n") == 0) {
            if (!readIntArg(argc, argv, i, "--n", 1, MAX_ELEMENT_COUNT, value)) {
                return ParseResult::ERROR;
            }
            config.elementCount = static_cast<int>(value);
        } else if (std::strcmp(arg, "--width") == 0) {
            if (!readIntArg(argc, argv, i, "--width", MIN_WINDOW_WIDTH, MAX_WINDOW_SIDE, value)) {
                return ParseResult::ERROR;
            }
            config.windowWidth = static_cast<int>(value);
        } else if (std::strcmp(arg, "--height") == 0) {
            if (!readIntArg(argc, argv, i, "--height", MIN_WINDOW_HEIGHT, MAX_WINDOW_SIDE, value)) {
                return ParseResult::ERROR;
            }
            config.windowHeight = static_cast<int>(value);
        } else if (std::strcmp(arg, "--threads") == 0) {
            if (!readIntArg(argc, argv, i, "--threads", 1, MAX_THREAD_COUNT, value)) {
                return ParseResult::ERROR;
            }
            config.threadCount = static_cast<int>(value);
        } else if (std::strcmp(arg, "--seed") == 0) {
            if (!readIntArg(argc, argv, i, "--seed", 1, 2147483647L, value)) {
                return ParseResult::ERROR;
            }
            config.seed = static_cast<unsigned>(value);
        } else if (std::strcmp(arg, "--frames") == 0) {
            if (!readIntArg(argc, argv, i, "--frames", 1, 1000000, value)) {
                return ParseResult::ERROR;
            }
            config.frameLimit = static_cast<int>(value);
        } else {
            std::cerr << "Error: argumento desconocido \"" << arg << "\".\n";
            return ParseResult::ERROR;
        }
    }
    return ParseResult::OK;
}

void printUsage(const char* programName) {
    std::cout
        << "Screensaver acuatico - Proyecto 1 de Computacion Paralela\n\n"
        << "Uso: " << programName << " [opciones]\n\n"
        << "  --n <entero>        Cantidad de elementos a renderizar (default 300)\n"
        << "  --width <entero>    Ancho de la ventana en pixeles (minimo "
        << MIN_WINDOW_WIDTH << ", default 1200)\n"
        << "  --height <entero>   Alto de la ventana en pixeles (minimo "
        << MIN_WINDOW_HEIGHT << ", default 800)\n"
        << "  --threads <entero>  Hilos de OpenMP (default: los que escoja el sistema)\n"
        << "  --seed <entero>     Semilla del generador aleatorio (default: el reloj)\n"
        << "  --frames <entero>   Corre esa cantidad de frames, imprime tiempos y sale\n"
        << "  --help, -h          Muestra esta ayuda\n\n"
        << "Ejemplo: " << programName << " --n 5000 --width 800 --height 600 --threads 4\n";
}

void printConfig(const Config& config, int activeThreads) {
    std::cout << "Configuracion:\n"
              << "  elementos : " << config.elementCount << "\n"
              << "  ventana   : " << config.windowWidth << "x" << config.windowHeight << "\n"
              << "  hilos     : " << activeThreads << "\n"
              << "  semilla   : " << config.seed << "\n";
    if (config.frameLimit > 0) {
        std::cout << "  frames    : " << config.frameLimit << " (modo medicion)\n";
    }
    std::cout << std::endl;
}
