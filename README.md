# Screensaver Acuático — Proyecto 1

Screensaver de un ecosistema acuático escrito en C++ con SFML 3, paralelizado con OpenMP.
Proyecto 1 del curso de Computación Paralela y Distribuida, UVG, Semestre 2 2026.

La descripción de los elementos y su comportamiento está en [`docs/propuesta.png`](docs/propuesta.png).

## Estructura

```
src/
├── main.cpp        Punto de entrada: argumentos, ventana y ciclo principal
├── config.h/.cpp   Parámetros de línea de comandos y su validación
├── elements.h      Tipos de elementos: burbujas, estrellas de mar y tortugas
├── simulation.*    Estado de la escena y física  (zona paralelizable)
├── renderer.*      Construcción de geometría y dibujo con SFML
└── metrics.*       Medición de FPS y de tiempos por sección
docs/
├── propuesta.png             Propuesta del screensaver
```

La regla que separa `simulation` de `renderer`: **`simulation` calcula dónde están las cosas,
`renderer` decide cómo se ven.** Si se borrara `renderer`, la simulación debería seguir
corriendo correctamente. Esa frontera importa porque `simulation` corre en varios hilos y
la entrega final al GPU no puede.

## Versión secuencial y paralela

Ambos ejecutables se compilan **de las mismas fuentes**. La única diferencia es que
`screensaver_par` enlaza OpenMP y `screensaver_seq` no; sin OpenMP el compilador ignora
las directivas `#pragma omp` y el resultado es el mismo algoritmo en un solo hilo.

| Ejecutable        | OpenMP | Uso                                   |
| ----------------- | ------ | ------------------------------------- |
| `screensaver_seq` | no     | línea base para el cálculo de speedup |
| `screensaver_par` | sí     | versión paralela, acepta `--threads`  |

## Requisitos

- CMake ≥ 3.16
- Compilador con C++17
- SFML 3
- OpenMP

## Compilación

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### Instalar dependencias

```bash
# macOS
brew install cmake sfml libomp

# Ubuntu / Debian
sudo apt-get install build-essential cmake libsfml-dev libomp-dev

# Windows (vcpkg)
vcpkg install sfml:x64-windows
cmake .. -DCMAKE_TOOLCHAIN_FILE=<ruta-vcpkg>/scripts/buildsystems/vcpkg.cmake
```

Si CMake no encuentra SFML, se le puede pasar la ruta a mano:

```bash
cmake .. -DSFML_DIR=$(brew --prefix sfml)/lib/cmake/SFML     # macOS
cmake .. -DSFML_DIR=C:/ruta/a/SFML/lib/cmake/SFML            # Windows
```

## Uso

```bash
./screensaver_par --n 5000 --width 800 --height 600 --threads 4
```

| Argumento            | Descripción                                          | Default                   |
| -------------------- | ---------------------------------------------------- | ------------------------- |
| `--n <entero>`       | Cantidad de elementos a renderizar                   | 300                       |
| `--width <entero>`   | Ancho de la ventana (mínimo 640)                     | 1200                      |
| `--height <entero>`  | Alto de la ventana (mínimo 480)                      | 800                       |
| `--threads <entero>` | Hilos de OpenMP                                      | los que escoja el sistema |
| `--seed <entero>`    | Semilla del generador aleatorio                      | el reloj                  |
| `--frames <entero>`  | Corre esa cantidad de frames, imprime tiempos y sale | 0 (sin límite)            |
| `--help`, `-h`       | Muestra la ayuda                                     |                           |

`Esc` o cerrar la ventana termina el programa.

## Medición

`--frames` corre una cantidad fija de frames sin limitar los FPS y al terminar imprime el
tiempo promedio de frame, de `update` y de `render`. Ese es el modo que se usa para las
mediciones del informe, porque con el límite de 60 FPS activo el tiempo lo domina la
espera del monitor y no el cómputo.

```bash
./screensaver_seq --n 20000 --frames 600 --seed 42
./screensaver_par --n 20000 --frames 600 --seed 42 --threads 8
```

Usar la misma `--seed` en ambas corridas garantiza que las dos versiones simulen
exactamente la misma escena y que la comparación sea válida.

## Estado

- [x] Ventana gráfica y ciclo principal
- [x] Parámetros por línea de comandos con validación
- [x] Medición de FPS y de tiempos por sección
- [ ] Elementos según la propuesta (cielo cíclico, agua que sube, efecto lupa)
- [ ] HUD de FPS en pantalla
- [ ] Dibujo por lotes con `sf::VertexArray`
- [ ] Paralelización con OpenMP y medición de speedup
