#ifndef LINIA_H
#define LINIA_H
#include <iostream>
#include <list>
#include <iterator>
#include "punkt.h"
#include <QGraphicsRectItem>
//#include "auto.h"


class Auto;

using namespace std;

class linia
{

public:
    punkt* p1;
    punkt* p2;
    list <Auto*> listaAut;

    linia(punkt * p1, punkt * p2);
    void wyswietl();
};

#endif // LINIA_H
