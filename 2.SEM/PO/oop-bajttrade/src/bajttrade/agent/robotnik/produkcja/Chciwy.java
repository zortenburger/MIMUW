package bajttrade.agent.robotnik.produkcja;

import bajttrade.gielda.Gielda;
import bajttrade.produkty.Produkt;

public class Chciwy extends StrategiaProdukcji{

    public Chciwy() {
    }

    @Override
    public Produkt wybierz(int premiowany, int premia, int kara, int[] produktywnosc, Gielda gielda) {
        Produkt najlepszy = null;
        double wynik = 0;
        for(int i = 0; i < 5; i++){
            Produkt akt = Produkt.fromInt(i);
            double aktWynik = 0;
            int pom = kara + 100;
            if(i == premiowany){
                pom += premia;
            }
            int ilosc = produktywnosc[i] /100 * pom;
            if(ilosc > 0){
                aktWynik = ilosc * gielda.sredniaCena(i, 1);
            }
            if(aktWynik >= wynik){
                wynik = aktWynik;
                najlepszy = akt;
            }
        }
        return najlepszy;
    }
}
