package bajttrade.agent.robotnik.nauka;

import bajttrade.gielda.Gielda;

public class Konserwatysta extends StrategiaKariery{
    public Konserwatysta(){}

    @Override
    public int zmienKariere(int aktualnaKariera, int id, Gielda gielda, int dni) {
        return aktualnaKariera;
    }
}
