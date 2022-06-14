package bajttrade.produkty;

import java.util.ArrayList;
import java.util.List;

public class Ubrania extends Produkt{
    private int poziomJakosci;
    private int zuzycie;

    public Ubrania(double ilosc) {
        super(ilosc);
        this.poziomJakosci = 1;
        this.zuzycie = 0;
    }

    public Ubrania(double ilosc, int poziomJakosci) {
        super(ilosc);
        this.poziomJakosci = poziomJakosci;
        this.zuzycie = 0;
    }

    public Ubrania(double ilosc, int poziomJakosci, int zuzycie) {
        super(ilosc);
        this.poziomJakosci = poziomJakosci;
        this.zuzycie = zuzycie;
    }

    public int getPoziomJakosci() {
        return poziomJakosci;
    }

    public int getZuzycie() {
        return zuzycie;
    }

    public void uzyj(){
        zuzycie++;
    }

    @Override
    public Ubrania clone(double ilosc){
        return new Ubrania(ilosc, getPoziomJakosci(), getZuzycie());
    }

    public void setPoziomJakosci(int poziomJakosci) {
        this.poziomJakosci = poziomJakosci;
    }

    public List<Produkt> ulepsz (List<ProgramyKomputerowe> programy){
        List<Produkt> res = new ArrayList<Produkt>();
        double ilosc = getIlosc();
        for(int i = programy.size() - 1; i >= 0; i--){
            if (ilosc == 0){
                break;
            }
            if (programy.get(i).getIlosc() > ilosc){
                programy.get(i).dodaj(ilosc);
                res.add(new Ubrania(ilosc, programy.get(i).getPoziomZaawansowania()));
                ilosc = 0;
            }
            else{
                ilosc -= programy.get(i).getIlosc();
                res.add(new Ubrania(programy.get(i).getIlosc(), programy.get(i).getPoziomZaawansowania()));
                programy.remove(i);
            }
        }
        if(ilosc > 0){
            res.add(new Ubrania(ilosc));
        }
        return res;
    }

    @Override
    public int toInt() {
        return 1;
    }
}
