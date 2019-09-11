#include "linia.h"
#include "punkt.h"
#include "math.h"
#include <QtDebug>


linia::linia(punkt * p1, punkt * p2)
{
this->p1=p1;
    this->p2=p2;
}

void linia::wyswietl()
{
    this->p1->wyswietl();
    this->p2->wyswietl();
}
