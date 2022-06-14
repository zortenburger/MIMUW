package bajttrade.gielda;

import bajttrade.agent.robotnik.Robotnik;

public class Kapitalistyczna extends Gielda{


    public Kapitalistyczna(int dlugosc, int kara_za_brak_ubran, Tura zerowa) {
        super(dlugosc, kara_za_brak_ubran, zerowa);
    }

    @Override
    protected int compareAbs(Robotnik robotnik1, Robotnik robotnik2) {
        return compare(robotnik1, robotnik2);
    }
}
