#ifndef MAPA_H
#define MAPA_H

#include <iostream>
#include <list>
#include <QGraphicsScene>
#include <QObject>

#include "punkt.h"
#include "linia.h"

class sygnalizator;



using namespace std;

const int xSiatki=30;
const int ySiatki=17;
const int wymiarSiatki=50;



class Mapa :public QObject
{
Q_OBJECT
public:
    int czestotliwoscPojawianiaSieAut=2000;// z taką czestotliwoscia beda pojawiac sie auta na mapie.
    int czestotliwoscSprawdzaniaPrzypadkowSzczegolnych=2000;
    bool pom=0;
    int licznik=0;
    QGraphicsScene* scena;
    punkt ** siatka = new punkt * [xSiatki];
    //punkt siatka[xSiatki][ySiatki]={};
    list <linia> listaUlic;
    list <punkt*> sygnalizatory;
    list <linia*> pomocniczaListaUlic;
    list <sygnalizator*> listaSygnalizatorow;

    Mapa(QGraphicsScene* scene); // inicjalizacja mapy czyli listy punktow
    void tworz_ulice();// tworzenie ulic poźniej można zaimplementować wczytywanie mapy z pliku.
    void wyswietl();
    void dodajDoSceny();
    void tworz_sygnalizatory();
    QBrush br;
    bool wlaczIgnorowaniePrawej=false;

public slots:

    void tworz_auto();
    void sprawdzPrzypadkiszczegolne();
};

#endif // MAPA_H
