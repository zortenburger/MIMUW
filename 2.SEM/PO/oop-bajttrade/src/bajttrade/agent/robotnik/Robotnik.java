package bajttrade.agent.robotnik;

import bajttrade.agent.Agent;
import bajttrade.agent.robotnik.kariera.*;
import bajttrade.agent.robotnik.kupno.StrategiaKupna;
import bajttrade.agent.robotnik.nauka.*;
import bajttrade.agent.robotnik.praca.StrategiaPracy;
import bajttrade.agent.robotnik.produkcja.StrategiaProdukcji;
import bajttrade.gielda.Gielda;
import bajttrade.produkty.*;

import java.util.*;
import java.io.*;

public class Robotnik extends Agent {
    public boolean czyZyje;
    private boolean czyJadl;
    private int ileNieJadl;
    private int[] wektorProduktywnosci;
    private SciezkaKariery kariery[];
    private int aktualnaKariera;
    private final StrategiaKariery kariera;
    private final StrategiaKupna kupno;
    private final StrategiaPracy praca;
    private final StrategiaProdukcji produkcja;
    public List<Produkt> ofertySprzedazy;
    public List<Produkt> ofertyKupna;


    public Robotnik(int id, int poziom, int jedzenie, int ubrania, int narzedzia, int diamenty, int programy,
                    SciezkaKariery aktualnaKariera, StrategiaKariery kariera, StrategiaKupna kupno,
                    StrategiaPracy praca, StrategiaProdukcji produkcja, Produkt... produkty) {
        super(id, produkty);
        this.czyZyje = true;
        this.czyJadl = false;
        this.ileNieJadl = 0;
        this.wektorProduktywnosci = new int[5];
        this.wektorProduktywnosci[0] = jedzenie;
        this.wektorProduktywnosci[1] = ubrania;
        this.wektorProduktywnosci[2] = narzedzia;
        this.wektorProduktywnosci[3] = diamenty;
        this.wektorProduktywnosci[4] = programy;
        this.kariery = new SciezkaKariery[5];
        this.kariery[0] = new Rolnik();
        this.kariery[1] = new Rzemieslnik();
        this.kariery[2] = new Inzynier();
        this.kariery[3] = new Gornik();
        this.kariery[4] = new Programista();
        this.aktualnaKariera = aktualnaKariera.toInt();
        this.kariery[this.aktualnaKariera].setPoziom(poziom);
        this.kariera = kariera;
        this.kupno = kupno;
        this.praca = praca;
        this.produkcja = produkcja;
    }

    private void umrzyj(){
        czyZyje = false;
    }

    private void zjedz(boolean czySieUczy){
        if(czySieUczy){
            this.czyJadl = true;
            this.ileNieJadl = 0;
        }
        else{
            if(mojeJedzenie.getIlosc() > 100){
                ileNieJadl = 0;
            }
            else{
                ileNieJadl++;
            }
            mojeJedzenie.dodaj((-1) * Math.min(100, mojeJedzenie.getIlosc()));
            if(ileNieJadl == 3){
                umrzyj();
            }
        }
    }

    public void uczSie(Gielda gielda){
        int pop = aktualnaKariera;
        aktualnaKariera = kariera.zmienKariere(this.aktualnaKariera, this.id, gielda, gielda.getTura());
        if(pop == aktualnaKariera){
            kariery[aktualnaKariera].uczSie();
        }
        zjedz(true);
    }

    public void pracuj(Gielda gielda, int kara){
        int prog = 0;
        if(aktualnaKariera == 4){
            prog = kariery[aktualnaKariera].getPoziom();
        }
        this.ofertySprzedazy = produkcja.produkuj(aktualnaKariera, kariery[aktualnaKariera].premia(),
                kara + getNarzedzia(), wektorProduktywnosci, gielda, prog, mojeProgramy);
        if(ofertySprzedazy.get(0).getClass() == Diamenty.class){
            mojeDiamenty.dodaj(ofertySprzedazy.get(0).getIlosc());
            ofertySprzedazy = null;
        }
        this.ofertyKupna = kupno.kupuj(this);
    }

    public void przezyjDzien(Gielda gielda){
        if(!czyZyje){
            return;
        }
        czyJadl = false;
        int kara = 0;
        if(ileNieJadl == 1){
            kara -= 100;
        }
        if(ileNieJadl == 2){
            kara -= 300;
        }
        int mojeUbrania = getUbrania();
        if(mojeUbrania < 100){
            kara -= gielda.getKara_za_brak_ubran();
            zuzyjUbrania(0);
        }
        else{
            zuzyjUbrania(1);
        }
        czyJadl = false;
        if(praca.decyduj(mojeDiamenty.getIlosc(), gielda)){
            uczSie(gielda);
        }
        else{
            pracuj(gielda, kara);
        }
    }

    public void zakonczDzien(){
        if(!czyJadl){
            zjedz(false);
        }
    }

}
