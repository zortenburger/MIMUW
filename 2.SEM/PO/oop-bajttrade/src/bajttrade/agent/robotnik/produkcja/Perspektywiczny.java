package bajttrade.agent.robotnik.produkcja;

import bajttrade.gielda.Gielda;
import bajttrade.produkty.Produkt;

public class Perspektywiczny extends StrategiaProdukcji{
    private int historia_perspektywy_dni;

    public Perspektywiczny(int historia_perspektywy_dni) {
        this.historia_perspektywy_dni = historia_perspektywy_dni;
    }

    @Override
    public Produkt wybierz(int premiowany, int premia, int kara, int[] produktywnosc, Gielda gielda) {
        Produkt najlepszy = null;
        double wynik = 0;
        for(int i = 0; i < 5; i++){
            Produkt akt = Produkt.fromInt(i);
            double aktWynik = gielda.sredniaCena(i, 1) - gielda.sredniaCena(i, historia_perspektywy_dni);
            if(aktWynik >= wynik){
                wynik = aktWynik;
                najlepszy = akt;
            }
        }
        return najlepszy;
    }
}
