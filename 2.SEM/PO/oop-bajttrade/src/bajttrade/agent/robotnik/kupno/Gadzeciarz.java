package bajttrade.agent.robotnik.kupno;

import bajttrade.agent.robotnik.Robotnik;
import bajttrade.produkty.Produkt;
import bajttrade.produkty.ProgramyKomputerowe;

import java.util.List;

public class Gadzeciarz extends Zmechanizowany{
    public Gadzeciarz(int liczba_narzedzi) {
        super(liczba_narzedzi);
    }

    @Override
    public List<Produkt> kupuj(Robotnik robotnik) {
        List<Produkt> res = super.kupuj(robotnik);
        int ilosc = 0;
        for (Produkt prod : robotnik.ofertySprzedazy){
            ilosc += prod.getIlosc();
        }
        res.add(new ProgramyKomputerowe(ilosc,1));
        return res;
    }
}

