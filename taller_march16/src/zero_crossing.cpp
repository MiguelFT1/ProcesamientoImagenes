#include "filtros.h"
#include <cmath>

/*
 * Deteccion de bordes por cruces por cero (Zero Crossing).
 *
 * El proceso es: suavizar con Gaussiana -> aplicar Laplaciano -> marcar
 * donde la imagen pasa de positivo a negativo (o al reves).
 * Esos cambios de signo corresponden a la ubicacion exacta de un borde.
 * Suavizar primero reduce el ruido que el Laplaciano amplificaria.
 */

// Kernel Gaussiano 5x5 (sin normalizar, suma = 256)
static const float GAUSS5[25] = {
     1,  4,  6,  4,  1,
     4, 16, 24, 16,  4,
     6, 24, 36, 24,  6,
     4, 16, 24, 16,  4,
     1,  4,  6,  4,  1
};

// Laplaciano 3x3: segunda derivada, sensible a todos los bordes
static const float LAPLACIAN3[9] = {
     0,  1,  0,
     1, -4,  1,
     0,  1,  0
};

// Convolucion generica para imagen CV_8U, devuelve CV_32F
static cv::Mat convolucionar(const cv::Mat& src, const float* kernel, int ksize) {
    int radio = ksize / 2;
    cv::Mat dst = cv::Mat::zeros(src.size(), CV_32F);

    for (int i = radio; i < src.rows - radio; i++) {
        for (int j = radio; j < src.cols - radio; j++) {
            float suma = 0;
            for (int ki = 0; ki < ksize; ki++) {
                for (int kj = 0; kj < ksize; kj++) {
                    suma += (float)src.at<uchar>(i + ki - radio, j + kj - radio)
                            * kernel[ki * ksize + kj];
                }
            }
            dst.at<float>(i, j) = suma;
        }
    }
    return dst;
}

cv::Mat aplicar_zero_crossing(const cv::Mat& src) {
    CV_Assert(src.type() == CV_8U);

    // Normalizar el Gaussiano para que la convolucion no escale los valores
    float gauss_norm[25];
    for (int i = 0; i < 25; i++) gauss_norm[i] = GAUSS5[i] / 256.0f;

    // Paso 1: suavizar
    cv::Mat suavizada_f = convolucionar(src, gauss_norm, 5);
    cv::Mat suavizada;
    suavizada_f.convertTo(suavizada, CV_8U);

    // Paso 2: Laplaciano sobre la imagen suavizada
    cv::Mat log_img = convolucionar(suavizada, LAPLACIAN3, 3);

    // Paso 3: marcar donde hay cambio de signo entre pixel y su vecino
    cv::Mat dst = cv::Mat::zeros(src.size(), CV_8U);

    for (int i = 1; i < log_img.rows - 1; i++) {
        for (int j = 1; j < log_img.cols - 1; j++) {
            float c = log_img.at<float>(i, j);

            bool cruce =
                (c > 0 && log_img.at<float>(i, j + 1) < 0) ||
                (c < 0 && log_img.at<float>(i, j + 1) > 0) ||
                (c > 0 && log_img.at<float>(i + 1, j) < 0) ||
                (c < 0 && log_img.at<float>(i + 1, j) > 0);

            if (cruce) dst.at<uchar>(i, j) = 255;
        }
    }

    return dst;
}
