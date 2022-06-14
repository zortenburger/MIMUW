package bajttrade.produkty;

public class Jedzenie extends Produkt{
    public Jedzenie(double ilosc) {
        super(ilosc);
    }

    @Override
    public Jedzenie clone(double ilosc){
        return new Jedzenie(ilosc);
    }

    @Override
    public int toInt() {
        return 0;
    }
}
