package bajttrade.gielda;

import bajttrade.agent.robotnik.Robotnik;

public class Socjalistyczna extends Gielda{

    public Socjalistyczna(int dlugosc, int kara_za_brak_ubran, Tura zerowa) {
        super(dlugosc, kara_za_brak_ubran, zerowa);
    }

    @Override
    protected int compareAbs(Robotnik robotnik1, Robotnik robotnik2) {
        return compare(robotnik2, robotnik1);
    }
}
