package bajttrade.agent.spekulant;

import bajttrade.gielda.Gielda;
import bajttrade.produkty.*;

import java.util.ArrayList;
import java.util.List;

public class Wypukly extends Spekulant{
    public Wypukly(int id, Produkt... produkty) {
        super(id, produkty);
    }

    private int isWypukla(int produkt, Gielda gielda){
        double jedenDzienTemu = gielda.sredniaCena(produkt,1);
        double dwaDniTemu = gielda.sredniaCena(produkt,2);
        double trzyDniTemu = gielda.sredniaCena(produkt,3);
        if(jedenDzienTemu-dwaDniTemu > dwaDniTemu - trzyDniTemu){
            return 1;
        }
        else if(jedenDzienTemu-dwaDniTemu == dwaDniTemu - trzyDniTemu){
            return 0;
        }
        else{
            return -1;
        }
    }
    @Override
    public List<Oferta> przygotujOfertySprzedazy(Gielda gielda) {
        List<Oferta> res = new ArrayList<>();
        //Jedzenie
        if(mojeJedzenie.getIlosc() > 0 && isWypukla(0, gielda) < 0){
            double srednia = gielda.sredniaCena(0, 1);
            srednia = 1.1 * srednia;
            res.add(new Oferta(mojeJedzenie, id, srednia));
        }
        //Ubrania
        if(getUbrania() > 0 && isWypukla(1, gielda) < 0){
            double srednia = gielda.sredniaCena(1, 1);
            srednia = 1.1 * srednia;
            for(int i = 0; i < mojeUbrania.size(); i++){
                res.add(new Oferta(mojeUbrania.get(i), id, srednia));
            }
        }
        //Narzedzia
        if(getNarzedzia() > 0 && isWypukla(2, gielda) < 0){
            double srednia = gielda.sredniaCena(2, 1);
            srednia = 1.1 * srednia;
            for(int i = 0; i < mojeNarzedzia.size(); i++){
                res.add(new Oferta(mojeNarzedzia.get(i), id, srednia));
            }
        }
        //Programy
        if(mojeProgramy.size() > 0 && isWypukla(4, gielda) < 0){
            double srednia = gielda.sredniaCena(4, 1);
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
        if(isWypukla(0, gielda) > 0){
            double srednia = gielda.sredniaCena(0, 1);
            srednia = 0.9 * srednia;
            res.add(new Oferta(new Jedzenie(100), id, srednia));
        }
        //Ubrania
        if(isWypukla(1, gielda) > 0){
            double srednia = gielda.sredniaCena(1, 1);
            srednia = 0.9 * srednia;
            res.add(new Oferta(new Ubrania(100), id, srednia));
        }
        //Narzedzia
        if(isWypukla(2, gielda) > 0){
            double srednia = gielda.sredniaCena(2, 1);
            srednia = 0.9 * srednia;
            res.add(new Oferta(new Narzedzia(100), id, srednia));
        }
        //Programy
        if(isWypukla(4, gielda) < 0){
            double srednia = gielda.sredniaCena(4, 1);
            srednia = 0.9 * srednia;
            res.add(new Oferta(new ProgramyKomputerowe(100), id, srednia));
        }
        return res;
    }
}
