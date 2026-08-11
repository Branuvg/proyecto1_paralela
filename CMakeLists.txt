cmake_minimum_required(VERSION 3.10)
project(MarineSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# macOS specific settings for OpenMP
if(APPLE)
    if(NOT CMAKE_C_COMPILER_LOADED)
        enable_language(C)
    endif()
    set(OpenMP_C_FLAGS "-Xclang -fopenmp")
    set(OpenMP_C_LIB_NAMES "omp")
    set(OpenMP_CXX_FLAGS "-Xclang -fopenmp")
    set(OpenMP_CXX_LIB_NAMES "omp")
    set(OpenMP_omp_LIBRARY "/opt/homebrew/opt/libomp/lib/libomp.dylib")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xclang -fopenmp")
endif()

# Find SFML (3.0 or higher)
find_package(SFML 3 COMPONENTS Audio Graphics Network System Window REQUIRED)

# Find OpenMP
find_package(OpenMP REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/simulation.cpp
    src/renderer.cpp
)

set(HEADERS
    src/elements.h
    src/simulation.h
    src/renderer.h
)

# Create executable
add_executable(marine_sim ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(marine_sim PRIVATE
    SFML::Audio
    SFML::Graphics
    SFML::Network
    SFML::System
    SFML::Window
    OpenMP::OpenMP_CXX
)

# Include directories
target_include_directories(marine_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Include OpenMP headers
if(APPLE)
    target_include_directories(marine_sim PRIVATE /opt/homebrew/opt/libomp/include)
endif()
