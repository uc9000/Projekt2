#pragma once
#include "Gracz.h"
#include <sstream>
class Bot : public Gracz{
    private:
    int odwaga; //limit punktow do ktorego Bot bedzie dobieral
    public:
    Bot();
    void setOdwaga(int _odwaga);
    int getOdwaga() const;
    void enterNazwa(int n);
    bool nextMove(); //automatycznie podejmuje decyzje na podstawie odwagi
};