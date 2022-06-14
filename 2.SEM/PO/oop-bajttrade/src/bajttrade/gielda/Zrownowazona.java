package bajttrade.gielda;

import bajttrade.agent.robotnik.Robotnik;

public class Zrownowazona extends Gielda{

    public Zrownowazona(int dlugosc, int kara_za_brak_ubran, Tura zerowa) {
        super(dlugosc, kara_za_brak_ubran, zerowa);
    }

    @Override
    public int compareAbs(Robotnik robotnik1, Robotnik robotnik2) {
        if(getTura() % 2 == 0) {
            return compare(robotnik2, robotnik1);
        }
        else{
            return compare(robotnik1, robotnik2);
        }
    }
}
