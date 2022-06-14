package pl.edu.mimuw.matrix;

public class VectorDoubleMatrix extends DoubleMatrix{
    double[] values;

    public VectorDoubleMatrix(Shape shape, double... values) {
        super(shape);
        this.values = values;
    }

    public VectorDoubleMatrix(Shape shape) {
        super(shape);
        this.values = new double[shape.rows];
    }

    public static VectorDoubleMatrix create(double... values) {
        assert(values != null && values.length > 0);
        Shape shape = Shape.vector(values.length);
        return new VectorDoubleMatrix(shape, values);
    }

    public static VectorDoubleMatrix create(Shape shape) {
        return new VectorDoubleMatrix(shape);
    }

    @Override
    public IDoubleMatrix tryTimes(double scalar) {
        VectorDoubleMatrix res = VectorDoubleMatrix.create(this.shape());
        for(int i = 0; i < this.values.length; i++){
            res.values[i] = this.values[i] * scalar;
        }
        return res;
    }

    @Override
    public double get(int row, int column) {
        shape().assertInShape(row, column);
        return this.values[row];
    }

    @Override
    public double normOne() {
        double res = 0;
        for(int i = 0; i < this.values.length; i++){
            res += Math.abs(this.values[i]);
        }
        return res;
    }

    @Override
    public double normInfinity() {
        double res = this.values[0];
        for(int i = 0; i < this.values.length; i++){
            res = Math.max(res, Math.abs(this.values[i]));
        }
        return res;
    }

    @Override
    public double frobeniusNorm() {
        double res = 0;
        for(int i = 0; i < this.values.length; i++){
            res += this.values[i] * this.values[i];
        }
        return Math.sqrt(res);
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
