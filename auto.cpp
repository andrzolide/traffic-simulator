#include "auto.h"
#include <QDebug>
#include <QTimer>
#include <math.h>
#include <iterator>
#include <cstdlib>
#include <ctime>

#include "mapa.h"
#include "linia.h"
#include "sygnalizator.h"



double Auto::getPredkosc() //const
{
    return predkosc;
}

int Auto::getOdlegloscHamowaniaOdSkrzyzowania()
{
    return odlegloscHamowaniaOdSkrzyzowania;
}

void Auto::setIgnorujPrawaReke(bool value)
{
    ignorujPrawaReke = value;
}

punkt *Auto::getPunktDocelowy() const
{
    return punktDocelowy;
}

int Auto::getRozmiarAutaX() const
{
    return rozmiarAutaX;
}

void Auto::sprawdzCzyMozeszWjechacNaLinie()
{
    if(predkosc<predkoscJazdyNaSkrzyzowaniu+przyspieszenie) {
        list<Auto*>::iterator itAuto;
        itAuto=zakrety.front()->listaAut.begin();
        for(itAuto;itAuto!=zakrety.front()->listaAut.end();itAuto++){
            if(itAuto.operator*()->kierunek==obliczKierunekZakretu() && itAuto.operator*()->obliczOdlegloscDoPunktuWyjsciowego()<drogaHamowania + rozmiarAutaX+odlegloscHamowaniaOdSamochodu){
                drogaWolna=false;
            }
        }

    }
}

Auto::Auto(Mapa *map):QGraphicsRectItem(){
    qsrand(0);
    srand(time(NULL));
    //przepisanie wskaźnika do mapy
    mapa=map;
    //aktualne polozenie ustawiane jest na punkt poczatkowy trasy na sztywno
    punktWyjsciowy=map->listaUlic.begin()->p1;
    //ustawianie punktu docelowego na sztywno.
    punktDocelowy=map->listaUlic.begin()->p2;
    //przypisywana jest na sztywno aktualna linia. czyli pierwsza na trasie
    aktualnaLinia=&map->listaUlic.front();
    //obliczanie kierunku jazdy na podstawie tego jak ustawieni jesteśmy w stosunku do linii
    obliczKierunek();
    //samochod dodawany jest do aktualnej linii
    aktualnaLinia->listaAut.push_back(this);
    //wyswietlanie kierunku jazdy
    qDebug()<<"kierunek " <<kierunek;
    //na sztywno deklarowany jest wymiar samochodu. bedzie to pozniej podmienione na grafike
    setRect(-rozmiarAutaX/2,0,rozmiarAutaX,rozmiarAutaY);
    //ustawianie polozenia auta
    setPos(map->listaUlic.begin()->p1->x,map->listaUlic.begin()->p1->y);
    //ustawianie pierwszego zakrętu

    //paletka
    br=QBrush(Qt::SolidPattern);
    // dodanie poczatkowej listy mozliwych zakretow
    list <linia> :: iterator it2;
    it2=mapa->listaUlic.begin();
    for(it2;it2!=mapa->listaUlic.end();it2++){

        if( (it2.operator*().p1==punktDocelowy || it2.operator*().p2==punktDocelowy)&& &it2.operator*()!=aktualnaLinia ){
            uliceZaSkrzyzowaniem.push_front(&it2.operator*());
        }

    }

    //sprawdzam czy sa jakies drogi do wylosowania
    if(uliceZaSkrzyzowaniem.empty()){ // jezeli nie ma takiego czegoś to wypierdalamy to auto.
        usunAutoZLinii();
        delete this;return;
    }
    //losuj droge.
    int * size=new int;
    *size=(int)uliceZaSkrzyzowaniem.size();
    int *pom=new int;
    *pom= qrand() % (*size) + 0;
    list <linia*> :: iterator it3;
    it3=uliceZaSkrzyzowaniem.begin();
    for(int i=0;i<*pom;i++){
        it3++;
    }
    zakrety.push_front(it3.operator*());
    delete pom;
    delete size;

    //obsługa timera
    QTimer *timer= new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(jedz()));
    timer->start(40);
}

