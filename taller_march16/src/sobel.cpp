#include "filtros.h"
#include <cmath>

/*
 * Deteccion de bordes con el operador Sobel (resultado umbralizado).
 *
 * Calcula el gradiente en horizontal (Gx) y vertical (Gy) usando dos
 * kernels 3x3. La magnitud combina ambos con la norma L2.
 * Al final se aplica un umbral para obtener una imagen binaria de bordes.
 *
 * Ver sobel_magnitudes.cpp para el gradiente continuo sin umbral.
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

cv::Mat aplicar_sobel(const cv::Mat& src) {
    CV_Assert(src.type() == CV_8U);

    cv::Mat gx = convolucionar(src, SOBEL_X, 3);
    cv::Mat gy = convolucionar(src, SOBEL_Y, 3);

    cv::Mat magnitud(src.size(), CV_32F);
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            float vx = gx.at<float>(i, j);
            float vy = gy.at<float>(i, j);
            magnitud.at<float>(i, j) = std::sqrt(vx * vx + vy * vy);
        }
    }

    // Normalizar y aplicar umbral para obtener bordes binarios
    cv::Mat normalizada;
    cv::normalize(magnitud, normalizada, 0, 255, cv::NORM_MINMAX, CV_8U);

    cv::Mat dst;
    cv::threshold(normalizada, dst, 50, 255, cv::THRESH_BINARY);
    return dst;
}
