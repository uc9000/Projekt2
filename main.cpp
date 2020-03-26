#include <algorithm>
#include <random>
#include <chrono>
#include <vector>
#include "Karta.h"

class Gracz{
    private:
    std::vector<Karta>* mainDeck;//wskaznik na vector kart pozostalych w talii (agregacja)
    std::vector <Karta> inHand; //vector kart w rece gracza

    public:
    Gracz(){
        mainDeck = new std::vector<Karta>();
    }

    Gracz(std::vector<Karta>* mD){
        mainDeck = mD;
    }

    void setMainDeck(std::vector<Karta>* mD){
        mainDeck = mD;
    }

    void dajKarte(){ //pobiera karte z gory stosu kart i daje graczowi
        inHand.push_back(mainDeck->back());
        mainDeck->pop_back();
    }

    int points(){
        int suma = 0;
        for(int i = 0; i < (int)inHand.size(); i++){
            suma += inHand.at(i).getWartosc();
        }
        return suma;
    }    
};

class Kasyno{
    private:
    //kompozycja z klas Karta i Gracz
    std::vector<Karta> mainDeck;
    std::vector<Gracz> listaGraczy;

    void printDeck() const{
        for(int i = 0; i < 52; i++){
            mainDeck.at(i).wypisz();
            std::cout << std::endl;
        }
    }

    void initDeck(){ //tworzenie vectora 52 unikalnych, potasowanych kart
        mainDeck.clear();
        for(int i = 0; i < 13; i++){
            for(int j = 0; j < 4; j++){
                mainDeck.push_back(Karta(j, i)); 
            }
        }
        size_t s = std::chrono::system_clock::now().time_since_epoch().count(); //czas jako seed
        std::shuffle(mainDeck.begin(), mainDeck.end(), std::default_random_engine(s)); //tasowanie kart
        printDeck();
    }

    public:

    Kasyno(){
        initDeck();
    }

    void reset(){
        initDeck();
        listaGraczy.clear();
    }

    void runOczko(){
        initDeck();
    }
};

int main(){
    Kasyno k;
    k.runOczko();
    return 0;
}