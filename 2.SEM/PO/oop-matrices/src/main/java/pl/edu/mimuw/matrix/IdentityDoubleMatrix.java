package pl.edu.mimuw.matrix;

public class IdentityDoubleMatrix extends DoubleMatrix{

    public IdentityDoubleMatrix(Shape shape) {
        super(shape);
    }

    public static IdentityDoubleMatrix create(Shape shape){
        assert (shape != null && shape.columns > 0 && shape.rows > 0);
        return new IdentityDoubleMatrix(shape);
    }

    @Override
    public IDoubleMatrix times(IDoubleMatrix other) {
        assert(this.shape().columns == other.shape().rows);
        IDoubleMatrix res = other.copy();
        return res;
    }

    public IDoubleMatrix revTimes(IDoubleMatrix other){
        return other.copy();
    }

    @Override
    public IDoubleMatrix tryTimes(double scalar) {
        return DiagonalDoubleMatrix.create(this.shape(), scalar);
    }

    @Override
    public double get(int row, int column) {
        shape().assertInShape(row, column);
        if(row == column){
            return 1;
        }
        else return 0;
    }

    @Override
    public double normOne() {
        return 1;
    }

    @Override
    public double normInfinity() {
        return 1;
    }

    @Override
    public double frobeniusNorm() {
        return Math.sqrt(this.shape().rows);
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
            res += zeros(i) + "1 " + zeros(size - i - 1) + "\n";
        }
        return res;
    }

    @Override
    public IDoubleMatrix negative() {
        return DiagonalDoubleMatrix.create(this.shape(), -1);
    }

    @Override
    public IDoubleMatrix copy() {
        return this.create(this.shape());
    }
}
