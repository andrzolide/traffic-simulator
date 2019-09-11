#include "punkt.h"
#include <QGraphicsScene>
#include <QDebug>

void punkt::tworzPunkt(int x, int y,int wymiarSiatki)
{
this->x=x;
this->y=y;
    setRect(x-wymiarSiatki/2,y-wymiarSiatki/2,wymiarSiatki,wymiarSiatki);
}

void punkt::wyswietl()
{
    qDebug()<<"punkt: "<<this->x<<" "<<this->y;
}
