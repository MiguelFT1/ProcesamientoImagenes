#include "filtros.h"
#include <cmath>

/*
 * Operador Scharr: variante mejorada del Sobel.
 *
 * Usa pesos de 3 y 10 (en lugar de 1 y 2 del Sobel) lo que le da
 * mejor precision rotacional: detecta bordes inclinados con menos error.
 * Se calcula el gradiente en X y en Y, luego se combina con la norma L2.
 */

static const float SCHARR_X[9] = {
    -3,  0,  3,
   -10,  0, 10,
    -3,  0,  3
};

static const float SCHARR_Y[9] = {
    -3, -10, -3,
     0,   0,  0,
     3,  10,  3
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

cv::Mat aplicar_scharr(const cv::Mat& src) {
    CV_Assert(src.type() == CV_8U);

    cv::Mat gx = convolucionar(src, SCHARR_X, 3);
    cv::Mat gy = convolucionar(src, SCHARR_Y, 3);

    cv::Mat magnitud(src.size(), CV_32F);

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            float vx = gx.at<float>(i, j);
            float vy = gy.at<float>(i, j);
            magnitud.at<float>(i, j) = std::sqrt(vx * vx + vy * vy);
        }
    }

    // Normalizar al rango [0, 255] para poder mostrar la imagen
    cv::Mat dst;
    cv::normalize(magnitud, dst, 0, 255, cv::NORM_MINMAX, CV_8U);
    return dst;
}
