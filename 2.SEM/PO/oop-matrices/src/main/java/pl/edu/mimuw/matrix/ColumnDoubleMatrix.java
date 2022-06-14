package pl.edu.mimuw.matrix;

public class ColumnDoubleMatrix extends DoubleMatrix{
    double[] column;

    public ColumnDoubleMatrix(Shape shape, double... column) {
        super(shape);
        this.column = column;
    }

    public static ColumnDoubleMatrix create(int columns, double... column){
        assert(columns > 0 && column != null && column.length > 0);
        Shape shape = Shape.matrix(column.length, columns);
        return new ColumnDoubleMatrix(shape, column);
    }

    @Override
    public IDoubleMatrix tryTimes(double scalar) {
        ColumnDoubleMatrix res = new ColumnDoubleMatrix(this.shape(), this.column);
        for(int i = 0; i < this.column.length ; i++){
            this.column[i] *= scalar;
        }
        return res;
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other){
        assert (this.shape().equals(other.shape()));
        if(other.getClass() == ColumnDoubleMatrix.class){
            ColumnDoubleMatrix res = (ColumnDoubleMatrix) this.copy();
            for(int i = 0; i < column.length ; i++){
                res.column[i] += ((ColumnDoubleMatrix) other).column[i];
            }
            return res;
        }
        return super.plus(other);
    }

    @Override
    public IDoubleMatrix plus(double scalar){
        ColumnDoubleMatrix res = (ColumnDoubleMatrix) this.copy();
        for(int i = 0; i < column.length ; i++){
            res.column[i] += scalar;
        }
        return res;
    }

    @Override
    public double get(int row, int column) {
        shape().assertInShape(row, column);
        return this.column[row];
    }

    @Override
    public double normOne() {
        double res = 0;
        for(int i = 0; i < column.length; i++){
            res = Math.max(res, Math.abs(column[i]));
        }
        return res * this.shape().columns;
    }

    @Override
    public double normInfinity() {
        double res = 0;
        for(int i = 0; i < column.length; i++){
            res = res + Math.abs(column[i]);
        }
        return res;
    }

    @Override
    public double frobeniusNorm() {
        double res = 0;
        for(int i = 0; i < column.length; i++){
            res = res + Math.abs(column[i]);
        }
        return res * this.shape().columns;
    }

    @Override
    public IDoubleMatrix negative(){
        return this.tryTimes(-1);
    }

    @Override
    public IDoubleMatrix copy() {
        return  this.tryTimes(1);
    }

    @Override
    public String toString(){
        String res = shape().rows + " " + shape().columns + ":\n";
        for(int i = 0; i < shape().rows; i++){
                if(this.shape().columns == 1){
                    res += column[i] + "\n";
                }
                if(this.shape().columns == 2){
                    res += column[i] + " " + column[i] + "\n";
                }
                if(this.shape().columns > 2){
                    res +=  column[i] + " ... " + column[i] + "\n";
                }
        }
        return res;
    }
}
