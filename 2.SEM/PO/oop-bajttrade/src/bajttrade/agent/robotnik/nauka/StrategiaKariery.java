package bajttrade.agent.robotnik.nauka;

import bajttrade.gielda.Gielda;

public abstract class StrategiaKariery {
    public StrategiaKariery(){}

    public abstract int zmienKariere(int aktualnaKariera, int id, Gielda gielda, int dni);

}
