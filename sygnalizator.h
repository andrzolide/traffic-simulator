#ifndef SYGNALIZATOR_H
#define SYGNALIZATOR_H
#include <QObject>
#include <QGraphicsRectItem>
#include <iostream>
#include <list>
#include <iterator>

#include "punkt.h"
#include "mapa.h"


struct element_sygnalizatora
{
    QGraphicsRectItem *element;
    int strona; // strony są zapisywane tak jak kierunki. ( 1 - gora, 2- lewo, 3-dol, 4 - prawo )
    bool stan=false;  //zapalony na zielono albo zgaszony.
    QBrush br; // sluzy do kolorków
    element_sygnalizatora();
};

class sygnalizator:public QObject
{
Q_OBJECT
private:
    int co_ile_zmiana=10000;
    punkt *pkt_sygnalizatora;

    Mapa *mapa;
    int wymiar_sygnalizatora=10;

public:
    std::list <element_sygnalizatora> elementy_sygnalizatora;
    sygnalizator(punkt* pkt_sygn,Mapa *map);
    void dodajSygnalizator(int kierunek,punkt * pkt);
    void ustawKolor(Qt::GlobalColor color,element_sygnalizatora *elem);


    punkt *getPkt_sygnalizatora() const;

public slots:

    void zmiana();
};



#endif // SYGNALIZATOR_H
