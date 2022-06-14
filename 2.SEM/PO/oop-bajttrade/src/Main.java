
import bajttrade.gielda.*;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;

public class Main {
    public static void main(String[] args){
        File input = new File("in.json");
        File output = new File("out.json");
        try {
            JsonElement fileElement = JsonParser.parseReader(new FileReader(input));
            JsonObject fileObject = fileElement.getAsJsonObject();

            Gielda gielda = Wczytywanie.wczytaj(fileObject);

            for(int i = 0; i < gielda.getDlugosc(); i++){
                gielda.wykonajTure();
                Wypisywanie.wypisz(gielda);
            }


        } catch (FileNotFoundException e) {
            System.err.println("Error File not found");
            e.printStackTrace();
        } catch (Exception e){
            System.err.println("Error");
            e.printStackTrace();
        }
    }
}
