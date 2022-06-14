package bajttrade.agent.robotnik.praca;


import bajttrade.gielda.Gielda;
import bajttrade.produkty.Jedzenie;

public class Student extends StrategiaPracy{
    private int zapas;
    private int okres;

    public Student(int zapas, int okres) {
        this.zapas = zapas;
        this.okres = okres;
    }

    @Override
    public boolean decyduj(double diamenty, Gielda gielda) {
        return (diamenty <= zapas * 100 * gielda.sredniaCena((new Jedzenie(0)).toInt(),okres));
    }

}