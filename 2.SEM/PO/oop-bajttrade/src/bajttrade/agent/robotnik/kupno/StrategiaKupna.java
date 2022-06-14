package bajttrade.agent.robotnik.kupno;

import bajttrade.agent.robotnik.Robotnik;
import bajttrade.produkty.Produkt;

import java.util.List;

public abstract class StrategiaKupna {

    public StrategiaKupna() {
    }
    public abstract List<Produkt> kupuj(Robotnik robotnik);
}