bool Auto::obliczKierunek()//brakuje tutaj niestety odniesienia do tego w jakim położeniu aktualnie jesteśmy.!!!!!!!!!
{
    if(punktWyjsciowy->x==punktDocelowy->x && punktWyjsciowy->y > punktDocelowy->y){
        kierunek=1;
        return true;
    }
    else if(punktWyjsciowy->x==punktDocelowy->x && punktWyjsciowy->y < punktDocelowy->y){
        kierunek=3;
        return true;
    }
    else if(punktWyjsciowy->y==punktDocelowy->y && punktWyjsciowy->x > punktDocelowy->x){
        kierunek=4;
        return true;
    }
    else if(punktWyjsciowy->y==punktDocelowy->y && punktWyjsciowy->x < punktDocelowy->x){
        kierunek=2;
        return true;
    }
    return false;
}

//czemu to dziala dobrze?
int Auto::obliczKierunekZakretu()
{

    int kier;

    if(zakrety.front()->p1->x==zakrety.front()->p2->x && (zakrety.front()->p1->y > punktDocelowy->y ||zakrety.front()->p2->y > punktDocelowy->y)  ){
        kier=3;
    }
    else if(zakrety.front()->p1->x==zakrety.front()->p2->x && (zakrety.front()->p1->y < punktDocelowy->y ||zakrety.front()->p2->y < punktDocelowy->y)  ){
        kier=1;
    }
    else if(zakrety.front()->p1->y==zakrety.front()->p2->y && (zakrety.front()->p1->x < punktDocelowy->x ||zakrety.front()->p2->x < punktDocelowy->x)  ){
        kier=4;
    }
    else if(zakrety.front()->p1->y==zakrety.front()->p2->y && (zakrety.front()->p1->x > punktDocelowy->x ||zakrety.front()->p2->x > punktDocelowy->x)  ){
        kier=2;
    }

    else kier=0;

    return kier;
}



void Auto::ustawNastepnaLinie()// ustawia nastepna linie która jest okreslona w zakretach.
{
    //usun auto z bierzacej linii
    usunAutoZLinii();
    //ustaw nowa linie
    aktualnaLinia=zakrety.front();
    //dodaj auto do nowej linii
    aktualnaLinia->listaAut.push_back(this);
    //qDebug()<<"dodano auto do liniio";
    //ustawianie punktu wyjcsciowego i wejsciowego.
    if(zakrety.front()->p1==punktWyjsciowy){
        punktDocelowy=zakrety.front()->p2;
       // qDebug()<<"punkt docelowy"<<punktDocelowy->x<<" "<<punktDocelowy->y;
    }else{
        punktDocelowy=zakrety.front()->p1;
        //qDebug()<<"punkt docelowy"<<punktDocelowy->x<<" "<<punktDocelowy->y;
    }
    //ususwanie zakrętu ktory przed chwila wykonalismy
    zakrety.pop_front();
    //obliczenie nowego kierunku
    obliczKierunek();
    //qDebug()<<"kierunek " <<kierunek;
    //wymiar samochodu.
    if(kierunek==1){
        setRect(2,-rozmiarAutaX/2,rozmiarAutaY,rozmiarAutaX);
    }
    else if(kierunek==2){
        setRect(-rozmiarAutaX/2,2,rozmiarAutaX,rozmiarAutaY);
    }
    else if(kierunek==3){
        setRect(-rozmiarAutaY-2,-rozmiarAutaX/2,rozmiarAutaY,rozmiarAutaX);
    }
    else if(kierunek==4){
        setRect(-rozmiarAutaX/2,-rozmiarAutaY-2,rozmiarAutaX,rozmiarAutaY);
    }
    //ustawianie polozenia auta -------!!!!!!!!!!! tutaj jeszcze trzeba bedzie to ogarnac zeby poruszal sie po dobrej stronie drogu.
    setPos(punktWyjsciowy->x,punktWyjsciowy->y);
}

void Auto::wylosujNastepnaLinie()
{
    //    srand(time(NULL));
    //zakrety.push_front(rand()%4+1);

    list <linia>::iterator it;
    it=mapa->listaUlic.begin();
    for(it; it!=mapa->listaUlic.end() ;it++){
        if(kierunek==1){

        }
        else if(kierunek==2){
            //if(it.operator*()->x()-this->x()-odlegloscHamowaniaOdSamochodu<drogaHamowania && it.operator*()->x()>this->x() )
           // {
                //drogaWolna=false;
           // }
        }
        else if(kierunek==3){

        }
        else if(kierunek==4){

        }
    }


}



