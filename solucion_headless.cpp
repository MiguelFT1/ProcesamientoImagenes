#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

// ============================================================
// Construccion manual del Kernel Gaussiano 2D
// ============================================================
Mat crearKernelGaussiano(int tamano, double sigma) {
    Mat kernel(tamano, tamano, CV_64F);
    int centro = tamano / 2;
    double suma = 0.0;

    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            int x = i - centro;
            int y = j - centro;
            double valor = (1.0 / (2 * CV_PI * sigma * sigma)) *
                           exp(-(x * x + y * y) / (2 * sigma * sigma));
            kernel.at<double>(i, j) = valor;
            suma += valor;
        }
    }

    kernel = kernel / suma;
    return kernel;
}

int main() {
    // ============================================================
    // 1. Representacion Digital e Histograma
    // ============================================================
    Mat img = imread("S1A_IW_GRDH_1SDV_20260221T053027_20260221T053052_063313_07F369_8E56.jpg", IMREAD_GRAYSCALE);
    if (img.empty()) {
        cout << "Error cargando imagen" << endl;
        return -1;
    }

    imwrite("original.png", img);

    cout << "=== Representacion Digital ===" << endl;
    cout << "Filas: " << img.rows << endl;
    cout << "Columnas: " << img.cols << endl;
    cout << "Canales: " << img.channels() << endl;
    cout << "Profundidad: " << img.depth() << endl;
    cout << endl;

    // ============================================================
    // 2. Modelado Gaussiano de Pixeles - Media y Desviacion
    // ============================================================
    Scalar media, desviacion;
    meanStdDev(img, media, desviacion);

    cout << "=== Modelado Gaussiano ===" << endl;
    cout << "Media: " << media[0] << endl;
    cout << "Desviacion: " << desviacion[0] << endl;
    cout << endl;

    // ============================================================
    // 3. Segmentacion Gaussiana (mu +/- 2*sigma)
    // ============================================================
    Mat mascara = Mat::zeros(img.size(), CV_8U);

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            uchar pixel = img.at<uchar>(i, j);
            if (pixel > media[0] - 2 * desviacion[0] &&
                pixel < media[0] + 2 * desviacion[0]) {
                mascara.at<uchar>(i, j) = 255;
            }
        }
    }

    imwrite("segmentacion.png", mascara);

    // ============================================================
    // 4. Filtro Gaussiano (Suavizado - Lowpass)
    // ============================================================
    Mat suavizada;
    GaussianBlur(img, suavizada, Size(7, 7), 1.5);

    imwrite("suavizada.png", suavizada);

    // Comparar desviacion antes y despues del filtro
    Scalar media2, desv2;
    meanStdDev(suavizada, media2, desv2);

    cout << "=== Comparacion tras Filtro Gaussiano ===" << endl;
    cout << "Desviacion original:  " << desviacion[0] << endl;
    cout << "Desviacion suavizada: " << desv2[0] << endl;
    cout << "Reduccion de varianza: "
         << (1.0 - (desv2[0] * desv2[0]) / (desviacion[0] * desviacion[0])) * 100
         << "%" << endl;
    cout << endl;

    // ============================================================
    // 5. Filtro Laplaciano (Afilado - Highpass)
    // ============================================================
    Mat laplaciano;
    Laplacian(img, laplaciano, CV_16S, 3);
    convertScaleAbs(laplaciano, laplaciano);

    imwrite("laplaciano.png", laplaciano);

    // Imagen realzada (original + bordes)
    Mat sharpened;
    add(img, laplaciano, sharpened);

    imwrite("afilada.png", sharpened);

    // ============================================================
    // 6. Pipeline Completo: Gauss -> Laplaciano -> Realce
    // ============================================================
    Mat gauss;
    GaussianBlur(img, gauss, Size(7, 7), 1.5);

    Mat lap;
    Laplacian(gauss, lap, CV_16S, 3);
    convertScaleAbs(lap, lap);

    Mat final_img;
    add(gauss, lap, final_img);

    imwrite("pipeline.png", final_img);

    // ============================================================
    // 7. Kernel Gaussiano Manual 5x5
    // ============================================================
    Mat kernel = crearKernelGaussiano(5, 1.0);
    cout << "=== Kernel Gaussiano 5x5 (sigma=1.0) ===" << endl;
    cout << kernel << endl;

    // Verificar suma
    double suma_kernel = sum(kernel)[0];
    cout << "Suma del kernel: " << suma_kernel << endl;
    cout << "Valor central:  " << kernel.at<double>(2, 2) << endl;
    cout << endl;

    // Kernels con diferentes sigma para comparacion
    Mat kernel_05 = crearKernelGaussiano(5, 0.5);
    Mat kernel_30 = crearKernelGaussiano(5, 3.0);
    cout << "=== Kernel sigma=0.5 ===" << endl;
    cout << kernel_05 << endl;
    cout << "=== Kernel sigma=3.0 ===" << endl;
    cout << kernel_30 << endl;

    cout << "\nPresione cualquier tecla para cerrar..." << endl;
    // waitKey(0);
    // destroyAllWindows();

    return 0;
}
