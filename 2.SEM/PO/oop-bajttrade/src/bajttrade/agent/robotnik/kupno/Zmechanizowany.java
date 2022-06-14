package bajttrade.agent.robotnik.kupno;

import bajttrade.agent.robotnik.Robotnik;
import bajttrade.produkty.Narzedzia;
import bajttrade.produkty.Produkt;

import java.util.List;

public class Zmechanizowany extends Czyscioszek{
    private int liczba_narzedzi;

    public Zmechanizowany(int liczba_narzedzi) {
        this.liczba_narzedzi = liczba_narzedzi;
    }

    @Override
    public List<Produkt> kupuj(Robotnik robotnik) {
        List<Produkt> res = super.kupuj(robotnik);
        res.add(new Narzedzia(liczba_narzedzi));
        return res;
    }
}
