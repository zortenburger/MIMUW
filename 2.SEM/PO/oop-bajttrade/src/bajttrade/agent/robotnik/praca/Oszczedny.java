package bajttrade.agent.robotnik.praca;

import bajttrade.gielda.Gielda;

public class Oszczedny extends StrategiaPracy{
    private int limit_diamentow;

    public Oszczedny(int limit_diamentow) {
        this.limit_diamentow = limit_diamentow;
    }

    @Override
    public boolean decyduj(double diamenty, Gielda gielda) {
        return (limit_diamentow < diamenty);
    }
}
