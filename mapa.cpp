#include "mapa.h"
#include <iostream>
#include <list>
#include <iterator>
#include "punkt.h"
#include "linia.h"
#include <QDebug>
#include <QPointF>
#include <QGraphicsScene>
#include <auto.h>
#include <QTimer>
#include "math.h"
#include "sygnalizator.h"


using namespace std;


Mapa::Mapa(QGraphicsScene* scene)
{
    scena=scene;
    // generowanie poszczegolnych wymiarów tablicy siatki i tworzenie obiektow punkt
    for (int i = 0; i<xSiatki; i++)
       siatka[i] = new punkt [ySiatki];


    //wypelnianie obiektów punkt odpowiednimi wartosciami. Jako, że do tablicy nie da się zapisywać obiektów używając konstruktora musiałem stworzyć osobną metodę.
    for(int i=0;i<xSiatki;i++){
        for(int j=0;j<ySiatki;j++){
            siatka[i][j].tworzPunkt(wymiarSiatki*i,wymiarSiatki*j,wymiarSiatki);
            scene->addItem(&siatka[i][j]);
        }
    }
    //paletka
    br=QBrush(Qt::SolidPattern);
    //tajmer służący do tworzenia aut.
    QTimer *timer= new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(tworz_auto()));
    timer->start(czestotliwoscPojawianiaSieAut);
    //tajmer służący do sprawdzania przypadkow szczegolnych
    QTimer *timer2= new QTimer;
    connect(timer2,SIGNAL(timeout()),this,SLOT(sprawdzPrzypadkiszczegolne()));
    timer2->start(czestotliwoscSprawdzaniaPrzypadkowSzczegolnych);
}

void Mapa::tworz_auto()
{
    Auto* auto1=new Auto(this);
    scena->addItem(auto1);
}




// tworzenie ulic. Na razie zadeklarowane na sztywno, potem można zrobić jakieś wczytywanie z pliku czy coś takiego.
void Mapa::tworz_ulice(){

    //poziom

    listaUlic.push_front(*new linia(&siatka[0][10],&siatka[5][10]));
        listaUlic.push_back(*new linia(&siatka[0][10],&siatka[0][11]));
    listaUlic.push_back(*new linia(&siatka[5][10],&siatka[10][10]));
    listaUlic.push_back(*new linia(&siatka[10][10],&siatka[15][10]));
    listaUlic.push_back(*new linia(&siatka[15][10],&siatka[20][10]));
    listaUlic.push_back(*new linia(&siatka[20][10],&siatka[25][10]));

    listaUlic.push_back(*new linia(&siatka[5][5],&siatka[10][5]));
    listaUlic.push_back(*new linia(&siatka[10][5],&siatka[15][5]));
    listaUlic.push_back(*new linia(&siatka[15][5],&siatka[20][5]));
    listaUlic.push_back(*new linia(&siatka[20][5],&siatka[25][5]));
    listaUlic.push_back(*new linia(&siatka[25][5],&siatka[29][5]));
        listaUlic.push_back(*new linia(&siatka[29][5],&siatka[29][4]));

    listaUlic.push_back(*new linia(&siatka[0][1],&siatka[10][1]));
        listaUlic.push_back(*new linia(&siatka[0][1],&siatka[0][2]));
    listaUlic.push_back(*new linia(&siatka[10][1],&siatka[20][1]));
    listaUlic.push_back(*new linia(&siatka[20][1],&siatka[25][1]));

    listaUlic.push_back(*new linia(&siatka[10][15],&siatka[15][15]));
    listaUlic.push_back(*new linia(&siatka[15][15],&siatka[20][15]));

    //pion

    listaUlic.push_back(*new linia(&siatka[5][5],&siatka[5][10]));

    listaUlic.push_back(*new linia(&siatka[10][1],&siatka[10][5]));
    listaUlic.push_back(*new linia(&siatka[10][10],&siatka[10][15]));

    listaUlic.push_back(*new linia(&siatka[15][5],&siatka[15][10]));
    listaUlic.push_back(*new linia(&siatka[15][10],&siatka[15][15]));

    listaUlic.push_back(*new linia(&siatka[20][1],&siatka[20][5]));
    listaUlic.push_back(*new linia(&siatka[20][5],&siatka[20][10]));
    listaUlic.push_back(*new linia(&siatka[20][10],&siatka[20][15]));

    listaUlic.push_back(*new linia(&siatka[25][1],&siatka[25][5]));
    listaUlic.push_back(*new linia(&siatka[25][5],&siatka[25][10]));


    for(list <linia> :: iterator it=listaUlic.begin();it!=listaUlic.end();it++)
    {
        //ustawianie koloru na rogach.
        br.setColor(Qt::gray);
        it.operator*().p1->setBrush(br);
        it.operator*().p1->update();
        //petla wykonuje sie tyle razu ile jest pointsow. czyli ustawianie koloru na liniach.
        bool * kierunek24=new bool;
        if(it.operator*().p1->y==it.operator*().p2->y){*kierunek24=true;}
            else if(it.operator*().p1->x==it.operator*().p2->x) {*kierunek24==false;}
        int *p1x=new int;int *p1xpom=new int;
        int *p1y=new int;int *p1ypom=new int;
        int *p2x=new int;int *p2xpom=new int;
        int *p2y=new int;int *p2ypom=new int;
        *p1x=it.operator*().p1->x/wymiarSiatki;
        *p1y=it.operator*().p1->y/wymiarSiatki;
        *p2x=it.operator*().p2->x/wymiarSiatki;
        *p2y=it.operator*().p2->y/wymiarSiatki;
        *p1xpom=*p1x;
        *p1ypom=*p1y;
        *p2xpom=*p2x;
        *p2ypom=*p2y;
        for(int i=0;i<(abs(it.operator*().p1->x-it.operator*().p2->x)+abs(it.operator*().p1->y-it.operator*().p2->y))/wymiarSiatki;i++){
            if(*kierunek24==true){
                if(it.operator*().p1->x>it.operator*().p2->x){
                    br.setColor(Qt::gray);
                    siatka[*p1xpom][*p1y].setBrush(br);
                    siatka[*p1xpom][*p1y].update();
                    *p1xpom=*p1xpom-1;
                }else if(it.operator*().p1->x<it.operator*().p2->x){
                    br.setColor(Qt::gray);
                    siatka[*p2xpom][*p2y].setBrush(br);
                    siatka[*p2xpom][*p2y].update();
                    *p2xpom=*p2xpom-1;
                }

            }
            if (*kierunek24==false){
                if(it.operator*().p1->y>it.operator*().p2->y){
                    br.setColor(Qt::gray);
                    siatka[*p1x][*p1ypom].setBrush(br);
                    siatka[*p1x][*p1ypom].update();
                    *p1ypom=*p1ypom-1;
                }else if(it.operator*().p1->y<it.operator*().p2->y){
                    br.setColor(Qt::gray);
                    siatka[*p2x][*p2ypom].setBrush(br);
                    siatka[*p2x][*p2ypom].update();
                    *p2ypom=*p2ypom-1;
                }
            }
        }
        delete kierunek24;
        delete p1x;
        delete p2x;
        delete p1y;
        delete p2y;
        delete p1xpom;
        delete p2xpom;
        delete p1ypom;
        delete p2ypom;

    }

/*
    list <linia> :: iterator it;
    // UŻYWANIE ITERATORA
    it=listaUlic.begin();
    qDebug()<<it->p1->x<< " "<< it->p2->x;
    qDebug()<<it->p1->y<< " "<< it->p2->y;
    it++;
    qDebug()<<it->p1->x<< " "<< it->p2->x;
    qDebug()<<it->p1->y<< " "<< it->p2->y;
    it++;
    qDebug()<<it->p1->x<< " "<< it->p2->x;
    qDebug()<<it->p1->y<< " "<< it->p2->y;

    */
}


