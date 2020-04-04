#pragma once
#include "Gracz.h"
#include "Bot.h"
#include <algorithm>
#include <random>
#include <chrono>

class Kasyno{
    private:
    //kompozycja z klas Karta i Gracz
    Karta* mainDeck;
    int size;
    std::vector<Gracz*> listaGraczy; //edit: graczy i botow
    Karta tmp; //karta przekazywana graczowi
    //wskazniki pomocnicze:
    int botCount;
    Gracz* g;
    Bot* b;
    void printDeck() const;
    void initDeck();//tworzenie tablicy 52 unikalnych, potasowanych kart
    void addPlayer();
    void addBot();
    Karta* dajKarte();

    public:
    Kasyno();
    ~Kasyno();    
    void reset();
    void runOczko();
    void menu();
};