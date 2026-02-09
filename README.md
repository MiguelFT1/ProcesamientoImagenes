
# Procesamiento de Imagenes en C++

##Miguel FLechas
##Juan Hurtado

Proyecto universitario de procesamiento digital de imagenes utilizando **C++** y **OpenCV**. Se implementan conversiones de espacios de color de forma manual, recorriendo pixel por pixel sin usar funciones de conversion de OpenCV.

## Contenido del Proyecto

| Archivo | Descripcion |
|---------|-------------|
| `conversiones.cpp` | Conversiones manuales de color: Grises, YUV y HSV |
| `matriz.cpp` | Generacion de una imagen PPM desde una matriz de pixeles |
| `images.jpeg` | Imagen de prueba |

## Conversiones de Color (`conversiones.cpp`)

Se implementan **3 funciones propias** que recorren cada pixel de la imagen y aplican las formulas matematicas de conversion. OpenCV solo se usa para cargar y guardar las imagenes.

### Escala de Grises

Promedio simple de los tres canales:

```
Gray = (R + G + B) / 3
```

### Espacio YUV

Separa la luminancia (Y) de la crominancia (U, V):

```
Y =  0.299 * R + 0.587 * G + 0.114 * B
U = -0.169 * R - 0.331 * G + 0.500 * B + 128
V =  0.500 * R - 0.419 * G - 0.081 * B + 128
```

### Espacio HSV

Descompone el color en Hue (tono), Saturation (saturacion) y Value (brillo):

```
V = max(R, G, B)
S = (max - min) / max
H = basado en cual canal es el maximo, calculado en grados (0-360)
```

## Generador de Imagen PPM (`matriz.cpp`)

Crea una imagen en formato PPM (P3) a partir de una matriz tridimensional definida en codigo. Cada elemento `[fila][columna][canal]` representa un pixel con sus valores RGB.

## Requisitos

- **g++** (compilador de C++)
- **OpenCV 4** (`libopencv-dev`)

### Instalacion de OpenCV

```bash
# Ubuntu / Debian / Pop!_OS
sudo apt install libopencv-dev
```

## Compilacion y Ejecucion

### Conversiones de color

```bash
g++ conversiones.cpp -o conversiones $(pkg-config --cflags --libs opencv4)
./conversiones images.jpeg
```

Esto genera tres imagenes de salida:

- `resultado_gris.png` - Imagen en escala de grises
- `resultado_yuv.png` - Imagen en espacio YUV
- `resultado_hsv.png` - Imagen en espacio HSV

Ademas muestra las 4 imagenes (original + 3 conversiones) en ventanas interactivas.

### Generador PPM

```bash
g++ matriz.cpp -o matriz
./matriz
```

Genera el archivo `mi_imagen.ppm`.
