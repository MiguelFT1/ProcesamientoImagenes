#include "filtros.h"
#include <cmath>

/*
 * Magnitud del gradiente Sobel (imagen continua, sin umbral).
 *
 * A diferencia de sobel.cpp que binariza el resultado, aqui se muestra
 * la "intensidad" del borde en cada pixel: blanco = borde fuerte,
 * gris = borde debil, negro = zona plana.
 * Sirve para analizar cuanto cambia la imagen en cada punto.
 */

static const float SOBEL_X[9] = {
    -1, 0, 1,
    -2, 0, 2,
    -1, 0, 1
};

static const float SOBEL_Y[9] = {
    -1, -2, -1,
     0,  0,  0,
     1,  2,  1
};

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

cv::Mat aplicar_sobel_magnitudes(const cv::Mat& src) {
    CV_Assert(src.type() == CV_8U);

    cv::Mat gx = convolucionar(src, SOBEL_X, 3);
    cv::Mat gy = convolucionar(src, SOBEL_Y, 3);

    cv::Mat magnitud(src.size(), CV_32F);
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            float vx = gx.at<float>(i, j);
            float vy = gy.at<float>(i, j);
            // sqrt(Gx^2 + Gy^2): norma euclidiana del gradiente
            magnitud.at<float>(i, j) = std::sqrt(vx * vx + vy * vy);
        }
    }

    // Solo normalizar, sin umbral, para ver la magnitud real
    cv::Mat dst;
    cv::normalize(magnitud, dst, 0, 255, cv::NORM_MINMAX, CV_8U);
    return dst;
}
