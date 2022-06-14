package pl.edu.mimuw.matrix;

import org.jetbrains.annotations.Contract;
import org.jetbrains.annotations.NotNull;

public class RowDoubleMatrix extends DoubleMatrix{
    double[] row;

    public RowDoubleMatrix(Shape shape, double... row) {
        super(shape);
        this.row = row;
    }


    public static RowDoubleMatrix create(int rows, double... row){
        assert(rows > 0 && row != null && row.length > 0);
        Shape shape = Shape.matrix(rows, row.length);
        return new RowDoubleMatrix(shape, row);
    }

    @Override
    public IDoubleMatrix tryTimes(double scalar) {
       RowDoubleMatrix res = new RowDoubleMatrix(this.shape(), this.row);
        for(int i = 0; i < this.row.length ; i++){
            this.row[i] *= scalar;
        }
        return res;
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other){
        assert (this.shape().equals(other.shape()));
        if(other.getClass() == RowDoubleMatrix.class){
            RowDoubleMatrix res = (RowDoubleMatrix) this.copy();
            for(int i = 0; i < row.length ; i++){
                res.row[i] += ((RowDoubleMatrix) other).row[i];
            }
            return res;
        }
        return super.plus(other);
    }

    @Override
    public IDoubleMatrix plus(double scalar){
        RowDoubleMatrix res = (RowDoubleMatrix) this.copy();
        for(int i = 0; i < row.length ; i++){
            res.row[i] += scalar;
        }
        return res;
    }

    @Override
    public double get(int row, int column) {
        shape().assertInShape(row, column);
        return this.row[column];
    }

    @Override
    public double normOne() {
        double res = 0;
        for(int i = 0; i < row.length; i++){
            res = res + Math.abs(row[i]);
        }
        return res;
    }

    @Override
    public double normInfinity() {
        double res = 0;
        for(int i = 0; i < row.length; i++){
            res = Math.max(res, Math.abs(row[i]));
        }
        return res * this.shape().rows;

    }

    @Override
    public double frobeniusNorm() {
        double res = 0;
        for(int i = 0; i < row.length; i++){
            res = res + Math.abs(row[i]);
        }
        return res * this.shape().rows;
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
            for(int j = 0; j < row.length; j++){
                res += row[j] + " ";
            }
            res += "\n";
        }
        return res;
    }
}
