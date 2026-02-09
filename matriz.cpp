#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    int ancho = 3;
    int alto = 2;

    int matriz[2][3][3] = {
        {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
        {{255, 255, 0}, {255, 255, 255}, {0, 0, 0}}
    };

    ofstream archivo("mi_imagen.ppm");

    archivo << "P3" << endl;
    archivo << ancho << " " << alto << endl;
    archivo << "255" << endl;

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            archivo << matriz[i][j][0] << " " 
                    << matriz[i][j][1] << " " 
                    << matriz[i][j][2] << "  ";
        }
        archivo << endl;
    }

    archivo.close();

    return 0;
}