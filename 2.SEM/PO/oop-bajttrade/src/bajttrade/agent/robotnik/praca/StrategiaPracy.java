package bajttrade.agent.robotnik.praca;

import bajttrade.gielda.Gielda;

public abstract class StrategiaPracy {

    public StrategiaPracy() {
    }

    public abstract boolean decyduj(double diamenty, Gielda gielda);
}
