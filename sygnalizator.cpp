#include "sygnalizator.h"
#include <QTimer>
#include <iostream>
#include <list>
#include <iterator>
#include <QGraphicsRectItem>
#include <QDebug>

#include "punkt.h"
#include "linia.h"
#include "mapa.h"


using namespace std;


element_sygnalizatora::element_sygnalizatora(){
br=QBrush(Qt::SolidPattern);
}

punkt *sygnalizator::getPkt_sygnalizatora() const
{
    return pkt_sygnalizatora;
}

sygnalizator::sygnalizator(punkt *pkt_sygn, Mapa *map)
{
    qDebug()<<"siema";
    mapa=map;
    pkt_sygnalizatora=pkt_sygn;
    list <linia> :: iterator it2;
    it2=mapa->listaUlic.begin();
    for(it2;it2!=mapa->listaUlic.end();it2++){
        if( pkt_sygnalizatora==it2.operator*().p1){
            if( it2.operator*().p1->x>it2.operator*().p2->x){
                dodajSygnalizator(4, it2.operator*().p1);
            }
            if( it2.operator*().p1->x<it2.operator*().p2->x){
                dodajSygnalizator(2, it2.operator*().p1);
            }
            if( it2.operator*().p1->y>it2.operator*().p2->y){
                dodajSygnalizator(1, it2.operator*().p1);
            }
            if( it2.operator*().p1->y<it2.operator*().p2->y){
                dodajSygnalizator(3, it2.operator*().p1);
            }
        }
        if( pkt_sygnalizatora==it2.operator*().p2){
            if( it2.operator*().p1->x>it2.operator*().p2->x){
                dodajSygnalizator(2, it2.operator*().p2);
            }
            if( it2.operator*().p1->x<it2.operator*().p2->x){
                dodajSygnalizator(4, it2.operator*().p2);
            }
            if( it2.operator*().p1->y>it2.operator*().p2->y){
                dodajSygnalizator(3, it2.operator*().p2);
            }
            if( it2.operator*().p1->y<it2.operator*().p2->y){
                dodajSygnalizator(1, it2.operator*().p2);
            }
        }
    }
    //ify w zaleznosci od tego jakie to jest skrzyzowanie czyli ile ulic dochodzi to tego samego punktu.
    if(elementy_sygnalizatora.size()==2){

    }
    if(elementy_sygnalizatora.size()==3){

    }
    if(elementy_sygnalizatora.size()==4){
        qDebug()<<"siemasz";
        list <element_sygnalizatora> :: iterator it;
        it=elementy_sygnalizatora.begin();
        //tutaj jeśli pierwszy raz odpalasz funkcje to musi nastąpić inicjacja czyli pierwsze przypiasnie stanu
        //ewentualnie mozna zrobić to gdzies indziej np w konstruktorze czy cos.
        //trzeba tez gdzies zrobić wyswietlanie tego kolorku w zaleznosci od stanu.
        for(it; it!=elementy_sygnalizatora.end();it++){
            if(it.operator*().strona==2){
                qDebug()<<"elgoo";
                it.operator*().stan=true;
                ustawKolor(Qt::green,&it.operator*());
            }
            else if(it.operator*().strona==4){
                it.operator*().stan=true;
                ustawKolor(Qt::green,&it.operator*());
            }else{
                ustawKolor(Qt::red,&it.operator*());
            }
        }
    }
    QTimer *timer= new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(zmiana()));
    timer->start(co_ile_zmiana);
}

void sygnalizator::ustawKolor(Qt::GlobalColor color, element_sygnalizatora *elem)
{
    elem->br.setColor(color);
    elem->element->setBrush(elem->br);
    elem->element->setBrush(elem->br);
    elem->element->update();
}

void sygnalizator::dodajSygnalizator(int kierunek, punkt* pkt)
{
    element_sygnalizatora* elem=new element_sygnalizatora();
    elem->stan=0;
    elem->strona=kierunek;
    elem->element=new QGraphicsRectItem();
    //polozenie recta w zaleznosci od kierynku
    if(kierunek==1){
       elem->element->setRect(pkt->x-wymiar_sygnalizatora/2-35,pkt->y-wymiar_sygnalizatora/2-45,wymiar_sygnalizatora,wymiar_sygnalizatora);
    }
    if(kierunek==2){
       elem->element->setRect(pkt->x-wymiar_sygnalizatora/2+45,pkt->y-wymiar_sygnalizatora/2-35,wymiar_sygnalizatora,wymiar_sygnalizatora);
    }
    if(kierunek==3){
       elem->element->setRect(pkt->x-wymiar_sygnalizatora/2+35,pkt->y-wymiar_sygnalizatora/2+45,wymiar_sygnalizatora,wymiar_sygnalizatora);
    }
    if(kierunek==4){
       elem->element->setRect(pkt->x-wymiar_sygnalizatora/2-45,pkt->y-wymiar_sygnalizatora/2+35,wymiar_sygnalizatora,wymiar_sygnalizatora);
    }

    mapa->scena->addItem(elem->element);

    this->elementy_sygnalizatora.push_back(*elem);
}

void sygnalizator::zmiana()
{
    //ify w zaleznosci od tego jakie to jest skrzyzowanie czyli ile ulic dochodzi to tego samego punktu.
    if(elementy_sygnalizatora.size()==2){

    }
    if(elementy_sygnalizatora.size()==3){

    }
    if(elementy_sygnalizatora.size()==4){
        list <element_sygnalizatora> :: iterator it;
        it=elementy_sygnalizatora.begin();
        //tutaj jeśli pierwszy raz odpalasz funkcje to musi nastąpić inicjacja czyli pierwsze przypiasnie stanu
        //ewentualnie mozna zrobić to gdzies indziej np w konstruktorze czy cos.
        //trzeba tez gdzies zrobić wyswietlanie tego kolorku w zaleznosci od stanu.
        for(it; it!=elementy_sygnalizatora.end();it++){
            if(it.operator*().stan==false){
                it.operator*().stan=true;
                ustawKolor(Qt::green,&it.operator*());
            }
            else if(it.operator*().stan==true){
                it.operator*().stan=false;
                ustawKolor(Qt::red,&it.operator*());
            }
        }
    }
}
