# Setup del Proyecto

Guía para compilar y ejecutar el proyecto en diferentes plataformas.

## macOS (Apple Silicon / Intel)

### 1. Instalar Homebrew (si no está instalado)
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 2. Instalar dependencias
```bash
brew install sfml cmake libomp
```

### 3. Compilar el proyecto
```bash
mkdir build
cd build
cmake ..
make
```

### 4. Ejecutar
```bash
./marine_sim
```

---

## Linux (Ubuntu/Debian)

### 1. Instalar dependencias
```bash
sudo apt-get update
sudo apt-get install build-essential cmake libsfml-dev libomp-dev
```

### 2. Compilar el proyecto
```bash
mkdir build
cd build
cmake ..
make
```

### 3. Ejecutar
```bash
./marine_sim
```

---

## Windows (MSVC)

### 1. Instalar herramientas
- Visual Studio (con C++ tools)
- CMake
- SFML (descargar desde sfml-dev.org)

### 2. Configurar SFML
```bash
# En tu directorio de usuario:
mkdir SFML
# Copiar SFML descargado aquí
```

### 3. Compilar
```bash
mkdir build
cd build
cmake .. -DSFML_DIR=C:\path\to\SFML\lib\cmake\SFML
cmake --build . --config Release
```

### 4. Ejecutar
```bash
Release\marine_sim.exe
```

---

## Solución de problemas

### CMake no encuentra SFML
```bash
# macOS
cmake .. -DSFML_DIR=$(brew --prefix sfml)/lib/cmake/SFML

# Linux
cmake .. -DSFML_DIR=/usr/lib/x86_64-linux-gnu/cmake/SFML
```

### Compilación lenta en macOS
Asegurate de que estés usando la arquitectura correcta:
```bash
# Para Apple Silicon:
cmake -DCMAKE_OSX_ARCHITECTURES=arm64 ..

# Para Intel:
cmake -DCMAKE_OSX_ARCHITECTURES=x86_64 ..
```

### OpenMP no se encuentra
```bash
# macOS - LLVM fallback
brew install llvm
export CC=clang
export CXX=clang++
cmake ..
```
