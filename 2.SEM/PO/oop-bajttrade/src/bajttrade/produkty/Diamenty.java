package bajttrade.produkty;

public class Diamenty extends Produkt {
    public Diamenty(double ilosc) {
        super(ilosc);
    }

    @Override
    public int toInt() {
        return 3;
    }

    @Override
    public Diamenty clone(double ilosc){
        return new Diamenty(ilosc);
    }
}
