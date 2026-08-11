# Hito 1: Estructura Base Completada

## Commit
**Hash:** `6c228be`  
**Tag:** `v0.1-initial-structure`

---

## Requisitos Cumplidos ✅

### i. Ventana Gráfica
- **API:** SFML 3.0
- **Archivo:** `src/main.cpp`
- Ventana 1200x800 a 60 FPS

### ii. Variable en Memoria para Elementos
- **Tipo:** `std::vector<std::shared_ptr<Element>>`
- **Ubicación:** `src/simulation.h:15`
- Almacena todos los elementos dinámicamente

### iii. Funciones de Renderizado y Actualización
Implementadas para cada elemento:

| Elemento | Update | Draw |
|----------|--------|------|
| Bubble | Sube hacia superficie | Círculo con brillo |
| Starfish | Movimiento libre | Estrella amarilla |
| Turtle | Movimiento diagonal | Con cabeza visible |
| Crab | Fijo en agua | Con pinzas |

---

## Características Extra
- ✅ Paralelización con OpenMP
- ✅ Detección de colisiones
- ✅ Cielo que cambia por hora
- ✅ Animación de olas en el agua
- ✅ Interactividad con mouse

---

## Compilación

```bash
mkdir build && cd build
cmake ..
make
./marine_sim
```

**Requisitos:**
- SFML 3.0 (`brew install sfml`)
- OpenMP (`brew install libomp`)
- CMake 3.10+
- C++17

---

## Proyecto Listo para Evaluación ✓
