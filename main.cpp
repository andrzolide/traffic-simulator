#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QDebug>
#include <QKeyEvent>
#include <iostream>
#include <ctime>
#include "punkt.h"
#include "mapa.h"
#include "auto.h"


/*
Z CZYM SIĘ MĘCZYŁEM NAJDŁUŻEJ?

1.
Z tym, że jak w klasie mapa mam listę linii a w każdej linii listę wskaźników do aut to nie mogę
w klasie auto zaincludować mapy i jednocześnie w klasie mapy zaincludować auta. ( około 5h ) . trzeba było a klasie mapy ( listy ) napisać : class Auto;.
czyli poinfmować kompilator ze istnieje taka klasa jak Auto/

2.


*/


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //tworzenie sceny
    QGraphicsScene* scene= new QGraphicsScene();
    scene->setSceneRect(-25,-25,1500,850);

    // dodawanie obiektow
    //ustawianie parametrów obiektów
    Mapa * map=new Mapa(scene);
    map->wyswietl();
    map->tworz_ulice();
    map->tworz_sygnalizatory();


    Auto* auto1=new Auto(map);


    //dodaj przedmiot do sceny
    scene->addItem(auto1);

    //nastawianie fokusu na mape ( tylko w przypadku używania eventów )

    //map->setFlag(QGraphicsItem::ItemIsFocusable);
    //map->setFocus();

    //widok

    QGraphicsView * widok=new QGraphicsView(scene);
    widok->show();
    widok->setFixedSize(1500,850);

       //znaki oraz sygnalizoatory jako osobne obiekty?
    //

    return a.exec();
}
