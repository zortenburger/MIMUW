package bajttrade.agent.spekulant;

import bajttrade.agent.Agent;
import bajttrade.gielda.Gielda;
import bajttrade.produkty.Oferta;
import bajttrade.produkty.Produkt;

import java.util.List;

public abstract class Spekulant extends Agent {

    public Spekulant(int id, Produkt... produkty) {
        super(id, produkty);
    }

    public abstract List<Oferta> przygotujOfertySprzedazy(Gielda gielda);

    public abstract List<Oferta> przygotujOfertyKupna(Gielda gielda);
}