double Auto::obliczDrogeHamowania(){
    double drogaHamowania;
    drogaHamowania=pow(predkosc,2)/2/przyspieszenie;
    return drogaHamowania;
}

void Auto::usunAutoZLinii()
{
    list <Auto*>::iterator it;
    it=aktualnaLinia->listaAut.begin();
    for(it; it!=aktualnaLinia->listaAut.end() ;it++){
        if(it.operator*()==this){
            aktualnaLinia->listaAut.erase(it);
            //qDebug()<<"usunieto auto z linioi";
        }
    }
}

void Auto::ustawKolor(Qt::GlobalColor color)
{

    br.setColor(color);
    this->setBrush(br);
    this->update();
}

void Auto::ruszSie(){
    if(kierunek==1){
        setPos(x(),y()-predkosc);
    }
    else if(kierunek==2){
       setPos(x()+predkosc,y());
    }
    else if(kierunek==3){
        setPos(x(),y()+predkosc);
    }
    else if(kierunek==4){
        setPos(x()-predkosc,y());
    }
    else return;
}

double Auto::obliczOdlegloscDoPunktuDocelowego()
{
    double odl = NULL;
    //pion
    if(aktualnaLinia->p1->x==aktualnaLinia->p2->x){
        odl= abs(this->y()-punktDocelowy->y);
    }
    //poziom
    if(aktualnaLinia->p1->y==aktualnaLinia->p2->y){
        odl= abs(this->x()-punktDocelowy->x);
    }

    return odl;
}

double Auto::obliczOdlegloscDoPunktuWyjsciowego()
{
    double odl = NULL;
    //pion
    if(aktualnaLinia->p1->x==aktualnaLinia->p2->x){
        odl= abs(this->y()-punktWyjsciowy->y);
    }
    //poziom
    if(aktualnaLinia->p1->y==aktualnaLinia->p2->y){
        odl= abs(this->x()-punktWyjsciowy->x);
    }

    return odl;
}

bool Auto::sprawdzCzyNieMaSygnalizatora(){

    list <sygnalizator*>::iterator it;
    it=mapa->listaSygnalizatorow.begin();
    for(it;it!=mapa->listaSygnalizatorow.end();it++){

        if(punktDocelowy==it.operator*()->getPkt_sygnalizatora()){
            if(kierunek==1){
                qDebug()<<"kier 1";
                list <element_sygnalizatora>::iterator it2;
                it2=it.operator*()->elementy_sygnalizatora.begin();
                for(it2;it2!=it.operator*()->elementy_sygnalizatora.end();it2++){
                    if(it2.operator*().strona==3){
                        if(it2.operator*().stan==false) return true;
                        else zieloneSwiatlo=true;
                    }
                }
            }
            if(kierunek==2){
                qDebug()<<"kier 2";
                list <element_sygnalizatora>::iterator it2;
                it2=it.operator*()->elementy_sygnalizatora.begin();
                for(it2;it2!=it.operator*()->elementy_sygnalizatora.end();it2++){
                    if(it2.operator*().strona==4){
                        if(it2.operator*().stan==false) return true;
                        else zieloneSwiatlo=true;
                    }
                }
            }
            if(kierunek==3){
                qDebug()<<"kier 3";
                list <element_sygnalizatora>::iterator it2;
                it2=it.operator*()->elementy_sygnalizatora.begin();
                for(it2;it2!=it.operator*()->elementy_sygnalizatora.end();it2++){
                    if(it2.operator*().strona==1){
                        if(it2.operator*().stan==false) return true;
                        else zieloneSwiatlo=true;
                    }
                }
            }
            if(kierunek==4){
                qDebug()<<"kier 4";
                list <element_sygnalizatora>::iterator it2;
                it2=it.operator*()->elementy_sygnalizatora.begin();
                for(it2;it2!=it.operator*()->elementy_sygnalizatora.end();it2++){
                    if(it2.operator*().strona==2){
                        if(it2.operator*().stan==false) return true;
                        else zieloneSwiatlo=true;
                    }
                }
            }
        }
    }


    return false;
}

