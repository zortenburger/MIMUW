package bajttrade.agent.robotnik.praca;

import bajttrade.gielda.Gielda;

import java.util.Random;

public class Rozkladowy extends StrategiaPracy{
    public Rozkladowy() {
        super();
    }

    @Override
    public boolean decyduj(double diamenty, Gielda gielda) {
        Random rand = new Random();
        double p = rand.nextDouble();
        double pos = 1.0/(gielda.getTura() + 3);
        return(p <= pos);
    }

}
