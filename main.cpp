//do kasyna
#include <algorithm>
#include <random>
#include <chrono>
//do gracza
#include <vector>
#include <string>
#include <cctype>
#include "Karta.h"

class Gracz{
    protected:
    Karta* mainDeck;//wskaznik do kart pozostalych w talii (agregacja)
    int* mD_size;//wskaznik do ilosci kart pozostalych w talii (agregacja)
    std::vector <Karta> inHand; //vector kart w rece gracza
    bool pass; //czy spasowal
    std::string nazwa;    

    public:
    //funkcja wejscia do menu
    long int dajInt() {
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
        return stoi(str);
    }

    Gracz(){
        mainDeck = NULL;
        mD_size = NULL;
        pass = false;
    }

    Gracz(Karta* mD, int* size){
        mainDeck = mD;
        mD_size = size;
        pass = false;
    }

    ~Gracz(){
        inHand.clear();
    }

    void setMainDeck(Karta* mD){
        mainDeck = mD;
    }

    void setMainDeckSize(int* size){
        mD_size = size;
    }

    void wezKarte(Karta * _karta){ //nie wiem po co w ogole jakis argument do tej funkcji na ale tak wymusili w instrukcji...
        if(mD_size == 0 || mainDeck == NULL){
            std::cerr << "Error: Nie przypisano gracza do kasyna";
            return;
        }
        inHand.push_back(*_karta);
    }

    int points(){
        int suma = 0;
        for(int i = 0; i < (int)inHand.size(); i++){
            suma += inHand.at(i).getWartosc();
        }
        return suma;
    }

    void setPass(){
        pass = true;
    }

    bool getPass() const{
        return pass;
    }

    void setNazwa(std::string n){
        nazwa = n;
    }

    virtual void enterNazwa(int n){ //TODO: Polimorfizm do AI z tej metody
        std::cout << "Podaj nazwe gracza nr " << n << " : " << std::endl;
        std::getline(std::cin, nazwa, '\n');
    }

    virtual bool nextMove(){ //TODO: polimorfizm do AI z tej metody
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
            return true;
        }
        else{
            return false;
        }
    }
};


class Bot : public Gracz{
    private:
    int odwaga; //limit punktow do ktorego Bot bedzie dobieral

    public:
    Bot(){
        odwaga = 16; //odwaga dla normalnego gracza
    }

    Bot(int _odwaga){
        if(_odwaga > 0 && _odwaga < 20){
            odwaga = _odwaga;
        }
        else{
            std::cerr << "Odwaga poza dostepnym zakresem (u mad bro!?)" << std::endl
            << "Bot utworzony z domyslna wartoscia odwagi (16)";
            odwaga = 16;
        }
    }

    void enterNazwa(int n){
        nazwa.append("BOT ");
        nazwa.append(n);
    }

    bool nextMove(){
        if (points() < odwaga){
            return true;
        }
        else{
            return false;
        }
    }
};


class Kasyno{
    private:
    //kompozycja z klas Karta i Gracz
    Karta* mainDeck;
    int size;
    std::vector<Gracz*> listaGraczy;
    Karta tmp; //karta przekazywana graczowi

    void clear(){
        if(mainDeck != NULL){
            delete[] mainDeck;
            mainDeck = NULL;
        }
    }
    
    void push_back(Karta& k){
        size++;
        Karta* tmp_new = new Karta[size];
        Karta* tmp_old = mainDeck;
        for(int i = 0; size > 0 && i < size - 1; i++){
            tmp_new[i] = mainDeck[i];
        }
        mainDeck = tmp_new;
        mainDeck[size - 1] = k;        
        if(tmp_old != NULL){
            delete[] tmp_old;
            tmp_old = NULL;
        }
        //std::cout << "push: " << k.getWartosc() << " kol: " << k.getKolor() << std::endl;
        //std::cout << "size = " << size << std::endl << std::endl;
    }

    void pop_back(){
        if (size > 0){
            size--;
            Karta* tmp_new = new Karta[size];
            for(int i = 0; i < size; i++){
                tmp_new[i] = mainDeck[i];
            }
            clear();
            mainDeck = tmp_new;
        }
    }
///*
    void printDeck() const{
        for(int i = 0; i < 52; i++){
            mainDeck[i].wypisz();
            //std::cout << std::endl << "size = " << size << std::endl;
        }
        std::cout << std::endl;
    }
//*/
    void initDeck(){ //tworzenie tablicy 52 unikalnych, potasowanych kart
        clear();
        Karta k = Karta();
        for(int i = 0; i < 13; i++){
            for(int j = 0; j < 4; j++){
                k.setKolor(j);
                k.setWartosc(i);
                push_back(k);
            }
        }
        size_t s = std::chrono::system_clock::now().time_since_epoch().count(); //czas jako seed
        std::shuffle(mainDeck, mainDeck + size, std::default_random_engine(s)); //tasowanie kart
        printDeck();
    }

    void addPlayer(){
        Gracz g = Gracz(mainDeck, &size);
        g.enterNazwa(listaGraczy.size() + 1);
        listaGraczy.push_back(&g);
    }

    void addBot(){
        //TODO
    }

    //KONIEC PRIVATE


    public:
    Kasyno(){
        mainDeck = NULL;
        size = 0;
        //initDeck();
    }

    ~Kasyno(){
        listaGraczy.clear();
        //std::cout << "Destruktor" << std::endl;
        if(mainDeck != NULL){
            delete[] mainDeck;
        }
    }

    Karta* dajKarte(){
        tmp = mainDeck[size - 1];
        pop_back();
        return &tmp;
    }

    void reset(){
        initDeck();
        listaGraczy.clear();
    }

    void runOczko(){
        initDeck();     
    }

    void menu(){
        Gracz g = Gracz(mainDeck, &size);
        int w = 0;
        while(true){
            switch(w){
                case 0:
                std::cout << "1) Dodaj gracza" << std::endl
                << "2) Dodaj bota" << std::endl
                << "3) Rozpocznij nowa gre" << std::endl
                << "4) Wyswietl liste graczy i botow" << std::endl
                << "5) Zakoncz" << std::endl;
                w = g.dajInt();

                case 1:
                addPlayer();
                w = 0;
                break;

                case 2:
                addBot();
                w = 0;
                break;

                case 3:
                runOczko();
                w = 0;
                break;

                case 4: //TODO
                //printPlayers();
                //printBots();
                w = 0;
                break;

                case 5:
                return;
            }
        }
    }
};

int main(){
    Kasyno k = Kasyno();
    k.runOczko();
    return 0;
}