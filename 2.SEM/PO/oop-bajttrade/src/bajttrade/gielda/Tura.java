package bajttrade.gielda;

import java.util.List;

public class Tura {
    private int[] ilosc;

    private int[] ilosc2;
    private double[] minimalna;
    private double[] maksymalna;
    private double[] srednia;

    public Tura(){
        ilosc = new int[5];
        ilosc2 = new int[5];
        minimalna = new double[5];
        maksymalna = new double[5];
        srednia = new double[5];
    }

    public Tura(int jedzenie, int ubrania, int narzedzie, int programy){
        double[] pom = new double[5];
        pom[0] = jedzenie;
        pom[1] = ubrania;
        pom[2] = narzedzie;
        pom[3] = 0;
        pom[4] = programy;
        ilosc = new int[5];
        ilosc2 = new int[5];
        minimalna = pom;
        maksymalna = pom;
        srednia = pom;
    }

    public void statystyka(int przedmiot, List<Double> ceny, Tura zerowa){
        double mini = 1000000;
        double maxi = 0;
        int ile = ceny.size();
        double suma = 0;
        for(int i = 0; i < ile; i++){
            suma += ceny.get(i);
            mini = Math.min(mini, ceny.get(i));
            maxi = Math.max(maxi, ceny.get(i));
        }
        if(mini < 1000000)
            minimalna[przedmiot] = mini;
        else
            minimalna[przedmiot] = zerowa.minimalna[przedmiot];
        if(maxi > 0)
            maksymalna[przedmiot] = maxi;
        else
            maksymalna[przedmiot] = zerowa.maksymalna[przedmiot];
        ilosc[przedmiot] = ile;
        if(suma > 0)
            srednia[przedmiot] = maxi;
        else
            srednia[przedmiot] = zerowa.srednia[przedmiot];
    }

    public double getMaksymalna(int przedmiot) {
        return maksymalna[przedmiot];
    }

    public double getMinimalna(int przedmiot) {
        return minimalna[przedmiot];
    }

    public double getSrednia(int przedmiot) {
        return srednia[przedmiot];
    }

    public int getIlosc(int przedmiot) {
        return ilosc[przedmiot];
    }

    public int getIlosc2(int przedmiot) {
        return ilosc2[przedmiot];
    }

    public void setIlosc2(int[] tab){
        ilosc2 = tab;
    }
}
