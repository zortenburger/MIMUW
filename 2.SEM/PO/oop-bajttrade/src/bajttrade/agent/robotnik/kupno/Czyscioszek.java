package bajttrade.agent.robotnik.kupno;

import bajttrade.agent.robotnik.Robotnik;
import bajttrade.produkty.Produkt;
import bajttrade.produkty.Ubrania;

import java.util.List;

public class Czyscioszek extends Technofob{
    public Czyscioszek() {
    }

    @Override
    public List<Produkt> kupuj(Robotnik robotnik) {
        List <Produkt> res = super.kupuj(robotnik);
        int mojeUbrania = robotnik.getUbrania();
        if(mojeUbrania < 100){
            res.add(new Ubrania(100-mojeUbrania, 0));
        }
        return res;
    }
}
