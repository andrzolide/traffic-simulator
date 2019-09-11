#ifndef AUTO_H
#define AUTO_H
#include <QGraphicsRectItem>
#include <iostream>
#include <list>
#include <iterator>
#include <QObject>

#include "punkt.h"
#include "linia.h"
#include "mapa.h"

class Auto :public QObject,public QGraphicsRectItem
{
Q_OBJECT
private:

    /* KIERUNKI
            1
         4     2
            3
    */

    int kierunek;
    Mapa * mapa; // wskaźnik do aktualnej mapy po któRej jezdzza wszystkie auta. Dzięki niemu mamy dostęp do wektora linii, sygnalizacji, itd.
    double predkosc=0;
    double predkosc_max=5; //parametr jak niżej bezpiecznie 10
    double przyspieszenie=0.3;//parametr ktory trzeba będzie zrobić tak aby można go było wprowadzać czy coś takiego albo modyfikować. bezpiecznie 0.5
    double predkoscJazdyNaSkrzyzowaniu=predkosc_max+przyspieszenie;//jak chcemu zeby auta w ogoe nie zwalniały na skrzyzowaniu to ustawiamy to na: predkosc_max+przyspieszenie
    int rozmiarAutaX=30;//potrzebne to jest to tegy aby auto było w środku swojej tekstury.
    int rozmiarAutaY=15;
    double drogaHamowania=0;
    double czasHamowania=0;
    bool drogaWolna=true;
    bool ignorujPrawaReke=0;
    int odlegloscHamowaniaOdSkrzyzowania=50;
    int odlegloscHamowaniaOdSamochodu=50;
    punkt *punktWyjsciowy; //polozenie to w sumie mozna wywalić bo przecież dziedziczymy z qgraphics rect item
    punkt *punktDocelowy;
    linia *aktualnaLinia; //wskaźnik do aktualnej ninii na której znajduje się samochód. Potrzebny jest do tego aby uzyskać dostęp do listy aut znajdujących się nadanej linii.
    list <linia*> zakrety;
    list <linia*> uliceZaSkrzyzowaniem;// lista zakrętów które będa wykonywać się po kolei. Jeśli jest pusta to kolejne zakręty są losowane w momencie dotarcia do ostatniego znanego zakrętu.
    //linia aktualna_linia;
    QBrush br;
    bool czerwoneSwiatlo=false;
    bool zieloneSwiatlo=false;

public:


    Auto(Mapa *map);

    bool obliczKierunek();

    int obliczKierunekZakretu();

    void ustawNastepnaLinie();

    void wylosujNastepnaLinie();

    double obliczDrogeHamowania();

    double obliczCzasHamowania();

    void usunAutoZLinii();

    void ustawKolor(Qt::GlobalColor color);

    void jestemNaKoncuLinii();

    void ruszSie();

    double obliczOdlegloscDoPunktuDocelowego();

    double obliczOdlegloscDoPunktuWyjsciowego();

    double getPredkosc() ;//const;

    int getOdlegloscHamowaniaOdSkrzyzowania();

    void setIgnorujPrawaReke(bool value);

    punkt *getPunktDocelowy() const;

    int getRozmiarAutaX() const;

    void sprawdzCzyMozeszWjechacNaLinie();

    bool sprawdzCzyNieMaSygnalizatora();

public slots:

    void jedz();

};


#endif // AUTO_H
