package pl.edu.mimuw.matrix;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class SparseDoubleMatrix extends DoubleMatrix{
    private MatrixCellValue[] values;

    public SparseDoubleMatrix (Shape shape, MatrixCellValue[] values){
        super(shape);
        this.values = values;
        Arrays.sort(this.values, new CellComparator());
    }

    public SparseDoubleMatrix (Shape shape, int size){
        super(shape);
        this.values = new MatrixCellValue[size];
    }

    public static SparseDoubleMatrix create (Shape shape, MatrixCellValue... values){
        assert(values != null && values.length > 0 && shape != null && shape.rows > 0 && shape.columns > 0);
        for(int i = 0; i < values.length; i++){
            shape.assertInShape(values[i].row,values[i].column);
        }
        return new SparseDoubleMatrix(shape, values);
    }

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other) {
        assert(this.shape().equals(other.shape()));
        if(other.getClass() == SparseDoubleMatrix.class){
            MatrixCellValue[] temp = new MatrixCellValue[this.values.length + ((SparseDoubleMatrix) other).values.length];
            for(int i = 0; i < this.values.length; i++){
                temp[i] = this.values[i];
            }
            for(int i = 0; i <  ((SparseDoubleMatrix) other).values.length; i++){
                temp[i + this.values.length] =  ((SparseDoubleMatrix) other).values[i];
            }
            Arrays.sort(temp, new CellComparator());
            int hm = 1;
            for (int i = 1; i < temp.length; i++){
                if(temp[i].row != temp[i-1].row || temp[i].column != temp[i-1].column){
                    hm++;
                }
            }
            SparseDoubleMatrix res = new SparseDoubleMatrix (this.shape(), hm);
            hm = 0;
            double tempVal = temp[0].value;
            int tempRow = temp[0].row;
            int tempCol = temp[0].column;
            for (int i = 1; i < temp.length; i++){
                if(temp[i].row != tempRow || temp[i].column != tempCol){
                    res.values[hm] = MatrixCellValue.cell(tempRow, tempCol, tempVal);
                    hm++;
                    tempRow = temp[i].row;
                    tempCol = temp[i].column;
                    tempVal = temp[i].value;
                }
                else{
                    tempVal += temp[i].value;
                }
            }
            res.values[hm] = MatrixCellValue.cell(tempRow, tempCol, tempVal);
            return res;
        }
        return super.plus(other);
    }

    @Override
    public IDoubleMatrix times(IDoubleMatrix other) {
        assert(this.shape().columns == other.shape().rows);
        if(other.getClass() == SparseDoubleMatrix.class){
            List <MatrixCellValue> temp = new ArrayList<MatrixCellValue>();
            Shape shape = Shape.matrix(this.shape().rows, other.shape().columns);
            for(int i = 0; i < this.values.length; i++){
                int x = this.values[i].row;
                int k = this.values[i].column;
                double val1 = this.values[i].value;
                int first = 0, last = 0;
                first = ((SparseDoubleMatrix)other).bs(k);
                last = ((SparseDoubleMatrix)other).bs(k+1);
                for(int j = first; j < last; j++){
                    int y = ((SparseDoubleMatrix) other).values[j].column;
                    double val2 = ((SparseDoubleMatrix) other).values[j].value;
                    temp.add(MatrixCellValue.cell(x, y,val1 * val2));
                }
            }
            Collections.sort(temp, new CellComparator());
            int hm = 1;
            for (int i = 1; i < temp.size(); i++){
                if(temp.get(i).row != temp.get(i-1).row || temp.get(i).column != temp.get(i-1).column){
                    hm++;
                }
            }
            SparseDoubleMatrix res = new SparseDoubleMatrix (shape, hm);
            hm = 0;
            double tempVal = temp.get(0).value;
            int tempRow = temp.get(0).row;
            int tempCol = temp.get(0).column;
            for (int i = 1; i < temp.size(); i++){
                if(temp.get(i).row != tempRow || temp.get(i).column != tempCol){
                    res.values[hm] = MatrixCellValue.cell(tempRow, tempCol, tempVal);
                    hm++;
                    tempRow = temp.get(i).row;
                    tempCol = temp.get(i).column;
                    tempVal = temp.get(i).value;
                }
                else{
                    tempVal += temp.get(i).value;
                }
            }
            res.values[hm] = MatrixCellValue.cell(tempRow, tempCol, tempVal);
            return res;
        }
        return super.times(other);
    }

    @Override
    public IDoubleMatrix tryTimes(double scalar){
        SparseDoubleMatrix res = SparseDoubleMatrix.create(this.shape(), this.values.clone());
        for(int i = 0; i < res.values.length; i++){
            res.values[i] = MatrixCellValue.cell(res.values[i].row, res.values[i].column, res.values[i].value * scalar);
        }
        return res;
    }

    @Override
    public double get(int row, int column){
        shape().assertInShape(row, column);
        for(int i = 0; i < this.values.length; i++){
            if(this.values[i].row == row && this.values[i].column == column){
                return this.values[i].value;
            }
        }
        return 0;
    }

    @Override
    public double[][] data(){
        double [][] matrix = new double[this.shape().rows][this.shape().columns];
        for(int i = 0; i < this.values.length; i++){
            matrix[this.values[i].row][this.values[i].column] = this.values[i].value;
        }
        return matrix;
    }

    @Override
    public double normOne(){
        double[] columns = new double[this.shape().columns];
        double res = 0;
        for(int i = 0; i < this.values.length; i++){
            columns[this.values[i].column] += Math.abs(this.values[i].value);
        }
        for (int i = 0; i < columns.length; i++) {
            res = Math.max(res, columns[i]);
        }
        return res;
    }

    @Override
    public double normInfinity(){
        double[] rows = new double[this.shape().rows];
        double res = 0;
        for(int i = 0; i < this.values.length; i++){
            rows[this.values[i].row] += Math.abs(this.values[i].value);
        }
        for (int i = 0; i < rows.length; i++) {
            res = Math.max(res, rows[i]);
        }
        return res;
    }

    @Override
    public double frobeniusNorm(){
        double res = 0;
        for(int i = 0; i < this.values.length; i++){
            res += Math.pow(this.values[i].value, 2);
        }
        return Math.sqrt(res);
    }

    @Override
    public String toString(){
        String res = this.shape().rows + " " + this.shape().columns + ":\n";
        for(int i = 0; i < this.values.length; i++){
            res += this.values[i] + "\n";
        }
        return res;
    }

    @Override
     public IDoubleMatrix negative(){
        return this.tryTimes(-1);
    }

    @Override
     public IDoubleMatrix copy(){
        return this.tryTimes(1);
    }

    public int bs(int k){
        int l = 0, p = this.values.length;
        while(l < p){
            int s = (l+p)/2;
            if(this.values[s].row < k){
                l = s + 1;
            }
            else{
                p = s;
            }
        }
        return p;
    }

}
