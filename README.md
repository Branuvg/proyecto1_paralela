# Marine Simulation - Proyecto 1

Simulación interactiva de un ecosistema marino con paralelización usando OpenMP.

## Características

- **Ventana gráfica**: SFML (Simple and Fast Multimedia Library)
- **Paralelización**: OpenMP para actualización de elementos y detección de colisiones
- **Elementos**:
  - Burbujas: suben hacia la superficie
  - Estrellas de mar: se mueven libremente
  - Tortugas: movimiento diagonal con rotación en colisiones
  - Cangrejo: aparece cada hora como sorpresa

- **Sistema de colores del cielo**: Varía según la hora del día
  - Claro (6 AM - 11 AM)
  - Rojizo (11 AM - 7 PM)
  - Azul oscuro/noche (7 PM - 5 AM)

- **Agua**: Ocupa 3/4 de la pantalla

## Compilación

### Requisitos
- CMake >= 3.10
- SFML 2.5
- OpenMP

### macOS
```bash
# Instalar SFML (si no está instalado)
brew install sfml

# Compilar
mkdir build
cd build
cmake ..
make
```

### Linux
```bash
# Ubuntu/Debian
sudo apt-get install libsfml-dev libomp-dev

mkdir build
cd build
cmake ..
make
```

## Uso

```bash
./marine_sim
```

### Controles
- **Click en cielo**: Agrega una estrella de mar
- **Click en agua**: Agrega una burbuja

## Estructura del proyecto

```
src/
├── main.cpp          # Punto de entrada
├── elements.h/cpp    # Definiciones de elementos
├── simulation.h/cpp  # Lógica de simulación con OpenMP
└── renderer.h/cpp    # Renderización con SFML
```

## Próximos pasos

- [ ] Animación de zoom y explosión de burbujas
- [ ] Mejora visual con sprites
- [ ] Paralelización distribuida
- [ ] Sistema de puntuación
