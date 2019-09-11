#ifndef LINIATEST_H
#define LINIATEST_H
#include <iostream>
#include <list>
#include <iterator>
#include "punkt.h"
#include <QGraphicsRectItem>
//#include "auto.h"


class liniatest:public QGraphicsRectItem
{
public:
    punkt* p1;
    punkt* p2;
    //list <Auto*> listaAut;

    liniatest(punkt * p1, punkt * p2);
};

#endif // LINIATEST_H
