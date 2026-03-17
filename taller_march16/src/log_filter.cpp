#include "filtros.h"
#include <cmath>

/*
 * Transformacion logaritmica: s = c * log(1 + r)
 *
 * Comprime el rango dinamico: los valores bajos (zonas oscuras) se elevan
 * mas que los valores altos, lo que saca detalles que estaban aplastados.
 * El coeficiente c normaliza el resultado para que quede en [0, 255].
 */
cv::Mat aplicar_log(const cv::Mat& src) {
    CV_Assert(src.type() == CV_8U);

    cv::Mat dst(src.size(), CV_8U);

    // c hace que el maximo posible (log(256)) mapee exactamente a 255
    double c = 255.0 / std::log(1.0 + 255.0);

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            double val = c * std::log(1.0 + src.at<uchar>(i, j));
            dst.at<uchar>(i, j) = cv::saturate_cast<uchar>(val);
        }
    }

    return dst;
}
