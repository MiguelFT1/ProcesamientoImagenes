#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace cv;

// Convierte un pixel BGR a escala de grises: Gray = (R + G + B) / 3
void bgrToGray(const Mat& src, Mat& dst) {
    dst = Mat(src.rows, src.cols, CV_8UC1);

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            Vec3b pixel = src.at<Vec3b>(i, j);
            uchar b = pixel[0], g = pixel[1], r = pixel[2];
            dst.at<uchar>(i, j) = (uchar)((r + g + b) / 3);
        }
    }
}

// Convierte un pixel BGR a YUV
// Y =  0.299*R + 0.587*G + 0.114*B
// U = -0.169*R - 0.331*G + 0.500*B + 128
// V =  0.500*R - 0.419*G - 0.081*B + 128
void bgrToYUV(const Mat& src, Mat& dst) {
    dst = Mat(src.rows, src.cols, CV_8UC3);

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            Vec3b pixel = src.at<Vec3b>(i, j);
            double b = pixel[0], g = pixel[1], r = pixel[2];

            double y = 0.299 * r + 0.587 * g + 0.114 * b;
            double u = -0.169 * r - 0.331 * g + 0.500 * b + 128.0;
            double v = 0.500 * r - 0.419 * g - 0.081 * b + 128.0;

            dst.at<Vec3b>(i, j) = Vec3b(
                (uchar)max(0.0, min(255.0, y)),
                (uchar)max(0.0, min(255.0, u)),
                (uchar)max(0.0, min(255.0, v))
            );
        }
    }
}

// Convierte un pixel BGR a HSV
// H: 0-180 (OpenCV usa mitad de grados para caber en uchar)
// S: 0-255
// V: 0-255
void bgrToHSV(const Mat& src, Mat& dst) {
    dst = Mat(src.rows, src.cols, CV_8UC3);

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            Vec3b pixel = src.at<Vec3b>(i, j);
            double b = pixel[0] / 255.0;
            double g = pixel[1] / 255.0;
            double r = pixel[2] / 255.0;

            double cmax = max({r, g, b});
            double cmin = min({r, g, b});
            double diff = cmax - cmin;

            double h = 0, s = 0, v = cmax;

            // Saturacion
            if (cmax > 0)
                s = diff / cmax;

            // Hue
            if (diff > 0) {
                if (cmax == r)
                    h = 60.0 * fmod((g - b) / diff, 6.0);
                else if (cmax == g)
                    h = 60.0 * ((b - r) / diff + 2.0);
                else
                    h = 60.0 * ((r - g) / diff + 4.0);
            }

            if (h < 0) h += 360.0;

            dst.at<Vec3b>(i, j) = Vec3b(
                (uchar)(h / 2.0),       // H: 0-180
                (uchar)(s * 255.0),     // S: 0-255
                (uchar)(v * 255.0)      // V: 0-255
            );
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Uso: ./conversiones <ruta_imagen>" << endl;
        return 1;
    }

    Mat imagen = imread(argv[1]);
    if (imagen.empty()) {
        cout << "Error: no se pudo abrir la imagen " << argv[1] << endl;
        return 1;
    }

    Mat gris, yuv, hsv;
    bgrToGray(imagen, gris);
    bgrToYUV(imagen, yuv);
    bgrToHSV(imagen, hsv);

    imwrite("resultado_gris.png", gris);
    imwrite("resultado_yuv.png", yuv);
    imwrite("resultado_hsv.png", hsv);

    // Mostrar las imagenes
    imshow("Original", imagen);
    imshow("Gris (R+G+B)/3", gris);
    imshow("YUV", yuv);
    imshow("HSV", hsv);

    cout << "Presiona cualquier tecla para cerrar..." << endl;
    waitKey(0);
    destroyAllWindows();

    return 0;
}
