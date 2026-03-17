#include "filtros.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    cv::VideoCapture camara(0);

    if (!camara.isOpened()) {
        std::cerr << "No se pudo abrir la camara" << std::endl;
        return -1;
    }

    std::vector<std::string> nombres = {
        "Original",
        "Log",
        "Zero Crossing",
        "Scharr",
        "Laplaciano",
        "Sobel",
        "Sobel Magnitudes",
        "Canny"
    };

    int filtro_actual = 0;
    int total = (int)nombres.size();

    std::cout << "Controles: [n] siguiente | [p] anterior | [q] salir" << std::endl;
    std::cout << "Filtro inicial: " << nombres[filtro_actual] << std::endl;

    cv::Mat frame, gris, resultado;

    while (true) {
        camara >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, gris, cv::COLOR_BGR2GRAY);

        switch (filtro_actual) {
            case 0: resultado = gris.clone();                   break;
            case 1: resultado = aplicar_log(gris);              break;
            case 2: resultado = aplicar_zero_crossing(gris);    break;
            case 3: resultado = aplicar_scharr(gris);           break;
            case 4: resultado = aplicar_laplacian(gris);        break;
            case 5: resultado = aplicar_sobel(gris);            break;
            case 6: resultado = aplicar_sobel_magnitudes(gris); break;
            case 7: resultado = aplicar_canny(gris, 50, 150);   break;
        }

        // Mostrar el nombre del filtro en pantalla
        std::string texto = "[" + std::to_string(filtro_actual + 1) + "/" +
                            std::to_string(total) + "] " + nombres[filtro_actual];
        cv::putText(resultado, texto, cv::Point(10, 28),
                    cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255), 2);

        cv::imshow("Taller Filtros", resultado);

        // & 0xFF: en Linux waitKey puede devolver el codigo con bits extra
        int tecla = cv::waitKey(30) & 0xFF;

        // Cerrar con 'q' o con la X de la ventana
        if (tecla == 'q') break;
        if (cv::getWindowProperty("Taller Filtros", cv::WND_PROP_VISIBLE) < 1) break;

        if (tecla == 'n') filtro_actual = (filtro_actual + 1) % total;
        if (tecla == 'p') filtro_actual = (filtro_actual - 1 + total) % total;
    }

    camara.release();
    cv::destroyAllWindows();
    return 0;
}
