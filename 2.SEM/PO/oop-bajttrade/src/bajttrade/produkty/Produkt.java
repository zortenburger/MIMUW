package bajttrade.produkty;

public abstract class Produkt{
    private double ilosc;

    public Produkt(double ilosc) {
        this.ilosc = ilosc;
    }

    public void dodaj(double x){
        ilosc += x;
    }

    public double getIlosc() {
        return ilosc;
    }

    public abstract int toInt();

    public static Produkt fromInt(int x){
        switch (x){
            case 0:
                return new Jedzenie(0);
            case 1:
                return new Ubrania(0);
            case 2:
                return new Narzedzia(0);
            case 3:
                return new Diamenty(0);
            case 4:
                return new ProgramyKomputerowe(0);
            default:
                return new Diamenty(420);
        }
    }

    public abstract Produkt clone(double ilosc);
}
