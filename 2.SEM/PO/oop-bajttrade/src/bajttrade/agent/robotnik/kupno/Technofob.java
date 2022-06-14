package bajttrade.agent.robotnik.kupno;

import bajttrade.agent.robotnik.Robotnik;
import bajttrade.produkty.Jedzenie;
import bajttrade.produkty.Produkt;

import java.util.ArrayList;
import java.util.List;

public class Technofob extends StrategiaKupna{

    public Technofob() {
    }

    @Override
    public List<Produkt> kupuj(Robotnik robotnik) {
        List<Produkt> res = new ArrayList<>();
        res.add(new Jedzenie(100));
        return res;
    }
}
