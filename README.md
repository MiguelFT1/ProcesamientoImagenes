# Taller de Procesamiento de Imágenes: Filtrado Gaussiano y Laplaciano en Imágenes SAR

Este repositorio contiene la solución al taller de procesamiento de imágenes, enfocado en la aplicación de filtros espaciales (Gaussiano y Laplaciano) sobre una imagen satelital SAR (Sentinel-1) utilizando C++ y la biblioteca OpenCV.

## 👥 Integrantes del Equipo

*   **Juan Hurtado**
*   **Miguel Flechas**
*   **Andres Castro**

## 📝 Descripción del Proyecto

El objetivo de este taller es procesar una imagen satelital proveniente del sensor SAR (Radar de Apertura Sintética) Sentinel-1. Dado que las imágenes de radar suelen presentar ruido (speckle), se implementó un pipeline de procesamiento digital para suavizar la imagen, extraer características y realzar los bordes.

El procesamiento incluye los siguientes pasos:
1.  **Representación Digital e Histograma**: Carga de la imagen en escala de grises y análisis inicial.
2.  **Modelado Gaussiano de Píxeles**: Cálculo de la media y la desviación estándar de la imagen original.
3.  **Segmentación Gaussiana**: Binarización de la imagen aislando los píxeles que se encuentran dentro del rango dinámico significativo ($\mu \pm 2\sigma$).
4.  **Filtro Gaussiano (Suavizado)**: Aplicación de un filtro pasa-bajas para atenuar el ruido y observar la reducción de la varianza.
5.  **Filtro Laplaciano (Afilado)**: Aplicación de un filtro pasa-altas para detectar cruces por cero y obtener los bordes. Posteriormente, se suma a la imagen original o suavizada para el realce.
6.  **Pipeline Completo**: Integración secuencial donde se aplica primero el filtro Gaussiano y luego el Laplaciano para un resultado óptimo.
7.  **Construcción Manual del Kernel Gaussian 2D**: Implementación desde cero de un generador de kernels Gaussianos para comprobar el funcionamiento matemático subyacente de la función de OpenCV y visualizar el efecto de diferentes valores de $\sigma$.

## 🚀 Requisitos Previos

Para poder compilar y ejecutar este código, necesitas tener instalado en tu sistema:
- Compilador de C++ (ej. `g++`) con soporte para C++17.
- **OpenCV** (versión 4 recomendada).
- Herramienta `make` (opcional, pero recomendada para facilitar la compilación).

En sistemas basados en Debian/Ubuntu, puedes instalar OpenCV con:
```bash
sudo apt-get update
sudo apt-get install libopencv-dev
```

## 🛠️ Cómo Compilar y Ejecutar

El proyecto incluye dos versiones del código:
*   `solucion.cpp`: Muestra las imágenes en ventanas emergentes paso a paso (recomendado para entornos gráficos).
*   `solucion_headless.cpp`: Genera y guarda los resultados como imágenes en el disco sin abrir ventanas (ideal para entornos de consola o servidores).

### Opción 1: Usando el Makefile (Entorno Gráfico - solucion.cpp)

Hemos incluido un `Makefile` para hacer la compilación muy sencilla.

1. Abre tu terminal en el directorio del proyecto.
2. Construye el ejecutable utilizando el comando `make`:
   ```bash
   make
   ```
3. Ejecuta el programa:
   ```bash
   make run
   ```
   *(También puedes ejecutarlo directamente con `./solucion`)*
4. Presiona cualquier tecla mientras estás en las ventanas de las imágenes para cerrarlas al finalizar.
5. Para limpiar los archivos generados por la compilación:
   ```bash
   make clean
   ```

### Opción 2: Compilación manual (Entorno sin interfaz - solucion_headless.cpp)

Si deseas utilizar la versión que guarda los archivos (headless) sin abrir la interfaz gráfica:

1. Compila el código enlazando la librería de OpenCV:
   ```bash
   g++ -std=c++17 -Wall -o solucion_headless solucion_headless.cpp $(pkg-config --cflags --libs opencv4)
   ```
2. Ejecuta el archivo resultante:
   ```bash
   ./solucion_headless
   ```
3. En la consola verás la salida detallada de las estadísticas y los kernels manuales. En el directorio se generarán las siguientes imágenes de resultado:
   * `original.png`
   * `segmentacion.png`
   * `suavizada.png`
   * `laplaciano.png`
   * `afilada.png`
   * `pipeline.png`

## 📁 Archivos Importantes

*   `solucion.cpp` / `solucion_headless.cpp`: Código fuente principal con la lógica de procesamiento.
*   `S1A_IW_GRDH_1SDV_20260221T053027_20260221T053052_063313_07F369_8E56.jpg`: Imagen satelital original de Sentinel-1 que sirve como entrada estática para nuestro programa.
*   `Makefile`: Receta automatizada para compilar el proyecto.
*   `TALLER.md.pdf`: Documento guía original con las especificaciones del taller.
