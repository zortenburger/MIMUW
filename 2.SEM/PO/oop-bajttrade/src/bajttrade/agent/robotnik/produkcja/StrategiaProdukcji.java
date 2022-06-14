package bajttrade.agent.robotnik.produkcja;

import bajttrade.gielda.Gielda;
import bajttrade.produkty.*;

import java.util.ArrayList;
import java.util.List;

public abstract class StrategiaProdukcji {

    public StrategiaProdukcji() {
    }

    public List<Produkt> produkuj(int premiowany, int premia, int kara, int[] produktywnosc, Gielda gielda, int prog,
        List<ProgramyKomputerowe> programy){
        Produkt res = wybierz(premiowany, premia, kara, produktywnosc, gielda);
        if(res.toInt() == premiowany){
            kara += premia;
        }
        kara += 100;
        int ilosc = produktywnosc[res.toInt()] /100 * kara;
        if(ilosc < 0){
            return null;
        }
        res.dodaj(ilosc);
        if(res.getClass() == Narzedzia.class){
            return ((Narzedzia)res).ulepsz(programy);
        }
        if(res.getClass() == Ubrania.class){
            return ((Ubrania) res).ulepsz(programy);
        }
        if(res.getClass() == ProgramyKomputerowe.class){
            if(prog > 0){
                ((ProgramyKomputerowe) res).setPoziomZaawansowania(prog);
            }
            List<Produkt> res1 = new ArrayList<>();
            res1.add(res);
            return res1;
        }
        List<Produkt> res1 = new ArrayList<>();
        res1.add(res);
        return res1;
    }
    public abstract Produkt wybierz(int premiowany, int premia, int kara, int[] produktywnosc, Gielda gielda);
}
