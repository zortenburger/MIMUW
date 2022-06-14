package pl.edu.mimuw.matrix;

public class DiagonalDoubleMatrix extends DoubleMatrix {
    private double[] diagonal;

    public DiagonalDoubleMatrix(Shape shape, double... values) {
        super(shape);
        this.diagonal = values;
    }

    public DiagonalDoubleMatrix(Shape shape) {
        super(shape);
        this.diagonal = new double[shape.columns];
    }

    public static IDoubleMatrix create(Shape shape, double value){
        double[] values = new double[shape.rows];
        for(int i = 0; i < values.length; i++){
            values[i] = value;
        }
        return new DiagonalDoubleMatrix(shape, values);
    }

    public static IDoubleMatrix create(double... values) {
        assert(values != null && values.length > 0);
        Shape shape = Shape.matrix(values.length, values.length);
        boolean ones = true;
        boolean zeros = true;
        for(int i = 0; i < values.length ; i++) {
            if(values[i] != 0) {
                zeros = false;
            }
            if(values[i] != 1) {
                ones = false;
            }
        }
        if(zeros == true) {
            return ZeroDoubleMatrix.create(shape);
        }
        if(ones == true) {
            return IdentityDoubleMatrix.create(shape);
        }
        return new DiagonalDoubleMatrix(shape, values);
    }

    public static DiagonalDoubleMatrix create(Shape shape) {
        return new DiagonalDoubleMatrix(shape);
    }

    @Override
    public IDoubleMatrix tryTimes(double scalar) {
        DiagonalDoubleMatrix res = DiagonalDoubleMatrix.create(this.shape());

        for(int i = 0; i < this.diagonal.length; i++) {
            res.diagonal[i] = scalar * this.diagonal[i];
        }
        return res;
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other){
        assert (this.shape().equals(other.shape()));
        if(other.getClass() == DiagonalDoubleMatrix.class){
            DiagonalDoubleMatrix res = (DiagonalDoubleMatrix) this.copy();
            for(int i = 0; i < res.diagonal.length; i++){
                res.diagonal[i] += ((DiagonalDoubleMatrix) other).diagonal[i];
            }
        }
        return super.plus(other);
    }

    @Override
    public double get(int row, int column) {
        shape().assertInShape(row, column);
        if(row == column){
            return this.diagonal[row];
        }
        else return 0;
    }

    @Override
    public double normOne() {
        double res = Math.abs(diagonal[0]);
        for (int i = 1; i < diagonal.length; i++) {
            res = Math.max(res, Math.abs(diagonal[i]));
        }
        return res;
    }

    @Override
    public double normInfinity() {
        double res = Math.abs(diagonal[0]);
        for (int i = 1; i < diagonal.length; i++) {
            res = Math.max(res, Math.abs(diagonal[i]));
        }
        return res;
    }

    @Override
    public double frobeniusNorm() {
        double res = 0;
        for (int i = 0; i < diagonal.length; i++) {
            res = res + diagonal[i] * diagonal[i];
        }
        return Math.sqrt(res);
    }

    public String zeros(int n){
        if(n == 0){
            return  "";
        }
        if(n == 1){
            return  "0 ";
        }
        if(n == 2){
            return "0 0 ";
        }
        return "0 ... 0 ";
    }

    @Override
    public String toString(){
        String res = this.shape().rows + " " + this.shape().columns + ":\n";
        int size = this.shape().columns;
        for(int i = 0; i < size; i++){
            res += zeros(i) + this.diagonal[i]  + " " + zeros(size - i - 1) + "\n";
        }
        return res;
    }

    @Override
    public IDoubleMatrix negative() {
        return this.tryTimes(-1);
    }

    @Override
    public IDoubleMatrix copy() {
        return this.tryTimes(1);
    }
}
