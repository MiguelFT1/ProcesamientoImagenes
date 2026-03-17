#pragma once

#include <opencv2/opencv.hpp>

// Transformacion logaritmica para realzar detalles en zonas oscuras
cv::Mat aplicar_log(const cv::Mat& src);

// Deteccion de bordes por cruces por cero (LoG: Laplaciano del Gaussiano)
cv::Mat aplicar_zero_crossing(const cv::Mat& src);

// Deteccion de bordes con el operador Scharr
cv::Mat aplicar_scharr(const cv::Mat& src);

// Deteccion de bordes con el Laplaciano
cv::Mat aplicar_laplacian(const cv::Mat& src);

// Deteccion de bordes con Sobel (resultado umbralizado)
cv::Mat aplicar_sobel(const cv::Mat& src);

// Magnitud del gradiente Sobel en escala de grises (sin umbral)
cv::Mat aplicar_sobel_magnitudes(const cv::Mat& src);

// Detector de bordes Canny implementado manualmente
cv::Mat aplicar_canny(const cv::Mat& src, double umbral_bajo, double umbral_alto);