void Auto::jedz()
{
    //qDebug()<<obliczOdlegloscDoPunktuDocelowego();
    if(ignorujPrawaReke==1){
        qDebug()<<"ignorujeeeeeeeeeeeeeeeeeeeeeee";
        if(obliczOdlegloscDoPunktuDocelowego()>100){ignorujPrawaReke=0;}
        //ustawKolor(Qt::black);
    }


    //dokoncznie 2 członu reguly prawej reki, do kierunku  3 i 1 trzeba wkleic kod i pozmianiec kilka zmiennych.
    //zagwarantować, na mapce gdzie sprawdzamy przypadki szczegolne, że p1 to jest unikalne skżyżowanie: zmodernizowac wyszukiwarke skrzyżowań
    // zagwarantować, że auto z priorytetem nie wjedzie na inna linie ktora jest w korku zapelniona.


    //ustaw drogę na wolną, zostanie ona zmieniona na false w trakcie dzialania funkcji jesli napotkany zostanie powod
    drogaWolna=true;
    czerwoneSwiatlo=false;
    zieloneSwiatlo=false;
    //wykonaj ruch z ustawionymi w poprzedniej petli parametrami
    ruszSie();
    //licze droge hamowania
    drogaHamowania=obliczDrogeHamowania();

    //------------------------------------------------------------------------------------------------------------------------------------------------
    // PRZED TOBA BLISKO JEST SAMOCHOD to hamuj.
    //------------------------------------------------------------------------------------------------------------------------------------------------
    list <Auto*>::iterator it;
    it=aktualnaLinia->listaAut.begin();
    for(it; it!=aktualnaLinia->listaAut.end() ;it++){
        if(it.operator*()==this){continue;}
        if(kierunek==4){
            if(abs(it.operator*()->x()-this->x())-odlegloscHamowaniaOdSamochodu<drogaHamowania && it.operator*()->x()<this->x() && it.operator*()->kierunek==4 )
            {
                drogaWolna=false;
            }
        }
        else if(kierunek==2){
            if(abs(it.operator*()->x()-this->x())-odlegloscHamowaniaOdSamochodu<drogaHamowania && it.operator*()->x()>this->x() && it.operator*()->kierunek==2 )
            {
                drogaWolna=false;
            }
        }
        else if(kierunek==3){
            if(abs(it.operator*()->y()-this->y())-odlegloscHamowaniaOdSamochodu<drogaHamowania && it.operator*()->y()>this->y() && it.operator*()->kierunek==3)
            {
                drogaWolna=false;
            }
        }
        else if(kierunek==1){
            if(abs(it.operator*()->y()-this->y())-odlegloscHamowaniaOdSamochodu<drogaHamowania && it.operator*()->y()<this->y() && it.operator*()->kierunek==1)
            {
                drogaWolna=false;
            }
        }
    }



    if(ignorujPrawaReke==1){
        goto ignorancja;
    }
    //--------------------------------------------------------------------------------------------------------------------------------------------------
    //ZBLIZASZ SIE DO SKRZYZOWANIA.
    //------------------------------------------------------------------------------------------------------------------------------------------------
    if((kierunek==2&&abs(x()-punktDocelowy->x)-odlegloscHamowaniaOdSkrzyzowania<drogaHamowania )  && (kierunek==2&&abs(x()-punktDocelowy->x)>rozmiarAutaX)){
        if(predkosc>predkoscJazdyNaSkrzyzowaniu) {drogaWolna=false;}


    czerwoneSwiatlo=sprawdzCzyNieMaSygnalizatora();
    if(czerwoneSwiatlo==true){
        drogaWolna=false;
    }


    sprawdzCzyMozeszWjechacNaLinie();

    // tutaj mozna pozmieniac i uzyc funkcji obliczkierunekzakretu.
        if(zakrety.front()->p1->x==zakrety.front()->p2->x && (zakrety.front()->p1->y > this->y() ||zakrety.front()->p2->y > this->y()  )  ){
            // jak skrecasz w prawo to nic sie nie dzieje
        }else
        {
            if(predkosc<predkoscJazdyNaSkrzyzowaniu+przyspieszenie) {
                list <linia*> :: iterator it;
                it=uliceZaSkrzyzowaniem.begin();
                //qDebug()<<"elooooooooooooooooooooooooooooo";
                for(it;it!=uliceZaSkrzyzowaniem.end();it++){
                    //it.operator*()->wyswietl();
                    //jezeli jest po prawej.
                    if(it.operator*()->p1->x==it.operator*()->p2->x&&zieloneSwiatlo==false){

                        //qDebug()<<obliczKierunekZakretu();
                        list <Auto*> ::iterator itAuto;
                        itAuto=it.operator*()->listaAut.begin();
                        //szukam auta ktorego droga hamowania jest mniejsza niz jego odleglosc od skrzyzowania +droga hamowania.
                        for(itAuto;itAuto!=it.operator*()->listaAut.end();itAuto++){
                            if(itAuto.operator*()->y()>this->y() && itAuto.operator*()->kierunek==1 && abs(itAuto.operator*()->y()-this->y())<itAuto.operator*()->drogaHamowania+odlegloscHamowaniaOdSamochodu*3){
                                drogaWolna=false;
                            }
                        }
                    }

                    if(obliczKierunekZakretu()==1 && it.operator*()->p1->y==it.operator*()->p2->y){

                        list <Auto*> ::iterator itAuto;
                        itAuto=it.operator*()->listaAut.begin();
                        //szukam auta ktorego droga hamowania jest mniejsza niz jego odleglosc od skrzyzowania +droga hamowania.
                        for(itAuto;itAuto!=it.operator*()->listaAut.end();itAuto++){
                            if( itAuto.operator*()->kierunek==4 && abs(itAuto.operator*()->x()-this->x())-drogaHamowania<itAuto.operator*()->drogaHamowania+odlegloscHamowaniaOdSamochodu*3){
                                if(itAuto.operator*()->obliczKierunekZakretu()!=3){
                                    drogaWolna=false;
                                }

                            }
                        }
                    }

                }
            }
        }
    }
    if((kierunek==4&&abs(x()-punktDocelowy->x)-odlegloscHamowaniaOdSkrzyzowania<drogaHamowania) && (kierunek==4&&abs(x()-punktDocelowy->x)>rozmiarAutaX)){

        czerwoneSwiatlo=sprawdzCzyNieMaSygnalizatora();
        if(czerwoneSwiatlo==true){
            drogaWolna=false;
        }

        //na razie po prostu każe mu sie zatrzymać.
        if(predkosc>predkoscJazdyNaSkrzyzowaniu) {drogaWolna=false;}

        sprawdzCzyMozeszWjechacNaLinie();

        if(zakrety.front()->p1->x==zakrety.front()->p2->x && (zakrety.front()->p1->y < this->y() ||zakrety.front()->p2->y < this->y()  )  ){

        }else
        {

        if(predkosc<predkoscJazdyNaSkrzyzowaniu+przyspieszenie) {
            list <linia*> :: iterator it;
            it=uliceZaSkrzyzowaniem.begin();
            //qDebug()<<"elooooooooooooooooooooooooooooo";
            for(it;it!=uliceZaSkrzyzowaniem.end();it++){
                //it.operator*()->wyswietl();
                //jezeli jest po prawej.
                if(it.operator*()->p1->x==it.operator*()->p2->x&&zieloneSwiatlo==false){
                    list <Auto*> ::iterator itAuto;
                    itAuto=it.operator*()->listaAut.begin();
                    //szukam auta ktorego droga hamowania jest mniejsza niz jego odleglosc od skrzyzowania +droga hamowania.
                    for(itAuto;itAuto!=it.operator*()->listaAut.end();itAuto++){
                        if(itAuto.operator*()->y()<this->y() && itAuto.operator*()->kierunek==3 && abs(itAuto.operator*()->y()-this->y())<itAuto.operator*()->drogaHamowania+odlegloscHamowaniaOdSamochodu*3){
                            drogaWolna=false;
                            //qDebug()<<"hamejeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
                        }
                    }
                }
                if(obliczKierunekZakretu()==3 && it.operator*()->p1->y==it.operator*()->p2->y){
                    list <Auto*> ::iterator itAuto;
                    itAuto=it.operator*()->listaAut.begin();
                    //szukam auta ktorego droga hamowania jest mniejsza niz jego odleglosc od skrzyzowania +droga hamowania.
                    for(itAuto;itAuto!=it.operator*()->listaAut.end();itAuto++){
                        if( itAuto.operator*()->kierunek==2 && abs(itAuto.operator*()->x()-this->x())-drogaHamowania<itAuto.operator*()->drogaHamowania+odlegloscHamowaniaOdSamochodu*3){
                            if(itAuto.operator*()->obliczKierunekZakretu()!=1){
                                drogaWolna=false;
                            }

                        }
                    }
                }

            }
        }
        }

        if(zakrety.front()->p1->x==zakrety.front()->p2->x)
        {

        }
    }

    if((kierunek==1&&abs(y()-punktDocelowy->y)-odlegloscHamowaniaOdSkrzyzowania<drogaHamowania) &&(kierunek==1&& abs(y()-punktDocelowy->y)>rozmiarAutaX)){

        czerwoneSwiatlo=sprawdzCzyNieMaSygnalizatora();
        if(czerwoneSwiatlo==true){
            drogaWolna=false;
        }

        if(predkosc>predkoscJazdyNaSkrzyzowaniu) {drogaWolna=false;}

        sprawdzCzyMozeszWjechacNaLinie();

        if(zakrety.front()->p1->y==zakrety.front()->p2->y && (zakrety.front()->p1->x > this->x() ||zakrety.front()->p2->x > this->x()  )  ){

        }else
        {
        if(predkosc<predkoscJazdyNaSkrzyzowaniu+przyspieszenie) {
            list <linia*> :: iterator it;
            it=uliceZaSkrzyzowaniem.begin();
            //qDebug()<<"elooooooooooooooooooooooooooooo";
            for(it;it!=uliceZaSkrzyzowaniem.end();it++){
                //it.operator*()->wyswietl();
                //jezeli jest po prawej.
                if(it.operator*()->p1->y==it.operator*()->p2->y&&zieloneSwiatlo==false){
                    list <Auto*> ::iterator itAuto;
                    itAuto=it.operator*()->listaAut.begin();
                    //szukam auta ktorego droga hamowania jest mniejsza niz jego odleglosc od skrzyzowania +droga hamowania.
                    for(itAuto;itAuto!=it.operator*()->listaAut.end();itAuto++){
                        if(itAuto.operator*()->x()>this->x()&& itAuto.operator*()->kierunek==4&& abs(itAuto.operator*()->x()-this->x())<itAuto.operator*()->drogaHamowania+odlegloscHamowaniaOdSkrzyzowania*3){
                            drogaWolna=false;
                            //qDebug()<<"hamejeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
                        }
                    }
                }

            }
        }
        }

        if(zakrety.front()->p1->y==zakrety.front()->p2->y)
        {

        }

    }
    if((kierunek==3&&abs(y()-punktDocelowy->y)-odlegloscHamowaniaOdSkrzyzowania<drogaHamowania)  && ( kierunek==3 && abs(y()-punktDocelowy->y)>rozmiarAutaX)){
        if(predkosc>predkoscJazdyNaSkrzyzowaniu) {drogaWolna=false;}

        czerwoneSwiatlo=sprawdzCzyNieMaSygnalizatora();
        if(czerwoneSwiatlo==true){
            drogaWolna=false;
        }

        sprawdzCzyMozeszWjechacNaLinie();

        if(zakrety.front()->p1->y==zakrety.front()->p2->y && (zakrety.front()->p1->x < this->x() ||zakrety.front()->p2->x < this->x()  )  ){

        }else
        {

        if(predkosc<predkoscJazdyNaSkrzyzowaniu+przyspieszenie) {
            list <linia*> :: iterator it;
            it=uliceZaSkrzyzowaniem.begin();
            //qDebug()<<"elooooooooooooooooooooooooooooo";
            for(it;it!=uliceZaSkrzyzowaniem.end();it++){
                //it.operator*()->wyswietl();
                //jezeli jest po prawej.
                if(it.operator*()->p1->y==it.operator*()->p2->y&&zieloneSwiatlo==false){
                    list <Auto*> ::iterator itAuto;
                    itAuto=it.operator*()->listaAut.begin();
                    //szukam auta ktorego droga hamowania jest mniejsza niz jego odleglosc od skrzyzowania +droga hamowania.
                    for(itAuto;itAuto!=it.operator*()->listaAut.end();itAuto++){
                        if(itAuto.operator*()->x()<this->x()&& itAuto.operator*()->kierunek==2&& abs(itAuto.operator*()->x()-this->x())<itAuto.operator*()->drogaHamowania+odlegloscHamowaniaOdSkrzyzowania*3){
                            drogaWolna=false;
                            //qDebug()<<"hamejeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
                        }
                    }
                }

            }
        }
        }

        if(zakrety.front()->p1->y==zakrety.front()->p2->y)
        {

        }


        //podejmij decyzję czy trzeba się zatrzymywać więc przeszukaj linie na skrzyżowaniu w poszukiwaniu innych aut oraz przeszukaj sygnalzatory swietlne.



        // jesli jest sygnalizacja
            //zastosuj zasady dla sygnalizacji

        //jesli nie ma sygnalizacji
            //jesli jedziesz prosto
                //przeszukaj linie po prawej stronie ( jesli coś jest pobierz predkosc i porównaj twoj czas pojawienia sie na skr z jego czasem
                // mozna jeszcze uwzglednic ze moze przyspieszac.
            // jesli skrecasz w lewo
                // na pewno zwolnij
                // przeszukaj zarówno prawo tak samo jak powyżej ale również prosto
            // jesli sprecasz w prawo
                // tylko zwolnij.


    }

    ignorancja:

    //------------------------------------------------------------------------------------------------------------------------------------------------
    // JESTES NA KONCU LINII
    // dokładniej jeśli jesteś w odległości mneijszej lub równej niż połowa prędkości gdzie prędkość to długość pojedynczego skoku.
    //------------------------------------------------------------------------------------------------------------------------------------------------
    if(abs(x()-punktDocelowy->x)<=predkosc/2 && abs(y()-punktDocelowy->y)<=predkosc/2)
    {

        //qDebug()<<"-------------------------------------";
        //qDebug()<<"jestem na koncu linii";
        //ustawiam nowy punkt wyjsciowy
        punktWyjsciowy=punktDocelowy;
        uliceZaSkrzyzowaniem.clear();//czyszczenie listy ulic za zakretem
        //qDebug()<<"punkt wyjsciowy: "<< punktWyjsciowy->x<<" "<<punktWyjsciowy->y;
        //ustawiam nastepna linie korzystajaz z listy zakręty
        ustawNastepnaLinie();
        //wylosuj nowy zakręt. oczywiscie tylko wtedy kiery lista zakretow jest pusta;
        if( zakrety.empty()){

            //losowanie zakretu // musisz ten mechanizm udoskonalić.
            list <linia> :: iterator it;
            it=mapa->listaUlic.begin();
            //it++; // to powoduje za auto nigdy nie jedzie na sam poczatkowa trase.
            //list < linia *> linPom;
            // petla ktora przechodzi po wszystkich liniach ktore maja w sobie aktualny punkt docelowy odrzucajac linie aktualna.
            for(it;it!=mapa->listaUlic.end();it++){

                if( (it.operator*().p1==punktDocelowy || it.operator*().p2==punktDocelowy)&& &it.operator*()!=aktualnaLinia ){
                    //qDebug()<<it.operator*().p1->x/50<<it.operator*().p1->y/50;
                    //qDebug()<<it.operator*().p2->x/50<<it.operator*().p2->y/50;
                    uliceZaSkrzyzowaniem.push_front(&it.operator*());
                }

            }

            if(uliceZaSkrzyzowaniem.empty()){ // jezeli nie ma takiego czegoś to wypierdalamy to auto.
                usunAutoZLinii();
                delete this;return;
            }
            //losuj
            int * size=new int;
            *size=(int)uliceZaSkrzyzowaniem.size();
            int *pom=new int;

            //qDebug()<<"-------------------------------------rand num"<<*size;
            *pom= qrand() % (*size) + 0;
            //qDebug()<<*pom<<" "<<qrand()%*size<<" "<<rand()%*size;

            list <linia*> :: iterator it2;
            it2=uliceZaSkrzyzowaniem.begin();
            for(int i=0;i<*pom;i++){
                it2++;
            }
            //qDebug()<<" dodaje do zakretow";
            //qDebug()<<it2.operator*()->p1->x/50<< " "<<it2.operator*()->p1->y/50;
            //qDebug()<<it2.operator*()->p2->x/50<< " "<<it2.operator*()->p2->y/50;
            zakrety.push_front(it2.operator*());
            delete pom;
            delete size;
        }
    }


    //------------------------------------------------------------------------------------------------------------------------------------------------
    //KONCOWE OPERACJE
    //ustaw prędność w zależności od tego czy jest przed tobą przeszkoda czy nie
    //------------------------------------------------------------------------------------------------------------------------------------------------
    if(drogaWolna==true){
        if(predkosc< predkosc_max){
            predkosc=predkosc+przyspieszenie;
            ustawKolor(Qt::green);
        }
    }
    else if(drogaWolna==false){
       predkosc=predkosc-przyspieszenie;
       ustawKolor(Qt::red);
    }
    if(predkosc<0)predkosc=0;

}


