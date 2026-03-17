# Taller - Filtros de Procesamiento de Imágenes
**Universidad:** Procesamiento de Imágenes
**Integrantes:** Miguel Flechas, Juan Hurtado, Andrés Castro
**Fecha:** Marzo 16, 2026

---

## ¿Qué hace el programa?

Abre la cámara del computador y aplica filtros de procesamiento de imágenes en tiempo real. Cada filtro está implementado manualmente (sin usar las funciones de filtrado de OpenCV), solo se usa OpenCV para capturar el video y mostrar las ventanas.

Se puede navegar entre filtros con el teclado mientras la cámara está corriendo.

---

## Filtros implementados

| Tecla | Filtro |
|-------|--------|
| `0` | Original (escala de grises) |
| `1` | Transformación logarítmica |
| `2` | Zero Crossing (cruces por cero) |
| `3` | Bordes Scharr |
| `4` | Bordes Laplaciano |
| `5` | Bordes Sobel |
| `6` | Magnitudes Sobel |
| `7` | Canny |

- `n` → siguiente filtro
- `p` → filtro anterior
- `q` → salir

---

## Requisitos

- CMake >= 3.10
- OpenCV >= 4.x
- g++ con soporte C++14
- Cámara web

En Ubuntu/Debian instalar dependencias:
```bash
sudo apt install cmake g++ libopencv-dev
```

---

## Compilar y correr

```bash
git clone https://github.com/MiguelFT1/ProcesamientoImagenes.git
cd ProcesamientoImagenes
git checkout taller-march16
cd taller_march16
mkdir build && cd build
cmake ..
make
./taller_filtros
```

---

## Estructura del proyecto

```
taller_march16/
├── CMakeLists.txt
├── include/
│   └── filtros.h          # declaraciones de todos los filtros
├── src/
│   ├── main.cpp            # cámara y loop principal
│   ├── log_filter.cpp      # transformación logarítmica
│   ├── zero_crossing.cpp   # detección de cruces por cero
│   ├── scharr.cpp          # operador Scharr
│   ├── laplacian.cpp       # operador Laplaciano
│   ├── sobel.cpp           # detección de bordes Sobel
│   ├── sobel_magnitudes.cpp# magnitud del gradiente Sobel
│   └── canny.cpp           # algoritmo Canny completo
└── build/                  # generado por cmake (no se sube al repo)
```

---

## Notas

- Si la cámara no abre, cambiar el índice `0` por `1` en `main.cpp` línea 10.
- Los filtros trabajan en escala de grises ya que los operadores de bordes operan sobre intensidad, no sobre color.
- El Canny incluye las 5 etapas: suavizado gaussiano, gradientes Sobel, supresión de no-máximos, doble umbral e histéresis.
