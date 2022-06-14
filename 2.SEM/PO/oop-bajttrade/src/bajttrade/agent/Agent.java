package bajttrade.agent;

import bajttrade.produkty.*;

import java.util.ArrayList;
import java.util.List;

public class Agent {
    protected int id;
    protected Jedzenie mojeJedzenie;
    protected List <Ubrania> mojeUbrania;
    protected List <Narzedzia> mojeNarzedzia;
    public Diamenty mojeDiamenty;
    protected List <ProgramyKomputerowe> mojeProgramy;

    public Agent(int id, Produkt... produkty){
        this.id = id;

    }

    public int getId() {
        return id;
    }

    public int getUbrania(){
        int ileUbran = 0;
        for(int i = 0; i < mojeUbrania.size(); i++){
            ileUbran += mojeUbrania.get(i).getIlosc();
        }
        return ileUbran;
    }

    public int getNarzedzia(){
        int ileNarzedzi = 0;
        for(int i = 0; i < mojeNarzedzia.size(); i++){
            ileNarzedzi += mojeNarzedzia.get(i).getIlosc() * mojeNarzedzia.get(i).getPoziomJakosci();
        }
        mojeNarzedzia.clear();
        return ileNarzedzi;
    }

    public void zuzyjUbrania(int tryb){
        if(tryb == 0){
            for(int j = 0; j < mojeUbrania.size(); j++){
                mojeUbrania.get(j).uzyj();
            }
        }
        else{
            boolean ubrany = false;
            int i = 0;
            double ileUbralam = 0;
            while(!ubrany){
                if(mojeUbrania.get(i).getIlosc() + ileUbralam > 100){
                    Ubrania pom = mojeUbrania.get(i).clone((int) (mojeUbrania.get(i).getIlosc() + ileUbralam - 100));
                    mojeUbrania.get(i).dodaj(-(mojeUbrania.get(i).getIlosc() + ileUbralam - 100));
                    mojeUbrania.get(i).uzyj();
                    mojeUbrania.add(pom);
                    ileUbralam = 100;
                }
                else{
                    mojeUbrania.get(i).uzyj();;
                    ileUbralam += mojeUbrania.get(i).getIlosc();
                }
                if(ileUbralam == 100){
                    ubrany = true;
                }
                i++;
            }

        }
        for(int j = 0; j < mojeUbrania.size(); j++){
            if(mojeUbrania.get(j).getPoziomJakosci() * mojeUbrania.get(j).getPoziomJakosci() ==
                    mojeUbrania.get(j).getZuzycie()){
                mojeUbrania.remove(j);
                j--;
            }
        }
    }

    public void dodajPrzedmiot(Produkt produkt){
        switch (produkt.toInt()){
            case 0:
                mojeJedzenie.dodaj(produkt.getIlosc());
            case 1:
                mojeUbrania.add((Ubrania) produkt);
            case 2:
                mojeNarzedzia.add((Narzedzia) produkt);
            case 4:
                mojeProgramy.add((ProgramyKomputerowe) produkt);
        }
    }
}
