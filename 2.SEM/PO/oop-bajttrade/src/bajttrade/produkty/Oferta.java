package bajttrade.produkty;

public class Oferta {
    Produkt produkt;
    int idSpekulanta;
    double cena;


    public Oferta(Produkt produkt, int idSpekulanta, double cena) {
        this.produkt = produkt;
        this.idSpekulanta = idSpekulanta;
        this.cena = cena;
    }

    public Produkt getProdukt() {
        return produkt;
    }

    public int getIdSpekulanta() {
        return idSpekulanta;
    }

    public double getCena() {
        return cena;
    }
}
