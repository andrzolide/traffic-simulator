#ifndef PUNKT_H
#define PUNKT_H
#include <QGraphicsRectItem>

class punkt:public QGraphicsRectItem{
public:
    int y;
    int x;

    void tworzPunkt(int x=0 , int y=0, int wymiarSiatki=0);
    void wyswietl();
};

#endif // PUNKT_H