void Mapa::wyswietl()
{


}

void Mapa::tworz_sygnalizatory()
{
    listaSygnalizatorow.push_back(new sygnalizator(&siatka[15][10],this));
    listaSygnalizatorow.push_back(new sygnalizator(&siatka[20][5],this));
}

void Mapa::sprawdzPrzypadkiszczegolne() // chuj wie czemu to nie działa.
{
    //qDebug()<<"przy[adkiszczegolne";
    list <linia> :: iterator it;
    it=listaUlic.begin();
    for(it;it!=listaUlic.end();it++){

        list <linia> :: iterator it2;
        it2=listaUlic.begin();
        for(it2;it2!=listaUlic.end();it2++){
            if( it.operator*().p1==it2.operator*().p1||it.operator*().p1==it2.operator*().p2){
                pomocniczaListaUlic.push_front(&it.operator*());
            }
        }
/*
        if(pom==0){

            list <linia> :: iterator it2;
            it2=listaUlic.begin();
            for(it2;it2!=listaUlic.end();it2++){
                if( it.operator*().p1==it2.operator*().p1||it.operator*().p1==it2.operator*().p2){
                    pomocniczaListaUlic.push_front(&it.operator*());
                }
            }
        }
        else{

            list <linia> :: iterator it2;
            it2=listaUlic.begin();
            for(it2;it2!=listaUlic.end();it2++){
                if( it.operator*().p2==it2.operator*().p1||it.operator*().p2==it2.operator*().p2){
                    pomocniczaListaUlic.push_front(&it.operator*());
                }
            }
        }
*/

        //mam lista ulic ze skrzyzowania, coś z nimi robie:

        list <linia*>::iterator itLinia;
        itLinia=pomocniczaListaUlic.begin();
        Auto* autoPom;
        //qDebug()<<"okres";
        for(itLinia;itLinia!=pomocniczaListaUlic.end();itLinia++){
            //qDebug()<<"linia";
            list <Auto*> :: iterator itAuto;
            itAuto=itLinia.operator*()->listaAut.begin();
            for(itAuto;itAuto!=itLinia.operator*()->listaAut.end();itAuto++){


                if(itAuto.operator*()->getPunktDocelowy()==it.operator*().p1
                        &&itAuto.operator*()->obliczOdlegloscDoPunktuDocelowego()<itAuto.operator*()->getOdlegloscHamowaniaOdSkrzyzowania()
                        &&itAuto.operator*()->getPredkosc()==0)
                {
                    qDebug()<<"auto";
                    licznik++;
                    autoPom=itAuto.operator*();
                }

            }

        }

        if(licznik==(int)pomocniczaListaUlic.size() && licznik>2){
            if(wlaczIgnorowaniePrawej==true)autoPom->setIgnorujPrawaReke(1);
        }

        if(licznik>1)qDebug()<<licznik;

        pomocniczaListaUlic.clear();
        if(pom==0)pom=1;
        if(pom==1)pom=0;
        licznik=0;
        autoPom=NULL;
        delete autoPom;


    }
}
