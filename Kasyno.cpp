#include "Kasyno.h"
void Kasyno::printDeck() const{
    for(int i = 0; i < 52; i++){
        mainDeck[i].wypisz();
        //std::cout << std::endl << "size = " << size << std::endl;
    }
    std::cout << std::endl;
}

void Kasyno::initDeck(){ //tworzenie tablicy 52 unikalnych, potasowanych kart
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

void Kasyno::addPlayer(){
    if((listaGraczy.size() - botCount) == 3){
        std::cout << "Osiagnieto limit graczy!" << std::endl;
        return;
    }
    g = new Gracz();
    g->setKasyno(this);
    g->enterNazwa(listaGraczy.size() + 1);
    listaGraczy.push_back(g);
    std::cout << "Pomyslnie utworzono gracza " << g->getNazwa() << std::endl;
}

void Kasyno::addBot(){
    if(botCount == 3){
        std::cout << "Osiagnieto limit botow!" << std::endl;
        return;
    }
    b = new Bot();
    int w = 0;
    int odwaga;
    std::cout << "Jakiego typu BOTa chcesz dodac?" << std::endl;
    while(w >= 0){
        switch(w){
            case 0:
            std::cout << "1) Ryzykujacy" << std::endl
            << "2) Normalny" << std::endl
            << "3) Zachowawczy" << std::endl;
            while(true){
                w = b->dajInt();
                if(w > 3 || w < 1){
                    std::cerr << "Wybierz jedna z dostepnych opcji!" << std::endl;
                    continue;
                }
                break;
            }
            break;

            case 1:
            odwaga = 17;
            w = -1;
            break;

            case 2:
            odwaga = 15;
            w = -1;
            break;

            case 3:
            odwaga = 12;
            w = -1;
            break;
        }
    }
    botCount++;
    b->setOdwaga(odwaga);
    b->setKasyno(this);
    b->enterNazwa(botCount);
    listaGraczy.push_back(b);
    std::cout << "Pomyslnie utworzona " << b->getNazwa() << std::endl;
}

Karta* Kasyno::dajKarte(){
    tmp = mainDeck[size - 1];
    size--;
    return &tmp;
}

Kasyno::Kasyno():      
    size(0),
    botCount(0),
    g(NULL),
    b(NULL)
{
    mainDeck = new Karta[52];
}

Kasyno::~Kasyno(){
    delete [] mainDeck;
    listaGraczy.clear();
    //std::cout << "Destruktor" << std::endl;        
}

void Kasyno::reset(){
    listaGraczy.clear();
    botCount = 0;
}

void Kasyno::runOczko(){
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
    std::vector<Gracz*> winners;
    int winnerScore = 0;
    //wylonienie zwyciezcy przy werblach
    std::cout << "Lista wynikow: " << std::endl;
    for(auto a : listaGraczy){
        std::cout << "Wynik dla " << a->getNazwa() << " wynosi : " << a->points();
        if(a->points() > 21){
            std::cout << " L00SER!";
        }
        if(a->points() == 21){
            std::cout << " OCZKO!";
        }
        std::cout  << std::endl;
        if(a->points() < 22 && a->points() >= winnerScore){
            winnerScore = a->points();
            winners.push_back(a);
        }
    }
    if(winners.size() == 0){
        std::cout << "WSZYSCY PRZEGRALI xD" << std::endl;
    }
    else if(winners.size() == 1){
        std::cout << std::endl << "THE WINNER IS: << " << winners.at(0)->getNazwa() << " >>"<< std::endl << std::endl;
    }
    else{
        std::cout << std::endl << "REMIS! Gracze: " << std::endl;
        for(auto w : winners){
            std::cout << w->getNazwa() << std::endl;
        }
        std::cout << "Uzyskali ten sam wynik i wygrali." << std::endl;
    }
}

void Kasyno::menu(){
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
            << "6) Zakoncz" << std::endl;
            while(true){
                w = g.dajInt();
                if(w > 6 || w < 1){
                    std::cout << "Wybierz jedna z dostepnych opcji z menu!" << std::endl;
                    continue;
                }
                break;
            }                
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
            break;

            case 6:
            std::cout << "Czy napewno chcesz zakonczyc?" << std::endl;
            if(g.takCzyNie()){
                return;
            }
            else{
                w = 0;
                break;
            }                
        }
    }
}