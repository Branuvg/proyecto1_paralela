# Screensaver Acuático — Proyecto 1

Screensaver de un ecosistema acuático en C++ con SFML 3, paralelizado con OpenMP.
Curso de Computación Paralela y Distribuida, UVG, Semestre 2 2026.

El comportamiento de cada elemento está descrito en [`docs/propuesta.png`](docs/propuesta.png).

## 1. Instalar dependencias

Se necesitan CMake, un compilador con C++17, SFML 3 y OpenMP.

**Windows** (con [vcpkg](https://vcpkg.io)):

```powershell
vcpkg install sfml:x64-windows
```

**macOS:**

```bash
brew install cmake sfml libomp
```

**Ubuntu / Debian:**

```bash
sudo apt-get install build-essential cmake libsfml-dev libomp-dev
```

## 2. Compilar

**Windows** — hay que pasarle a CMake dónde está vcpkg (cambiá la ruta por la tuya):

```powershell
cmake -B build -DCMAKE_TOOLCHAIN_FILE=C:/Users/TU_USUARIO/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

**macOS / Linux:**

```bash
cmake -B build
cmake --build build
```

Se generan **dos ejecutables**: `screensaver_seq` (sin OpenMP, es la línea base) y
`screensaver_par` (con OpenMP). Salen de las mismas fuentes; la única diferencia es que
uno enlaza OpenMP y el otro no.

## 3. Correr

**Windows:**

```powershell
.\build\Release\screensaver_seq.exe --n 2000
.\build\Release\screensaver_par.exe --n 2000 --threads 4
```

**macOS / Linux:**

```bash
./build/screensaver_seq --n 2000
./build/screensaver_par --n 2000 --threads 4
```

`Esc` o cerrar la ventana termina el programa.

## 4. Opciones

```powershell
.\build\Release\screensaver_seq.exe --help
```

| Argumento            | Qué hace                                             | Default                   |
| -------------------- | ---------------------------------------------------- | ------------------------- |
| `--n <entero>`       | Cantidad de elementos a renderizar                   | 300                       |
| `--width <entero>`   | Ancho de la ventana (mínimo 640)                     | 1200                      |
| `--height <entero>`  | Alto de la ventana (mínimo 480)                      | 800                       |
| `--threads <entero>` | Hilos de OpenMP (solo aplica a `screensaver_par`)    | los que escoja el sistema |
| `--seed <entero>`    | Semilla del generador aleatorio                      | el reloj                  |
| `--frames <entero>`  | Corre esa cantidad de frames, imprime tiempos y sale | 0 (sin límite)            |
| `--help`, `-h`       | Muestra la ayuda                                     |                           |

## 5. Medir para el informe

`--frames` corre una cantidad fija de frames **sin limitar los FPS** y al terminar imprime
el tiempo promedio de frame, de `update` y de `render`. Sin esa bandera el programa corre
a 60 FPS y el tiempo lo domina la espera del monitor, no el cómputo.

```powershell
.\build\Release\screensaver_seq.exe --n 20000 --frames 600 --seed 42
.\build\Release\screensaver_par.exe --n 20000 --frames 600 --seed 42 --threads 8
```

Usar la **misma `--seed`** en las dos corridas hace que ambas simulen exactamente la misma
escena, que es lo que vuelve válida la comparación.

## 6. Estructura

```
src/
├── main.cpp        Argumentos, ventana y ciclo principal
├── config.*        Parámetros de línea de comandos y su validación
├── elements.*      Burbujas, estrellas de mar y tortugas, con su actualización
├── simulation.*    Estado de la escena y física (zona paralelizable)
├── renderer.*      Geometría y dibujo con SFML
└── metrics.*       Medición de FPS y tiempos por sección
docs/
├── propuesta.png   Propuesta del screensaver
```

`simulation` calcula **dónde** están las cosas; `renderer` decide **cómo** se ven. Esa
frontera importa porque `simulation` va a correr en varios hilos y la entrega al GPU no
puede.

## 7. Estado

- [x] Ventana gráfica, ciclo principal y HUD de FPS
- [x] Parámetros por línea de comandos con validación
- [x] Elementos según la propuesta (cielo cíclico, agua que sube, efecto lupa)
- [x] Colores pseudoaleatorios y dibujo por lotes con `sf::VertexArray`
- [x] Medición de FPS y de tiempos por sección
- [ ] Paralelización con OpenMP y cálculo de speedup
