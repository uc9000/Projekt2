#include "Gracz.h"

long int Gracz::dajInt() {
    using namespace std;
    const int sizeLimit = 5;
    string str;
    while (true) {
        cin >> str;
        if ((int)str.size() > sizeLimit) {
            cerr << "Liczba jest za dluga. Podaj liczbe calkowita: " << endl;
            continue;
        }
        for (int i = 0; i < (int)str.size(); i++) {
            if (!(isdigit(str[i]) || str[i] == '-')) {
                cerr << "Podaj liczbe calkowita: ";
                continue;
            }
        }
        break;
    }
    //std::cout << "Wprowadzono liczbe: " << stoi(str) << std::endl;
    return stoi(str);
}

bool Gracz::takCzyNie() {
    using namespace std;
    int q;
    while (true) {
        cout << "	1) Tak\n	2) Nie" << endl;
        q = dajInt();
        if (q == 1)
            return true;
        else if (q == 2)
            return false;
        else
            continue;
    }
    return false;
}

//KONSTRUKTOR I DESTRUKTOR
Gracz::Gracz():
    pass(false),
    kasyno_ptr(NULL)
{}

Gracz::~Gracz(){
    inHand.clear();
}
//AKCESORY

void Gracz::setKasyno(Kasyno* kasyno){
    kasyno_ptr = kasyno;
}

void Gracz::wezKarte(Karta * _karta){
    if(kasyno_ptr == NULL){
        std::cerr << "ERROR: Gracz nie jest przypisany do kasyna" << std::endl;
        return;
    }
    inHand.push_back(*_karta);
}

int Gracz::points(){
    int suma = 0;
    for(int i = 0; i < (int)inHand.size(); i++){
        suma += inHand.at(i).getWartosc();
    }
    return suma;
}

void Gracz::setPass(){
    pass = true;
}

bool Gracz::getPass() const{
    return pass;
}

void Gracz::setNazwa(std::string n){
    nazwa.append(n, 20);
}

std::string Gracz::getNazwa(){
    return nazwa;
}

void Gracz::printInHand(){
    std::cout << "Karty w ręce:" << std::endl;
    for(auto a : inHand){
        a.wypisz();
        std::cout << std::endl;
    }
    std::cout << "Ich suma to: " << points() << std::endl;
}

void Gracz::clear(){ //zabiera karty z reki gracza
    pass = false;
    inHand.clear();
}

void Gracz::enterNazwa(int n){
    std::cout << "Podaj nazwe gracza nr " << n << " : " << std::endl;
    std::cin >> nazwa;
    nazwa.resize(20);
}

bool Gracz::nextMove(){ //Podejmuje decyzje czy dobrac czy pasowac
    if(points() > 19 || pass == true){ //idiotoodpornosc +1
        pass = true;
        return false;
    }
    if(inHand.size() >= 10){
        std::cout << "Osiagnieto limit kart w rece gracza (10)" << std::endl
        << "Gracz " << nazwa << " spasowal." << std::endl;
        pass = true;
        return false;
    }
    std::cout << "1) Dobierz karte" << std::endl
    << "2) Pas" << std::endl;
    int wybor = 0;
    while(true){
        wybor = dajInt();
        if(!(wybor == 1 || wybor == 2)){
            std::cout << "Wybierz jedna z podanych opcji! " << std::endl;
            continue;
        }
        break;
    }
    if(wybor == 1){
        pass = false;
        return true;
    }
    else{
        pass = true;
        return false;
    }
}