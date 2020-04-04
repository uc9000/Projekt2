#pragma once
#include <vector>
#include <string>
#include <cctype>
#include "Karta.h"


class Kasyno; //forward declaration zeby gracz wiedzial czy nalezy do kasyna
class Gracz{    
    protected:    
    std::vector <Karta> inHand; //vector kart w rece gracza, agregacja za pomoca metody wezKarte()
    std::string nazwa; //ograniczenie do 20 znakow w akcesorach / metodach wejscia
    bool pass; //czy spasowal
    Kasyno* kasyno_ptr;

    public:
    //funkcja wejscia
    long int dajInt();
    bool takCzyNie();
    //KONSTRUKTOR I DESTRUKTOR
    Gracz();
    virtual ~Gracz();
    //AKCESORY
    void setKasyno(Kasyno* kasyno);
    void wezKarte(Karta * _karta);
    int points();
    void setPass();
    bool getPass() const;
    void setNazwa(std::string n);
    std::string getNazwa();
    void printInHand();
    void clear(); //zabiera karty z reki gracza
    virtual void enterNazwa(int n);
    virtual bool nextMove(); //Podejmuje decyzje czy dobrac czy pasowac
};