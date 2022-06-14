package bajttrade.produkty;

import java.util.ArrayList;
import java.util.List;

public class Narzedzia extends Produkt{
    private int poziomJakosci;

    public Narzedzia(double ilosc) {
        super(ilosc);
        this.poziomJakosci = 1;
    }

    public Narzedzia(double ilosc, int poziomJakosci) {
        super(ilosc);
        this.poziomJakosci = poziomJakosci;
    }

    @Override
    public int toInt() {
        return 2;
    }

    public void setPoziomJakosci(int poziomJakosci) {
        this.poziomJakosci = poziomJakosci;
    }

    public int getPoziomJakosci() {
        return poziomJakosci;
    }

    @Override
    public Narzedzia clone(double ilosc){
        return new Narzedzia(ilosc, getPoziomJakosci());
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
                res.add(new Narzedzia(ilosc, programy.get(i).getPoziomZaawansowania()));
                ilosc = 0;
            }
            else{
                ilosc -= programy.get(i).getIlosc();
                res.add(new Narzedzia(programy.get(i).getIlosc(), programy.get(i).getPoziomZaawansowania()));
                programy.remove(i);
            }
        }
        if(ilosc > 0){
            res.add(new Narzedzia(ilosc));
        }
        return res;
    }
}
