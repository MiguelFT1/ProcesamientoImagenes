#include "filtros.h"
#include <cmath>

/*
 * Filtro Laplaciano: segunda derivada de la imagen.
 *
 * Detecta bordes en todas las direcciones al mismo tiempo porque no
 * depende de la orientacion como el Sobel. El resultado puede ser negativo
 * asi que se toma el valor absoluto antes de mostrar.
 * Es bastante sensible al ruido; en practica conviene suavizar antes.
 */

static const float KERNEL_LAPLACIAN[9] = {
     0,  1,  0,
     1, -4,  1,
     0,  1,  0
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

cv::Mat aplicar_laplacian(const cv::Mat& src) {
    CV_Assert(src.type() == CV_8U);

    cv::Mat resultado = convolucionar(src, KERNEL_LAPLACIAN, 3);

    // El Laplaciano produce valores negativos en un lado del borde y
    // positivos en el otro; con el valor absoluto vemos ambos lados
    cv::Mat absoluto(src.size(), CV_32F);
    for (int i = 0; i < resultado.rows; i++) {
        for (int j = 0; j < resultado.cols; j++) {
            absoluto.at<float>(i, j) = std::abs(resultado.at<float>(i, j));
        }
    }

    cv::Mat dst;
    cv::normalize(absoluto, dst, 0, 255, cv::NORM_MINMAX, CV_8U);
    return dst;
}
