import bajttrade.agent.robotnik.Robotnik;
import bajttrade.agent.robotnik.kariera.*;
import bajttrade.agent.robotnik.kupno.*;
import bajttrade.agent.robotnik.nauka.Konserwatysta;
import bajttrade.agent.robotnik.nauka.Rewolucjonista;
import bajttrade.agent.robotnik.nauka.StrategiaKariery;
import bajttrade.agent.robotnik.praca.*;
import bajttrade.agent.robotnik.produkcja.*;
import bajttrade.agent.spekulant.RegulujacyRynek;
import bajttrade.agent.spekulant.Spekulant;
import bajttrade.agent.spekulant.Sredni;
import bajttrade.agent.spekulant.Wypukly;
import bajttrade.gielda.*;
import bajttrade.produkty.*;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

public class Wczytywanie {
    public static Gielda wczytaj(JsonObject fileObject){
        JsonObject info = fileObject.get("info").getAsJsonObject();

        int dlugosc = info.get("dlugosc").getAsInt();
        String typ = info.get("gielda").getAsString();
        int kara_za_brak_ubran = info.get("kara_za_brak_ubran").getAsInt();
        JsonObject ceny = info.get("ceny").getAsJsonObject();

        int jedzenie = ceny.get("jedzenie").getAsInt();
        int ubrania = ceny.get("ubrania").getAsInt();
        int narzedzia = ceny.get("narzedzia").getAsInt();
        int programy = ceny.get("programy").getAsInt();

        Gielda gielda = null;

        switch (typ){
            case "kapitalistyczna":
                gielda = new Kapitalistyczna(dlugosc, kara_za_brak_ubran, new Tura(jedzenie, ubrania, narzedzia, programy));
            case "socjalistyczna":
                gielda = new Socjalistyczna(dlugosc, kara_za_brak_ubran, new Tura(jedzenie, ubrania, narzedzia, programy));
            case "zrownowazona":
                gielda = new Zrownowazona(dlugosc, kara_za_brak_ubran, new Tura(jedzenie, ubrania, narzedzia, programy));
        }

        JsonArray robotnicy = fileObject.get("robotnicy").getAsJsonArray();
        JsonArray spekulanci = fileObject.get("spekulanci").getAsJsonArray();

        for(JsonElement rob : robotnicy){
            JsonObject robotnikJson = rob.getAsJsonObject();

            int id = robotnikJson.get("id").getAsInt();
            int poziom = robotnikJson.get("poziom").getAsInt();

            String aktKarieraStr = robotnikJson.get("kariera").getAsString();
            SciezkaKariery aktKariera = new Gornik();

            switch (aktKarieraStr){
                case "gornik":
                    aktKariera = new Gornik();
                case "inzynier":
                    aktKariera = new Inzynier();
                case "programista":
                    aktKariera = new Programista();
                case "rolnik":
                    aktKariera = new Rolnik();
                case "rzemieslnik":
                    aktKariera = new Rzemieslnik();
            }

            JsonObject uczenie = robotnikJson.get("uczenie").getAsJsonObject();
            String pracaStr = uczenie.get("typ").getAsString();
            StrategiaPracy praca = new Pracus();
            int limit_diamentow;
            int zapas, okres;
            int okresowosc_nauki;

            switch (pracaStr){
                case "pracus":
                    praca = new Pracus();
                case "oszczedny":
                    limit_diamentow = uczenie.get("limit_diamentow").getAsInt();
                    praca = new Oszczedny(limit_diamentow);
                case "student":
                    zapas = uczenie.get("zapas").getAsInt();
                    okres = uczenie.get("okres").getAsInt();
                    praca = new Student(zapas, okres);
                case "okresowy":
                    okresowosc_nauki = uczenie.get("okresowosc_nauki").getAsInt();
                    praca = new Okresowy(okresowosc_nauki);
                case "rozkladowy":
                    praca = new Rozkladowy();
            }

            JsonObject kupowanie = robotnikJson.get("kupowanie").getAsJsonObject();
            String kupnoStr = kupowanie.get("typ").getAsString();
            StrategiaKupna kupno = new Technofob();
            int liczba_narzedzi;

            switch (kupnoStr){
                case "technofob":
                    kupno = new Technofob();
                case "czyscioszek":
                    kupno = new Czyscioszek();
                case "zmechanizowany":
                    liczba_narzedzi = kupowanie.get("liczba_narzedzi").getAsInt();
                    kupno = new Zmechanizowany(liczba_narzedzi);
                case "gadzeciarz":
                    liczba_narzedzi = kupowanie.get("liczba_narzedzi").getAsInt();
                    kupno = new Gadzeciarz(liczba_narzedzi);
            }

            JsonObject zmiana = robotnikJson.get("zmiana").getAsJsonObject();
            String naukaStr = zmiana.get("typ").getAsString();
            StrategiaKariery nauka = new Konserwatysta();

            switch (naukaStr){
                case "konserwatysta":
                    nauka = new Konserwatysta();
                case "rewolucjonista":
                    nauka = new Rewolucjonista();
            }

            JsonObject produkcjaJ = robotnikJson.get("produkcja").getAsJsonObject();
            String produkcjaStr = produkcjaJ.get("typ").getAsString();
            StrategiaProdukcji produkcja = new Losowy();
            int historia_sredniej_produkcji;
            int historia_perspektywy;

            switch (produkcjaStr){
                case "krotkowzroczny":
                    produkcja = new Krotkowzroczny();
                case "chciwy":
                    produkcja = new Chciwy();
                case "sredniak":
                    historia_sredniej_produkcji = produkcjaJ.get("historia_sredniej_produkcji").getAsInt();
                    produkcja = new Sredniak(historia_sredniej_produkcji);
                case "perspektywiczny":
                    historia_perspektywy = produkcjaJ.get("historia_perspektywy").getAsInt();
                    produkcja = new Perspektywiczny(historia_perspektywy);
                case "losowy":
                    produkcja = new Losowy();
            }

            JsonObject produktywnowc = robotnikJson.get("produktywnosc").getAsJsonObject();

            int diamentyRob = produktywnowc.get("diamenty").getAsInt();
            int jedzenieRob = produktywnowc.get("jedzenie").getAsInt();
            int ubraniaRob = produktywnowc.get("ubrania").getAsInt();
            int narzedziaRob = produktywnowc.get("narzedzia").getAsInt();
            int programyRob = produktywnowc.get("programy").getAsInt();

            JsonObject zasoby = robotnikJson.get("zasoby").getAsJsonObject();

            int diamentyZasobyRob = zasoby.get("diamenty").getAsInt();
            int jedzenieZasobyRob = zasoby.get("jedzenie").getAsInt();
            int ubraniaZasobyRob = zasoby.get("ubrania").getAsInt();
            int narzedziaZasobyRob = zasoby.get("narzedzia").getAsInt();
            int programyZasobyRob = zasoby.get("programy").getAsInt();

            Produkt diamentyZasoby = new Diamenty(diamentyZasobyRob);
            Produkt jedzenieZasoby = new Jedzenie(jedzenieZasobyRob);
            Produkt ubraniaZasoby = new Ubrania(ubraniaZasobyRob);
            Produkt narzedziaZasoby = new Narzedzia(narzedziaZasobyRob);
            Produkt programyZasoby = new ProgramyKomputerowe(programyZasobyRob);

            Robotnik robotnik = new Robotnik(id, poziom, jedzenieRob, ubraniaRob, narzedziaRob, diamentyRob, programyRob,
                    aktKariera, nauka, kupno, praca, produkcja, diamentyZasoby,jedzenieZasoby, ubraniaZasoby, narzedziaZasoby, programyZasoby);

            gielda.dodajRobotnika(robotnik);

        }

        for(JsonElement spek : spekulanci){
            JsonObject spekulantJson = spek.getAsJsonObject();

            int id = spekulantJson.get("id").getAsInt();

            JsonObject zasoby = spekulantJson.get("zasoby").getAsJsonObject();

            int diamentyZasobyRob = zasoby.get("diamenty").getAsInt();
            int jedzenieZasobyRob = zasoby.get("jedzenie").getAsInt();
            int ubraniaZasobyRob = zasoby.get("ubrania").getAsInt();
            int narzedziaZasobyRob = zasoby.get("narzedzia").getAsInt();
            int programyZasobyRob = zasoby.get("programy").getAsInt();

            Produkt diamentyZasoby = new Diamenty(diamentyZasobyRob);
            Produkt jedzenieZasoby = new Jedzenie(jedzenieZasobyRob);
            Produkt ubraniaZasoby = new Ubrania(ubraniaZasobyRob);
            Produkt narzedziaZasoby = new Narzedzia(narzedziaZasobyRob);
            Produkt programyZasoby = new ProgramyKomputerowe(programyZasobyRob);


            JsonObject karieraJson = spekulantJson.get("kariera").getAsJsonObject();
            String karieraStr = karieraJson.get("typ").getAsString();
            Spekulant spekulant = null;
            int historia_spekulanta_sredniego;

            switch (karieraStr){
                case "sredni":
                    historia_spekulanta_sredniego = karieraJson.get("historia_spekulanta_sredniego").getAsInt();
                    spekulant = new Sredni(historia_spekulanta_sredniego, id, diamentyZasoby, jedzenieZasoby, ubraniaZasoby, narzedziaZasoby, programyZasoby);
                case "wypukly":
                    spekulant = new Wypukly(id, diamentyZasoby, jedzenieZasoby, ubraniaZasoby, narzedziaZasoby, programyZasoby);
                case "regulujacy_rynek":
                    spekulant = new RegulujacyRynek(id, diamentyZasoby, jedzenieZasoby, ubraniaZasoby, narzedziaZasoby, programyZasoby);

            }

            gielda.dodajSpekulanta(spekulant);

        }
        return gielda;
    }
}
