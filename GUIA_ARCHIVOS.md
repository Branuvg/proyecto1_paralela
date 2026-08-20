# Guía: Qué hay en cada archivo

## 📁 Archivos Principales

### `src/main.cpp` (54 líneas)
**¿Qué es?** El programa principal - donde empieza todo

**Qué encontrarás:**
- Línea 9: `sf::RenderWindow` → **Crea la ventana gráfica SFML**
- Línea 12-13: Inicializa `Simulation` y `Renderer` → **Crea los sistemas**
- Línea 16-20: Agrega elementos iniciales (burbujas, estrellas, tortugas)
- Línea 26: `while (window.isOpen())` → **Ciclo principal**
- Línea 27-42: Manejo de eventos (clicks del mouse)
- Línea 44-45: Actualiza simulación y renderiza

**En palabras simples:** "Abre la ventana, crea los elementos, y cada frame actualiza y dibuja todo"

---

### `src/elements.h` (127 líneas)
**¿Qué es?** Definición de todos los elementos del juego

**Qué encontrarás:**
- Línea 13-28: Clase `Element` base → **Padre de todos**
- Línea 30-47: Clase `Bubble` → **La burbuja**
  - update(): sube hacia arriba
  - draw(): círculo con brillo
- Línea 49-68: Clase `Starfish` → **Estrella de mar**
  - update(): movimiento libre
  - draw(): estrella amarilla
- Línea 70-98: Clase `Turtle` → **Tortuga**
  - update(): movimiento diagonal
  - draw(): caparazón + cabeza
- Línea 100-120: Clase `Crab` → **Cangrejo**
  - update(): fijo en agua
  - draw(): con pinzas

**En palabras simples:** "Aquí definimos cómo se ven y se mueven todos los elementos"

---

### `src/simulation.h` (30 líneas)
**¿Qué es?** El cerebro de la simulación

**Qué encontrarás:**
- **Línea 15:** `std::vector<std::shared_ptr<Element>> elements;` 
  → **AQUÍ GUARDAMOS TODOS LOS ELEMENTOS** ⭐
- Línea 19: `update(float dt)` → Actualiza todo cada frame
- Línea 20-23: `addBubble()`, `addStarfish()`, etc → Agregar elementos
- Línea 25: `getWaterLevel()` → Dónde empieza el agua (20%)

**En palabras simples:** "Aquí guardamos la lista de elementos y les decimos que se actualicen"

---

### `src/simulation.cpp` (91 líneas)
**¿Qué es?** La lógica de actualización

**Qué encontrarás:**
- **Línea 27-31:** `#pragma omp parallel for` 
  → **PARALELIZACIÓN CON OPENMP** ⭐
  - Cada thread actualiza elementos diferentes al mismo tiempo
- Línea 34-58: `checkCollisions()` → Detecta choques
- Línea 61-67: `spawnCrabIfHourChanged()` → Cangrejo cada hora

**En palabras simples:** "Aquí se actualiza todo en paralelo (rápido) y se detectan colisiones"

---

### `src/renderer.h` (28 líneas)
**¿Qué es?** Define cómo se dibuja todo

**Qué encontrarás:**
- Línea 9: `render()` → Función principal
- Línea 13: `drawSky()` → Cielo
- Línea 14: `drawWater()` → Agua
- Línea 15: `drawWavePattern()` → Olas
- Línea 17: `getSkyColor()` → Color por hora

**En palabras simples:** "Estructura de lo que dibujamos"

---

### `src/renderer.cpp` (80 líneas)
**¿Qué es?** La implementación del renderizado

**Qué encontrarás:**
- Línea 9: `render()` → Llama todas las funciones en orden
- Línea 20-28: `drawSky()` → Cielo según hora
- Línea 30-35: `drawWater()` → Agua azul
- **Línea 37-52:** `drawWavePattern()` → **Animación de olas** ⭐
  - Línea 44: `waveOffset += 0.05f` → Las olas se mueven
- Línea 63-70: `getSkyColor()` → Lógica de colores

**En palabras simples:** "Aquí se dibuja todo en SFML"

---

### `CMakeLists.txt` (50 líneas)
**¿Qué es?** Configuración de compilación

**Qué encontrarás:**
- Línea 10-17: Configuración OpenMP para macOS
- Línea 19: `find_package(SFML 3)` → Busca SFML
- Línea 22: `find_package(OpenMP)` → Busca OpenMP
- Línea 34-41: `target_link_libraries` → Vincula librerías

**En palabras simples:** "Cómo compilar el proyecto"

---

## 🎯 Respuestas Rápidas

| Pregunta | Respuesta |
|----------|-----------|
| ¿Dónde están los elementos guardados? | `src/simulation.h:15` - `std::vector` |
| ¿Cómo se actualizan en paralelo? | `src/simulation.cpp:27` - `#pragma omp parallel for` |
| ¿Dónde se dibuja todo? | `src/renderer.cpp` - función `render()` |
| ¿Cielo que cambia de color? | `src/renderer.cpp:63` - `getSkyColor(hour)` |
| ¿Dónde está el ciclo principal? | `src/main.cpp:26` - `while (window.isOpen())` |
| ¿Cómo se crean elementos? | `src/simulation.h:20-23` - métodos `add*()` |
| ¿API gráfica? | `src/main.cpp:9` - SFML (sf::RenderWindow) |
| ¿Olas animadas? | `src/renderer.cpp:37-52` - wave pattern |
