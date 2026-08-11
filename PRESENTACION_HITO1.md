# Entrega Hito 1: Estructura Base del Proyecto

## Información del Commit

**Hash del Commit:** `6c228be`  
**Tag:** `v0.1-initial-structure`  
**Fecha:** 10 de Agosto, 2026  
**Autor:** Anggie Quezada

---

## Requisitos Cumplidos

### i. ✅ Ventana Gráfica con API Decidida

**Ubicación:** `src/main.cpp:9-11`

```cpp
sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1200, 800)), 
                        "Marine Simulation");
window.setFramerateLimit(60);
```

**API Elegida:** **SFML 3.0** (Simple and Fast Multimedia Library)
- ✅ Ideal para gráficos 2D interactivos
- ✅ Compatible con OpenMP para paralelización
- ✅ Soporte para eventos de mouse y teclado
- ✅ Funciona en macOS, Windows, Linux

**Configuración:** `CMakeLists.txt`
- SFML 3.0+ con componentes Audio, Graphics, Network, System, Window
- OpenMP configurado para macOS

---

### ii. ✅ Variable en Memoria para Almacenar Elementos

**Ubicación:** `src/simulation.h:15`

```cpp
std::vector<std::shared_ptr<Element>> elements;
```

**Características:**
- Contenedor dinámico que crece según se agregan elementos
- Usa `std::shared_ptr` para manejo automático de memoria
- Accesible en `getElements()` para renderización
- Compatible con paralelización OpenMP

**Métodos de acceso:**
- `addBubble(x, y)` → Agrega burbuja
- `addStarfish(x, y)` → Agrega estrella de mar
- `addTurtle(x, y)` → Agrega tortuga
- `addCrab(x, y)` → Agrega cangrejo

---

### iii. ✅ Funciones de Renderizado y Actualización

**Elementos Implementados:**

#### Burbuja (Bubble)
- **Actualización:** `elements.h:38-45` - Sube hacia la superficie
- **Renderizado:** `elements.h:47-54` - Círculo blanco con borde azul
- **Física:** Velocidad vertical constante, desaparece al salir de pantalla

#### Estrella de Mar (Starfish)
- **Actualización:** `elements.h:66-78` - Se mueve libremente dentro de límites
- **Renderizado:** `elements.h:80-87` - Forma de estrella (5 puntas) en amarillo
- **Física:** Se mantiene dentro de pantalla con rebotes suaves

#### Tortuga (Turtle)
- **Actualización:** `elements.h:107-130` - Movimiento diagonal con rotación
- **Renderizado:** `elements.h:132-138` - Círculo verde
- **Física:** Movimiento en ángulo, rotación al chocar bordes/agua, velocidad 50 px/s

#### Cangrejo (Crab)
- **Actualización:** `elements.h:150-162` - Se queda en nivel del agua
- **Renderizado:** `elements.h:164-170` - Círculo rojo
- **Física:** Aparece automáticamente cada hora como sorpresa

---

## Características Adicionales Implementadas

### ✅ Sistema de Colores Dinámicos del Cielo
**Ubicación:** `src/renderer.cpp:58-70`

```cpp
if (hour >= 6 && hour < 11) {
    return sf::Color(135, 206, 235);  // Claro
} else if (hour >= 11 && hour < 19) {
    return sf::Color(255, 150, 100);  // Rojizo (atardecer)
} else {
    return sf::Color(30, 30, 60);     // Azul oscuro (noche)
}
```

### ✅ Paralelización con OpenMP
**Ubicación:** `src/simulation.cpp:27-31`

```cpp
#pragma omp parallel for
for (size_t i = 0; i < elements.size(); ++i) {
    elements[i]->update(dt, screenWidth, screenHeight);
}
```

### ✅ Detección de Colisiones Paralelizada
**Ubicación:** `src/simulation.cpp:34-58`

```cpp
#pragma omp parallel for collapse(2)
for (size_t i = 0; i < elements.size(); ++i) {
    for (size_t j = i + 1; j < elements.size(); ++j) {
        // Detección de colisiones entre elementos
    }
}
```

### ✅ Interactividad
- **Click en cielo:** Agrega estrella de mar
- **Click en agua:** Agrega burbuja

---

## Estructura del Proyecto

```
src/
├── main.cpp              → Punto de entrada, ciclo principal
├── elements.h            → Definiciones de elementos (Bubble, Starfish, Turtle, Crab)
├── simulation.h/cpp      → Lógica de simulación con paralelización
└── renderer.h/cpp        → Renderización con SFML

CMakeLists.txt            → Configuración de compilación
SETUP.md                  → Instrucciones de instalación
README.md                 → Documentación general
```

---

## Compilación y Ejecución

### macOS
```bash
mkdir build && cd build
cmake ..
make
./marine_sim
```

### Requisitos
- SFML 3.0 (vía Homebrew: `brew install sfml`)
- OpenMP (vía Homebrew: `brew install libomp`)
- CMake 3.10+
- C++17

---

## Screenshot

![Marine Simulation Running](screenshot.png)

**Se observa:**
- Cielo azul oscuro (noche)
- Agua azul en 3/4 de la pantalla
- Estrellas de mar (⭐ amarillas)
- Tortugas (🐢 verdes)
- Cangrejo (🦀 rojo)
- Todos los elementos animándose correctamente

---

## Próximos Pasos

1. **Zoom y explosión de burbujas** (al tocar estrella)
2. **Mejora de física de colisiones** (rotación 120° de tortugas)
3. **Animación del cangrejo** cada hora
4. **Sprites visuales** más detallados
5. **Efectos visuales** (partículas, animaciones)

---

## Notas de Desarrollo

- Código compilable y ejecutable en primera entrega
- Arquitectura escalable para agregar más elementos
- OpenMP integrado desde el inicio para paralelización futura
- API SFML 3.0 compatible con versiones futuras

---

**Fecha de Entrega:** 10 de Agosto, 2026  
**Estado:** ✅ Listo para evaluación
