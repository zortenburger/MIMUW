package bajttrade.gielda;

import bajttrade.agent.robotnik.Robotnik;
import bajttrade.agent.spekulant.Spekulant;
import bajttrade.produkty.*;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public abstract class Gielda {
    private int tura;
    private int dlugosc;
    private int kara_za_brak_ubran;
    protected List <Robotnik> robotnicy;
    protected List <Spekulant> spekulanci;
    private List<Tura> historia;

    public List<Robotnik> getRobotnicy() {
        return robotnicy;
    }

    public List<Spekulant> getSpekulanci() {
        return spekulanci;
    }

    public List<Tura> getHistoria() {
        return historia;
    }

    public Gielda(int dlugosc, int kara_za_brak_ubran, Tura zerowa) {
        this.tura = 1;
        this.dlugosc = dlugosc;
        robotnicy = new ArrayList<Robotnik>();
        spekulanci = new ArrayList<Spekulant>();
        historia = new ArrayList<Tura>();
        historia.add(zerowa);
        this.kara_za_brak_ubran = kara_za_brak_ubran;
    }

    public int getKara_za_brak_ubran() {
        return kara_za_brak_ubran;
    }

    public void dodajRobotnika(Robotnik nowy){
        robotnicy.add(nowy);
    }

    public void dodajSpekulanta(Spekulant nowy){
        spekulanci.add(nowy);
    }

    public int getTura(){
        return tura;
    }

    public void wykonajTure(){
        tura++;
        historia.add(new Tura());
        Tura aktualnaTura = historia.get(tura);
        for(int i = 0 ; i < robotnicy.size(); i++){
            robotnicy.get(i).przezyjDzien(this);
        }
        setIlosc2(aktualnaTura);
        List <Oferta> sprzedaz = new ArrayList<Oferta>();
        List <Oferta> kupno = new ArrayList<Oferta>();
        for(int i = 0 ; i < spekulanci.size(); i++){
            sprzedaz.addAll(spekulanci.get(i).przygotujOfertySprzedazy(this));
            kupno.addAll(spekulanci.get(i).przygotujOfertyKupna(this));
        }

        List <Oferta> pom = dopasujOferty(sprzedaz, kupno);

        dodajSrednia(pom, aktualnaTura);

        skupOfery();

        for(int i = 0 ; i < robotnicy.size(); i++){
            robotnicy.get(i).zakonczDzien();
        }
    }

    public Spekulant znajdzSpekulant(int id){
        for (Spekulant spekulant : spekulanci){
            if(id == spekulant.getId()){
                return spekulant;
            }
        }
        return null;
    }

    public List <Oferta> dopasujOferty(List <Oferta> sprzedaz,  List <Oferta> kupno){
        sortujRobotnikow();
        sortujSprzedaz(sprzedaz);
        sortujKupno(kupno);
        List <Oferta> res = new ArrayList<>();
        for(Robotnik robotnik : robotnicy){
            List<Produkt> chceSprzedac = robotnik.ofertySprzedazy;
            for(Produkt produkt : chceSprzedac){
                boolean czyDotarlam = false;
                for(Oferta oferta : kupno){
                    if(oferta.getProdukt().toInt() != produkt.toInt()){
                        if(czyDotarlam){
                            break;
                        }
                        else{
                            continue;
                        }
                    }
                    else{
                        czyDotarlam = true;
                        if(oferta.getProdukt().getIlosc() == 0){
                            continue;
                        }
                        if(produkt.getIlosc() == 0){
                            break;
                        }
                        else{
                            Spekulant aktSpekulant = znajdzSpekulant(oferta.getIdSpekulanta());
                            int ileKupi = (int)Math.min(
                                    (aktSpekulant.mojeDiamenty.getIlosc())/oferta.getCena(),
                                    produkt.getIlosc());
                            if(ileKupi > 0){
                                robotnik.mojeDiamenty.dodaj(ileKupi * oferta.getCena());
                                aktSpekulant.mojeDiamenty.dodaj(-ileKupi * oferta.getCena());
                                oferta.getProdukt().dodaj(-ileKupi);
                                produkt.dodaj(-ileKupi);
                                Produkt nowy = produkt.clone((int) ileKupi);
                                aktSpekulant.dodajPrzedmiot(nowy);
                                res.add(new Oferta(nowy, aktSpekulant.getId(), oferta.getCena()));
                            }
                        }
                    }
                }
            }

            List<Produkt> chceKupic = robotnik.ofertyKupna;
            for(Produkt produkt : chceKupic){
                boolean czyDotarlam = false;
                for(Oferta oferta : sprzedaz){
                    if(oferta.getProdukt().toInt() != produkt.toInt()){
                        if(czyDotarlam){
                            break;
                        }
                        else{
                            continue;
                        }
                    }
                    else{
                        czyDotarlam = true;
                        if(oferta.getProdukt().getIlosc() == 0){
                            continue;
                        }
                        if(produkt.getIlosc() == 0){
                            break;
                        }
                        else{
                            Spekulant aktSpekulant = znajdzSpekulant(oferta.getIdSpekulanta());
                            int ileKupi = (int)Math.min(
                                    (robotnik.mojeDiamenty.getIlosc())/oferta.getCena(),
                                    produkt.getIlosc());
                            if(ileKupi > 0){
                                aktSpekulant.mojeDiamenty.dodaj(ileKupi * oferta.getCena());
                                robotnik.mojeDiamenty.dodaj(-ileKupi * oferta.getCena());
                                oferta.getProdukt().dodaj(-ileKupi );
                                produkt.dodaj(-ileKupi);
                                Produkt nowy = produkt.clone((int) ileKupi);
                                robotnik.dodajPrzedmiot(nowy);
                                res.add(new Oferta(nowy, aktSpekulant.getId(), oferta.getCena()));
                            }
                        }
                    }
                }
            }
        }
        return res;
    }

    public void setIlosc2(Tura aktualna){
        int tab[] = new int[5];
        for(int i = 0; i < robotnicy.size(); i++){
            for(int j = 0; j < robotnicy.get(i).ofertySprzedazy.size(); j++){
                tab[robotnicy.get(i).ofertySprzedazy.get(j).toInt()] += robotnicy.get(i).ofertySprzedazy.get(j).getIlosc();
            }
        }
        aktualna.setIlosc2(tab);
    }

    public void skupOfery(){
        for(int i = 0; i < robotnicy.size(); i++){
            Robotnik akt = robotnicy.get(i);
            for(int j = 0; j < akt.ofertySprzedazy.size(); j++){
                akt.mojeDiamenty.dodaj(akt.ofertySprzedazy.get(j).getIlosc()
                        * sredniaCena(akt.ofertySprzedazy.get(j).toInt(), 1));
            }
            akt.ofertySprzedazy = null;
            akt.ofertyKupna = null;
        }
    }

    public void dodajSrednia(List <Oferta> produkty, Tura aktualnaTura){
        for(int p = 0; p < 5; p++){
            List <Double> ceny = new ArrayList<Double>();
            for(int i = 0; i < produkty.size(); i++){
                Produkt aktualnyProdukt = produkty.get(i).getProdukt();
                if(aktualnyProdukt.toInt() == p){
                    for(int j = 0; j < produkty.get(i).getProdukt().getIlosc(); j++) {
                        ceny.add(produkty.get(i).getCena());
                    }
                }
            }
            aktualnaTura.statystyka(p, ceny, historia.get(0));
        }
        historia.add(aktualnaTura);
    }

    public double sredniaCena(int przedmiot, int dni_temu){
        return historia.get(tura - dni_temu).getSrednia(przedmiot);
    }

    public double minimalnaCena(int przedmiot, int dni_temu){
        return historia.get(tura - dni_temu).getMinimalna(przedmiot);
    }

    public double maksymalnaCena(int przedmiot, int dni_temu){
        return historia.get(tura - dni_temu).getMaksymalna(przedmiot);
    }

    public double iloscCena(int przedmiot, int dni_temu){
        return historia.get(tura - dni_temu).getIlosc(przedmiot);
    }

    public double iloscCena2(int przedmiot, int dni_temu){
        return historia.get(tura - dni_temu).getIlosc2(przedmiot);
    }

    protected int compare (Robotnik robotnik1, Robotnik robotnik2){
        double diaxy1 = robotnik1.mojeDiamenty.getIlosc();
        double diaxy2 = robotnik2.mojeDiamenty.getIlosc();

        if(diaxy1 != diaxy2){
            return Double.compare(diaxy1, diaxy2);
        }
        else{
            return Integer.compare(robotnik1.getId(), robotnik2.getId());
        }
    }

    protected int compareSprzedaz (Oferta oferta1, Oferta oferta2){
        Produkt przedmiot1 = oferta1.getProdukt();
        Produkt przedmiot2 = oferta2.getProdukt();

        if(przedmiot1.toInt() != przedmiot2.toInt()){
            return Integer.compare(przedmiot1.toInt(), przedmiot2.toInt());
        }
        else{
            if(przedmiot1.toInt() == 0){
                return Double.compare(oferta2.getCena(), oferta1.getCena());
            }
            else {
                if(przedmiot1.toInt() == 1){
                    if(((Ubrania) przedmiot1).getPoziomJakosci() != ((Ubrania) przedmiot2).getPoziomJakosci()){
                        return Integer.compare(((Ubrania) przedmiot1).getPoziomJakosci(),
                                ((Ubrania) przedmiot2).getPoziomJakosci());
                    }
                    else{
                        return Double.compare(oferta2.getCena(), oferta1.getCena());
                    }
                }
                if(przedmiot1.toInt() == 2){
                    if(((Narzedzia) przedmiot1).getPoziomJakosci() != ((Narzedzia) przedmiot2).getPoziomJakosci()){
                        return Integer.compare(((Narzedzia) przedmiot1).getPoziomJakosci(),
                                ((Narzedzia) przedmiot2).getPoziomJakosci());
                    }
                    else{
                        return Double.compare(oferta2.getCena(), oferta1.getCena());
                    }
                }
                if(przedmiot1.toInt() == 4){
                    if(((ProgramyKomputerowe) przedmiot1).getPoziomZaawansowania() !=
                            ((ProgramyKomputerowe) przedmiot2).getPoziomZaawansowania()){
                        return Integer.compare(((ProgramyKomputerowe) przedmiot1).getPoziomZaawansowania(),
                                ((ProgramyKomputerowe) przedmiot2).getPoziomZaawansowania());
                    }
                    else{
                        return Double.compare(oferta2.getCena(), oferta1.getCena());
                    }
                }
            }
        }
        return Double.compare(oferta2.getCena(), oferta1.getCena());
    }

    protected int compareKupno (Oferta oferta1, Oferta oferta2){
        Produkt przedmiot1 = oferta1.getProdukt();
        Produkt przedmiot2 = oferta2.getProdukt();

        if(przedmiot1.toInt() != przedmiot2.toInt()){
            return Integer.compare(przedmiot1.toInt(), przedmiot2.toInt());
        }
        return Double.compare(oferta1.getCena(), oferta2.getCena());
    }
    protected abstract int compareAbs(Robotnik robotnik1, Robotnik robotnik2);

    private void sortujRobotnikow(){
        Collections.sort(robotnicy, this::compareAbs);
    }

    private void sortujSprzedaz(List<Oferta> sprzedaz){
        Collections.sort(sprzedaz, this::compareSprzedaz);
    }

    private void sortujKupno(List<Oferta> kupno){
        Collections.sort(kupno, this::compareKupno);
    }

    public int getDlugosc() {
        return dlugosc;
    }
}
