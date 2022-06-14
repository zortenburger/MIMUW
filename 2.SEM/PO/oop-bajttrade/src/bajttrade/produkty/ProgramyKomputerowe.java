package bajttrade.produkty;

public class ProgramyKomputerowe extends Produkt{
    private int poziomZaawansowania;

    public ProgramyKomputerowe(int ilosc) {
        super(ilosc);
        this.poziomZaawansowania = 1;
    }

    public ProgramyKomputerowe(double ilosc, int poziomZaawansowania) {
        super(ilosc);
        this.poziomZaawansowania = poziomZaawansowania;
    }

    public int getPoziomZaawansowania() {
        return poziomZaawansowania;
    }

    @Override
    public ProgramyKomputerowe clone(double ilosc){
        return new ProgramyKomputerowe(ilosc, getPoziomZaawansowania());
    }

    public void setPoziomZaawansowania(int poziomZaawansowania) {
        this.poziomZaawansowania = poziomZaawansowania;
    }

    @Override
    public int toInt() {
        return 4;
    }
}
