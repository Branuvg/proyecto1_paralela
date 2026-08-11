cmake_minimum_required(VERSION 3.10)
project(MarineSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

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
    sfml-graphics
    sfml-window
    sfml-system
    OpenMP::OpenMP_CXX
)

# Include directories
target_include_directories(marine_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
