package bajttrade.agent.spekulant;

import bajttrade.gielda.Gielda;
import bajttrade.produkty.*;

import java.util.ArrayList;
import java.util.List;

public class RegulujacyRynek extends Spekulant{
    public RegulujacyRynek(int id, Produkt... produkty) {
        super(id, produkty);
    }
    @Override
    public List<Oferta> przygotujOfertySprzedazy(Gielda gielda) {
        List<Oferta> res = new ArrayList<>();
        //Jedzenie
        if(mojeJedzenie.getIlosc() > 0){
            double srednia = gielda.sredniaCena(0, 1) * gielda.iloscCena2(0, 0)
                    / Math.max(gielda.iloscCena2(0, 1),1);
            srednia = 1.1 * srednia;
            res.add(new Oferta(mojeJedzenie, id, srednia));
        }
        //Ubrania
        if(getUbrania() > 0){
            double srednia = gielda.sredniaCena(1, 1) * gielda.iloscCena2(1, 0)
                    / Math.max(gielda.iloscCena2(1, 1),1);
            srednia = 1.1 * srednia;
            for(int i = 0; i < mojeUbrania.size(); i++){
                res.add(new Oferta(mojeUbrania.get(i), id, srednia));
            }
        }
        //Narzedzia
        if(getNarzedzia() > 0){
            double srednia = gielda.sredniaCena(2, 1) * gielda.iloscCena2(2, 0)
                    / Math.max(gielda.iloscCena2(2, 1),1);
            srednia = 1.1 * srednia;
            for(int i = 0; i < mojeNarzedzia.size(); i++){
                res.add(new Oferta(mojeNarzedzia.get(i), id, srednia));
            }
        }
        //Programy
        if(mojeProgramy.size() > 0){
            double srednia = gielda.sredniaCena(4, 1) * gielda.iloscCena2(4, 0)
                    / Math.max(gielda.iloscCena2(4, 1),1);;
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
        if(true){
            double srednia = gielda.sredniaCena(0, 1) * gielda.iloscCena2(0, 0)
                    / Math.max(gielda.iloscCena2(0, 1),1);
            srednia = 0.9 * srednia;
            res.add(new Oferta(new Jedzenie(100), id, srednia));
        }
        //Ubrania
        if(true){
            double srednia = gielda.sredniaCena(1, 1) * gielda.iloscCena2(1, 0)
                    / Math.max(gielda.iloscCena2(1, 1),1);
            srednia = 0.9 * srednia;
            res.add(new Oferta(new Ubrania(100), id, srednia));
        }
        //Narzedzia
        if(true){
            double srednia = gielda.sredniaCena(2, 1) * gielda.iloscCena2(2, 0)
                    / Math.max(gielda.iloscCena2(2, 1),1);
            srednia = 0.9 * srednia;
            res.add(new Oferta(new Narzedzia(100), id, srednia));
        }
        //Programy
        if(true){
            double srednia = gielda.sredniaCena(4, 1) * gielda.iloscCena2(4, 0)
                    / Math.max(gielda.iloscCena2(4, 1),1);
            srednia = 0.9 * srednia;
            res.add(new Oferta(new ProgramyKomputerowe(100), id, srednia));
        }
        return res;
    }
}