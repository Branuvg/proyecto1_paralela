cmake_minimum_required(VERSION 3.16)
project(AquaticScreensaver CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif()

# AppleClang no trae OpenMP: se le indica donde quedo el libomp de Homebrew.
if(APPLE)
    foreach(BREW_PREFIX /opt/homebrew /usr/local)
        if(EXISTS "${BREW_PREFIX}/opt/libomp/lib/libomp.dylib")
            set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I${BREW_PREFIX}/opt/libomp/include")
            set(OpenMP_CXX_LIB_NAMES "omp")
            set(OpenMP_omp_LIBRARY "${BREW_PREFIX}/opt/libomp/lib/libomp.dylib")
            break()
        endif()
    endforeach()
endif()

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)
find_package(OpenMP REQUIRED)

set(SOURCES
    src/main.cpp
    src/config.cpp
    src/elements.cpp
    src/metrics.cpp
    src/simulation.cpp
    src/renderer.cpp
)

set(HEADERS
    src/config.h
    src/elements.h
    src/metrics.h
    src/simulation.h
    src/renderer.h
)

# Los dos ejecutables comparten las mismas fuentes: la unica diferencia es
# enlazar OpenMP o no. Sin OpenMP el compilador ignora los #pragma omp.
add_executable(screensaver_seq ${SOURCES} ${HEADERS})
target_include_directories(screensaver_seq PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(screensaver_seq PRIVATE SFML::Graphics SFML::Window SFML::System)

add_executable(screensaver_par ${SOURCES} ${HEADERS})
target_include_directories(screensaver_par PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(screensaver_par PRIVATE SFML::Graphics SFML::Window SFML::System OpenMP::OpenMP_CXX)
