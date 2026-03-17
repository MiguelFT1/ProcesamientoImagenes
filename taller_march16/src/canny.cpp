#include "filtros.h"
#include <cmath>

/*
 * Detector de bordes Canny implementado manualmente.
 *
 * El algoritmo tiene 5 pasos:
 *  1. Suavizar con Gaussiana para eliminar ruido
 *  2. Calcular gradiente (magnitud y angulo) con Sobel
 *  3. Supresion de no-maximos: adelgazar bordes a 1 pixel de ancho
 *  4. Doble umbral: clasificar pixeles como fuertes, debiles o nada
 *  5. Histeresis: conservar pixeles debiles solo si estan junto a fuertes
 */

static const float GAUSS5[25] = {
     1,  4,  6,  4,  1,
     4, 16, 24, 16,  4,
     6, 24, 36, 24,  6,
     4, 16, 24, 16,  4,
     1,  4,  6,  4,  1
};

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

// Convolucion para entrada CV_32F
static cv::Mat convolucionar32f(const cv::Mat& src, const float* kernel, int ksize) {
    int radio = ksize / 2;
    cv::Mat dst = cv::Mat::zeros(src.size(), CV_32F);

    for (int i = radio; i < src.rows - radio; i++) {
        for (int j = radio; j < src.cols - radio; j++) {
            float suma = 0;
            for (int ki = 0; ki < ksize; ki++) {
                for (int kj = 0; kj < ksize; kj++) {
                    suma += src.at<float>(i + ki - radio, j + kj - radio)
                            * kernel[ki * ksize + kj];
                }
            }
            dst.at<float>(i, j) = suma;
        }
    }
    return dst;
}

cv::Mat aplicar_canny(const cv::Mat& src, double umbral_bajo, double umbral_alto) {
    CV_Assert(src.type() == CV_8U);

    // --- Paso 1: suavizar con Gaussiana ---
    float gauss_norm[25];
    for (int i = 0; i < 25; i++) gauss_norm[i] = GAUSS5[i] / 256.0f;

    cv::Mat src_f;
    src.convertTo(src_f, CV_32F);
    cv::Mat suavizada = convolucionar32f(src_f, gauss_norm, 5);

    // --- Paso 2: gradiente Sobel ---
    cv::Mat gx = convolucionar32f(suavizada, SOBEL_X, 3);
    cv::Mat gy = convolucionar32f(suavizada, SOBEL_Y, 3);

    cv::Mat magnitud(src.size(), CV_32F);
    cv::Mat angulo(src.size(), CV_32F);

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            float vx = gx.at<float>(i, j);
            float vy = gy.at<float>(i, j);
            magnitud.at<float>(i, j) = std::sqrt(vx * vx + vy * vy);
            // atan2 da el angulo en radianes; convertir a grados
            angulo.at<float>(i, j) = std::atan2(vy, vx) * 180.0f / (float)M_PI;
        }
    }

    // --- Paso 3: supresion de no-maximos ---
    // Para cada pixel, revisar si es el maximo en la direccion de su gradiente.
    // Si no lo es, eliminarlo. Esto adelgaza los bordes a 1 pixel.
    cv::Mat suprimida = cv::Mat::zeros(src.size(), CV_32F);

    for (int i = 1; i < src.rows - 1; i++) {
        for (int j = 1; j < src.cols - 1; j++) {
            float ang = angulo.at<float>(i, j);
            float mag = magnitud.at<float>(i, j);

            // Llevar el angulo al rango [0, 180)
            if (ang < 0) ang += 180.0f;

            float v1, v2;

            if (ang < 22.5f || ang >= 157.5f) {
                // Borde horizontal -> comparar con vecinos izquierdo y derecho
                v1 = magnitud.at<float>(i, j - 1);
                v2 = magnitud.at<float>(i, j + 1);
            } else if (ang < 67.5f) {
                // Diagonal (/) -> comparar con vecinos en esa diagonal
                v1 = magnitud.at<float>(i - 1, j + 1);
                v2 = magnitud.at<float>(i + 1, j - 1);
            } else if (ang < 112.5f) {
                // Borde vertical -> comparar con vecinos arriba y abajo
                v1 = magnitud.at<float>(i - 1, j);
                v2 = magnitud.at<float>(i + 1, j);
            } else {
                // Diagonal (\) -> comparar con esa otra diagonal
                v1 = magnitud.at<float>(i - 1, j - 1);
                v2 = magnitud.at<float>(i + 1, j + 1);
            }

            if (mag >= v1 && mag >= v2) {
                suprimida.at<float>(i, j) = mag;
            }
        }
    }

    // --- Paso 4: doble umbral ---
    // Fuerte: seguro que es borde.  Debil: puede ser borde.  0: descartado.
    const uchar FUERTE = 255;
    const uchar DEBIL  = 128;

    cv::Mat bordes(src.size(), CV_8U, cv::Scalar(0));

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            float mag = suprimida.at<float>(i, j);
            if      (mag >= umbral_alto) bordes.at<uchar>(i, j) = FUERTE;
            else if (mag >= umbral_bajo) bordes.at<uchar>(i, j) = DEBIL;
        }
    }

    // --- Paso 5: histeresis ---
    // Un pixel debil se convierte en borde solo si tiene algun vecino fuerte.
    // De lo contrario se descarta (se considera ruido).
    for (int i = 1; i < src.rows - 1; i++) {
        for (int j = 1; j < src.cols - 1; j++) {
            if (bordes.at<uchar>(i, j) != DEBIL) continue;

            bool vecino_fuerte = false;
            for (int di = -1; di <= 1 && !vecino_fuerte; di++) {
                for (int dj = -1; dj <= 1 && !vecino_fuerte; dj++) {
                    if (bordes.at<uchar>(i + di, j + dj) == FUERTE)
                        vecino_fuerte = true;
                }
            }

            bordes.at<uchar>(i, j) = vecino_fuerte ? FUERTE : 0;
        }
    }

    return bordes;
}
