package bajttrade.agent.spekulant;

import bajttrade.gielda.Gielda;
import bajttrade.produkty.*;

import java.util.ArrayList;
import java.util.List;

public class Sredni extends Spekulant{
    private int historia_spekulanta_sredniego;
    public Sredni(int historia_spekulanta_sredniego, int id, Produkt... produkty) {
        super(id, produkty);
        this.historia_spekulanta_sredniego = historia_spekulanta_sredniego;
    }

    @Override
    public List<Oferta> przygotujOfertySprzedazy(Gielda gielda) {
        List<Oferta> res = new ArrayList<>();
        //Jedzenie
        if(mojeJedzenie.getIlosc() > 0){
            double srednia = 0;
            for(int i = 1; i <= historia_spekulanta_sredniego; i++){
                srednia += gielda.sredniaCena(0, i);
            }
            srednia = srednia / historia_spekulanta_sredniego;
            srednia = 1.1 * srednia;
            res.add(new Oferta(mojeJedzenie, id, srednia));
        }
        //Ubrania
        if(getUbrania() > 0){
            double srednia = 0;
            for(int i = 1; i <= historia_spekulanta_sredniego; i++){
                srednia += gielda.sredniaCena(1, i);
            }
            srednia = srednia / historia_spekulanta_sredniego;
            srednia = 1.1 * srednia;
            for(int i = 0; i < mojeUbrania.size(); i++){
                res.add(new Oferta(mojeUbrania.get(i), id, srednia));
            }
        }
        //Narzedzia
        if(getNarzedzia() > 0){
            double srednia = 0;
            for(int i = 1; i <= historia_spekulanta_sredniego; i++){
                srednia += gielda.sredniaCena(2, i);
            }
            srednia = srednia / historia_spekulanta_sredniego;
            srednia = 1.1 * srednia;
            for(int i = 0; i < mojeNarzedzia.size(); i++){
                res.add(new Oferta(mojeNarzedzia.get(i), id, srednia));
            }
        }
        //Programy
        if(mojeProgramy.size() > 0){
            double srednia = 0;
            for(int i = 1; i <= historia_spekulanta_sredniego; i++){
                srednia += gielda.sredniaCena(4, i);
            }
            srednia = srednia / historia_spekulanta_sredniego;
            srednia = 1.1 * srednia;
            for(int i = 0; i < mojeProgramy.size(); i++){
                res.add(new Oferta(mojeProgramy.get(i), id, srednia));
            }
        }
        return res;
    }

    @Override
    public List<Oferta> przygotujOfertyKupna(Gielda gielda) {
        List<Oferta> res = new ArrayList<>();
        //Jedzenie
        double srednia = 0;
        for(int i = 1; i <= historia_spekulanta_sredniego; i++){
            srednia += gielda.sredniaCena(0, i);
        }
        srednia = srednia / historia_spekulanta_sredniego;
        if(mojeJedzenie.getIlosc() > 0){
            srednia = 0.9 * srednia;
            res.add(new Oferta(new Jedzenie(100), id, srednia));
        }
        else{
            srednia = 0.95 * srednia;
            res.add(new Oferta(new Jedzenie(100), id, srednia));
        }
        //Ubrania
        srednia = 0;
        for(int i = 1; i <= historia_spekulanta_sredniego; i++){
            srednia += gielda.sredniaCena(1, i);
        }
        srednia = srednia / historia_spekulanta_sredniego;
        if(getUbrania() > 0){
            srednia = 0.9 * srednia;
            res.add(new Oferta(new Ubrania(100), id, srednia));
        }
        else {
            srednia = 0.95 * srednia;
            res.add(new Oferta(new Ubrania(100), id, srednia));
        }
        //Narzedzia
        srednia = 0;
        for(int i = 1; i <= historia_spekulanta_sredniego; i++){
            srednia += gielda.sredniaCena(2, i);
        }
        srednia = srednia / historia_spekulanta_sredniego;
        if(getNarzedzia() > 0){
            srednia = 0.9 * srednia;
            res.add(new Oferta(new Narzedzia(100), id, srednia));
        }
        else {
            srednia = 0.95 * srednia;
            res.add(new Oferta(new Narzedzia(100), id, srednia));
        }
        //Programy
        srednia = 0;
        for(int i = 1; i <= historia_spekulanta_sredniego; i++){
            srednia += gielda.sredniaCena(4, i);
        }
        srednia = srednia / historia_spekulanta_sredniego;
        if(mojeProgramy.size() > 0){
            srednia = 0.9 * srednia;
            res.add(new Oferta(new ProgramyKomputerowe(100), id, srednia));
        }
        else{
            srednia = 0.95 * srednia;
            res.add(new Oferta(new ProgramyKomputerowe(100), id, srednia));
        }
        return res;
    }
}
