package bajttrade.agent.robotnik.produkcja;

import bajttrade.gielda.Gielda;
import bajttrade.produkty.Produkt;

import java.util.Random;

public class Losowy extends StrategiaProdukcji{

    public Losowy() {
    }

    @Override
    public Produkt wybierz(int premiowany, int premia, int kara, int[] produktywnosc, Gielda gielda) {
        Random rand = new Random();
        int p = rand.nextInt(5);
        Produkt res = Produkt.fromInt(p);
        return res;
    }
}
