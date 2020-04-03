//do kasyna
#include <algorithm>
#include <random>
#include <chrono>
//do gracza
#include <vector>
#include <string>
#include <cctype>
#include "Karta.h"
//do Bota
#include <sstream>
class Kasyno; //forward declaration zeby gracz wiedzial czy nalezy do kasyna
class Gracz{    
    protected:    
    std::vector <Karta> inHand; //vector kart w rece gracza, agregacja
    bool pass; //czy spasowal
    std::string nazwa;
    Kasyno* kasyno_ptr;

    public:
    //funkcja wejscia
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
        //std::cout << "Wprowadzono liczbe: " << stoi(str) << std::endl;
        return stoi(str);
    }

    //KONSTRUKTORY
    Gracz():
        pass(false),
        kasyno_ptr(NULL)
    {}

    virtual ~Gracz(){
        inHand.clear();
    }

    //AKCESORY
    void setKasyno(Kasyno* kasyno){
        kasyno_ptr = kasyno;
    }

    void wezKarte(Karta * _karta){
        if(kasyno_ptr == NULL){
            std::cerr << "ERROR: Gracz nie jest przypisany do kasyna" << std::endl;
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

    std::string getNazwa(){
        return nazwa;
    }

    void printInHand(){
        std::cout << "Karty w ręce:" << std::endl;
        for(auto a : inHand){
            a.wypisz();
            std::cout << std::endl;
        }
        std::cout << "Ich suma to: " << points() << std::endl;
    }

    void clear(){ //zabiera karty z reki gracza
        inHand.clear();
    }

    virtual void enterNazwa(int n){
        std::cout << "Podaj nazwe gracza nr " << n << " : " << std::endl;
        std::cin >> nazwa;
    }

    virtual bool nextMove(){ //Podejmuje decyzje czy dobrac czy pasowac
        if(points() > 19){ //idiotoodpornosc +1
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
};

///////////////////////BOT///////////////////////

class Bot : public Gracz{
    private:
    int odwaga; //limit punktow do ktorego Bot bedzie dobieral

    public:
    Bot():
    Gracz(),
    odwaga(16) //odwaga dla normalnego gracza
    {}

    void setOdwaga(int _odwaga){
        if(_odwaga > 0 && _odwaga < 20){
            odwaga = _odwaga;
        }else{
            std::cerr << "Odwaga poza skala!" << std::endl;
        }
    }

    int getOdwaga() const{
        return odwaga;
    }

    void enterNazwa(int n){
        std::stringstream ss;
        ss << "BOT " << n;
        nazwa = ss.str();
    }

    bool nextMove(){ //automatycznie podejmuje decyzje na podstawie odwagi
        if (points() <= odwaga){
            std::cout << getNazwa() << " dobral karte!" << std::endl;
            pass = false;
            return true;
        }
        else{
            std::cout << getNazwa() << " spasowal!" << std::endl;
            pass = true;
            return false;
        }
    }
};

//////////////////////KASYNO////////////////////////

class Kasyno{
    private:
    //kompozycja z klas Karta i Gracz
    Karta* mainDeck;
    int size;
    std::vector<Gracz*> listaGraczy; //edit: graczy i botow
    Karta tmp; //karta przekazywana graczowi
    //wskazniki pomocnicze:
    Gracz* g;
    Bot* b;
    int botCount;
    
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
        g = NULL;
        b = NULL;        
        size = 0;
        for(auto a: listaGraczy){
            a->clear();
        }
        Karta k = Karta();
        for(int i = 0; i < 13; i++){
            for(int j = 0; j < 4; j++){
                k.setKolor(j);
                k.setWartosc(i);
                mainDeck[size] = k;
                size++;
            }
        }
        size_t s = std::chrono::system_clock::now().time_since_epoch().count(); //czas jako seed
        std::shuffle(mainDeck, mainDeck + size, std::default_random_engine(s)); //tasowanie kart
        printDeck();
    }

    void addPlayer(){
        g = new Gracz();
        g->setKasyno(this);
        g->enterNazwa(listaGraczy.size() + 1);
        listaGraczy.push_back(g);
    }

    void addBot(){
        botCount++;
        b = new Bot();
        b->setKasyno(this);
        b->enterNazwa(botCount);
        listaGraczy.push_back(b);
    }

    //KONIEC PRIVATE

    public:
    Kasyno(){
        g = NULL;
        b = NULL;        
        size = 0;
        botCount = 0;
        mainDeck = new Karta[52];
    }

    ~Kasyno(){
        delete [] mainDeck;
        listaGraczy.clear();
        //std::cout << "Destruktor" << std::endl;        
    }

    Karta* dajKarte(){
        tmp = mainDeck[size - 1];
        size--;
        return &tmp;
    }

    void reset(){
        listaGraczy.clear();
        botCount = 0;
    }

    void runOczko(){
        if(listaGraczy.size() < 2){
            std::cout << "Musisz dodac conajmniej dwoch graczy!" << std::endl;
            return;
        }
        initDeck();
        //rozdanie po dwie karty kazdemu
        for(int i = 0; i < 2; i++){
            for(auto a : listaGraczy){
                a->wezKarte(dajKarte());
            }
        }
        //poczatek gry
        bool dontFinish = true;
        while(dontFinish){
            for(auto a : listaGraczy){
                std::cout << std::endl << "---------------------" << std::endl << "Tura gracza " << a->getNazwa() << std::endl;
                a->printInHand();
                if(a->nextMove()){
                    a->wezKarte(dajKarte());
                }
            }
            for(auto a : listaGraczy){ //sprawdzenie czy kazdy spasowal
                if(!a->getPass()){
                    dontFinish = true;
                    break;
                }
                else{
                    dontFinish = false;
                    continue;
                }
            }
        }
        //sortowanie listy graczy od najwiekszego wyniku
        std::sort(listaGraczy.begin(), listaGraczy.end(), [] (Gracz* a, Gracz* b) { return a->points() > b->points(); } );
        Gracz* winner = NULL;
        //wylonienie zwyciezcy przy werblach
        std::cout << "Lista wynikow: " << std::endl;
        for(auto a : listaGraczy){
            std::cout << "Wynik dla " << a->getNazwa() << " wynosi : " << a->points();
            if(a->points() > 21){
                std::cout << " L00SER!";
            }
            std::cout  << std::endl;
            if(winner == NULL && a->points() < 22){
                winner = a;
            }
        }
        std::cout << std::endl << "THE WINNER IS: << " << winner->getNazwa() << " >>"<< std::endl << std::endl;
    }

    void menu(){
        Gracz g = Gracz();
        int w = 0;
        std::cout << "Witoj w grze w Oczko! \nWybierz jedna z opcji:" << std::endl;
        while(true){
            switch(w){
                case 0:
                std::cout << "1) Dodaj gracza" << std::endl
                << "2) Dodaj bota" << std::endl
                << "3) Rozpocznij nowa gre" << std::endl
                << "4) Wyswietl liste graczy i botow" << std::endl
                << "5) Usun wszystkich graczy i boty" << std::endl
                << "5) Zakoncz" << std::endl;
                w = g.dajInt();
                break;

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

                case 4:
                for(auto a : listaGraczy){
                    std::cout << a->getNazwa() << std::endl;
                }
                w = 0;
                break;

                case 5:
                reset();
                w = 0;
                return;

                case 6:
                return;
            }
        }
    }
};

int main(){
    Kasyno k = Kasyno();
    k.menu();
    return 0;
}