package pl.edu.mimuw.matrix;

public class ZeroDoubleMatrix extends ConstantDoubleMatrix{

    public ZeroDoubleMatrix(Shape shape) {
        super(shape, 0);
    }

    public static IDoubleMatrix create (Shape shape){
        assert(shape != null && shape.rows >0 && shape.columns > 0);
        return new ZeroDoubleMatrix(shape);
    }
    @Override
    public IDoubleMatrix times(IDoubleMatrix other){
        assert(this.shape().columns == other.shape().rows);
        return ZeroDoubleMatrix.create(Shape.matrix(this.shape().rows, other.shape().columns));
    }

    public IDoubleMatrix revTimes(IDoubleMatrix other){
        return ZeroDoubleMatrix.create(Shape.matrix(other.shape().rows, this.shape().columns));
    }

    @Override
    public IDoubleMatrix tryTimes(double scalar){
        return this.copy();
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other){
        assert(this.shape().equals(other.shape()));
        return other.copy();
    }

    @Override
    public double normOne(){
        return 0;
    }

    @Override
    public double normInfinity(){
        return 0;
    }

    @Override
    public double frobeniusNorm(){
        return 0;
    }

    public IDoubleMatrix negative(){
        return this.copy();
    }

    public IDoubleMatrix copy(){
        return ZeroDoubleMatrix.create(this.shape());
    }
}
