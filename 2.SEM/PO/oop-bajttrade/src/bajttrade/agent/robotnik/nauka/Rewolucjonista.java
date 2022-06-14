package bajttrade.agent.robotnik.nauka;

import bajttrade.gielda.Gielda;

public class Rewolucjonista extends StrategiaKariery{

    public Rewolucjonista() {
    }

    @Override
    public int zmienKariere(int aktualnaKariera, int id, Gielda gielda, int dni) {
        if(dni % 7 != 0 || dni == 0){
            return aktualnaKariera;
        }
        int n = Math.max(id % 17, 1);
        int najczestszy = 0;
        int wynik = 0;
        for(int i = 0; i < 5; i++){
            int aktualnyWynik = 0;
            for(int j = 1; j <= n; j++){
                aktualnyWynik += gielda.iloscCena(i, j);
            }
            if(aktualnyWynik >= wynik){
                wynik = aktualnyWynik;
                najczestszy = i;
            }
        }
        return najczestszy;
    }
}
