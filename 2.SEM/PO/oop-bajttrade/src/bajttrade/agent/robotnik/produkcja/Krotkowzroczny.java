package bajttrade.agent.robotnik.produkcja;

import bajttrade.gielda.Gielda;
import bajttrade.produkty.Produkt;

public class Krotkowzroczny extends StrategiaProdukcji{

    public Krotkowzroczny() {
    }

    @Override
    public Produkt wybierz(int premiowany, int premia, int kara, int[] produktywnosc, Gielda gielda) {
        Produkt najlepszy = null;
        double wynik = 0;
        for(int i = 0; i < 5; i++){
            Produkt akt = Produkt.fromInt(i);
            double cena = gielda.sredniaCena(i, 1);
            if(cena >= wynik){
                wynik = cena;
                najlepszy = akt;
            }
        }
        return najlepszy;
    }
}
