#include "Bot.h"

Bot::Bot():
    Gracz(),
    odwaga(15) //odwaga dla normalnego gracza
{}

void Bot::setOdwaga(int _odwaga){
    if(_odwaga > 0 && _odwaga < 20){
        odwaga = _odwaga;
    }else{
        std::cerr << "Odwaga poza skala!" << std::endl;
    }
}

int Bot::getOdwaga() const{
    return odwaga;
}

void Bot::enterNazwa(int n){
    std::stringstream ss;
    ss << "BOT " << n;
    nazwa = ss.str();
}

bool Bot::nextMove(){ //automatycznie podejmuje decyzje na podstawie odwagi
    if (points() <= odwaga && inHand.size() < 10){
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