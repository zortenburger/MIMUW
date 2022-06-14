import bajttrade.agent.robotnik.Robotnik;
import bajttrade.agent.spekulant.Spekulant;
import bajttrade.gielda.Gielda;
import bajttrade.gielda.Tura;
import com.google.gson.Gson;

public class Wypisywanie {

    public static void wypiszDzien(Tura tura){

    }
    public static void wypiszRobotnika(Robotnik robotnik){

    }public static void wypiszSpekulanta(Spekulant spekulant){

    }
    public static void wypisz (Gielda gielda){
        Gson gson = new Gson();
        Wypisywanie.wypiszDzien(gielda.getHistoria().get(gielda.getTura()));
        for(Robotnik robotnik : gielda.getRobotnicy()){
            Wypisywanie.wypiszRobotnika(robotnik);
        }
        for(Spekulant spekulant : gielda.getSpekulanci()){
            Wypisywanie.wypiszSpekulanta(spekulant);
        }
    }
}
