package bajttrade.agent.robotnik.praca;

import bajttrade.gielda.Gielda;

public class Okresowy extends StrategiaPracy{
    private int okresowosc_nauki;

    public Okresowy(int okresowosc_nauki) {
        this.okresowosc_nauki = okresowosc_nauki;
    }

    @Override
    public boolean decyduj(double diamenty, Gielda gielda) {
        if( gielda.getTura() % okresowosc_nauki == 0 ){
            return true;
        }
        else {
            return false;
        }
    }
}
