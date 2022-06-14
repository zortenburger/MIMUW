package bajttrade.agent.robotnik.kariera;

public abstract class SciezkaKariery {
    protected int poziom;

    public SciezkaKariery(){
        this.poziom = 1;
    }

    public void setPoziom(int poziom) {
        this.poziom = poziom;
    }

    public int premia(){
        switch(poziom){
            case 1:
                return 50;
            case 2:
                return 150;
            case 3:
                return 300;
            default:
                return 300 + (poziom - 3) * 25;
        }
    }

    public  void uczSie(){
        poziom++;
    }

    public int getPoziom() {
        return poziom;
    }

    public abstract int toInt();

    public static SciezkaKariery fromInt(int x){
        switch (x){
            case 0:
                return new Rolnik();
            case 1:
                return new Rzemieslnik();
            case 2:
                return new Inzynier();
            case 3:
                return new Gornik();
            case 4:
                return new Programista();
            default:
                return new Gornik();
        }
    }
}