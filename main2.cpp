#include "Karta.h"

int main(){
    Karta k = Karta (0, 0);
    k.setKolor(1);
    k.setWartosc(8);
    std::cout << "Figura: " << k.getFigura() << " ; Kolor: " << (int)k.getKolor() << std::endl;
}