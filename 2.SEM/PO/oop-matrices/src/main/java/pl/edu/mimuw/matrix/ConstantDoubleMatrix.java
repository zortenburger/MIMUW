package pl.edu.mimuw.matrix;

public class ConstantDoubleMatrix extends DoubleMatrix{
    private double value;

    public ConstantDoubleMatrix(Shape shape, double value) {
        super(shape);
        this.value = value;
    }

    public static IDoubleMatrix create(Shape shape, double value) {
        assert(shape != null && shape.rows > 0 && shape.columns > 0);
        if(value == 0) {
            return ZeroDoubleMatrix.create(shape);
        }
        else {
            return new ConstantDoubleMatrix(shape, value);
        }
    }


    @Override
    public IDoubleMatrix tryTimes(double scalar) {
        return ConstantDoubleMatrix.create(this.shape(), this.value * scalar);
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other){
        assert(this.shape().equals(other.shape()));
        return other.plus(this.value);
    }

    @Override
    public IDoubleMatrix plus(double scalar) {
        return ConstantDoubleMatrix.create(this.shape(), this.value + scalar);
    }

    @Override
    public double get(int row, int column) {
        shape().assertInShape(row, column);
        return this.value;
    }

    @Override
    public double normOne() {
        return this.shape().rows * Math.abs(this.value);
    }

    @Override
    public double normInfinity() {
        return this.shape().columns * Math.abs(this.value);
    }

    @Override
    public double frobeniusNorm() {
        return Math.sqrt(this.shape().rows * this.shape().columns * this.value * this.value);
    }

    @Override
    public String toString(){
        String res = this.shape().rows + " " + this.shape().columns + "\n";
        for(int i = 0; i < this.shape().rows; i++){
            if(this.shape().columns == 1){
                res += value + "\n";
            }
            if(this.shape().columns == 2){
                res += value + " " + value + "\n";
            }
            if(this.shape().columns > 2){
                res +=  value + " ... " + value + "\n";
            }
        }
        return res;
    }

    @Override
    public IDoubleMatrix negative() {
        return this.tryTimes(-1);
    }

    @Override
    public IDoubleMatrix copy() {
        IDoubleMatrix res = ConstantDoubleMatrix.create(this.shape(), this.value);
        return res;
    }
}
