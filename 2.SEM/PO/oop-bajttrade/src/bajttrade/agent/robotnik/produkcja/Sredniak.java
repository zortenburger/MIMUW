package bajttrade.agent.robotnik.produkcja;

import bajttrade.gielda.Gielda;
import bajttrade.produkty.Produkt;

public class Sredniak extends StrategiaProdukcji{
    private int historia_sredniej_produkcji;

    public Sredniak(int historia_sredniej_produkcji) {
        this.historia_sredniej_produkcji = historia_sredniej_produkcji;
    }

    @Override
    public Produkt wybierz(int premiowany, int premia, int kara, int[] produktywnosc, Gielda gielda) {
        Produkt najlepszy = null;
        double wynik = 0;
        for(int i = 0; i < 5; i++){
            Produkt akt = Produkt.fromInt(i);
            double aktWynik = 0;
            for(int j = 1; j <= historia_sredniej_produkcji; j++){
                aktWynik = Math.max(aktWynik, gielda.sredniaCena(i, j));
            }
            if(aktWynik >= wynik){
                wynik = aktWynik;
                najlepszy = akt;
            }
        }
        return najlepszy;
    }
}
