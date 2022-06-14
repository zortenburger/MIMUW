package pl.edu.mimuw.matrix;

public class FullDoubleMatrix extends DoubleMatrix {
    private double matrix[][];

    private FullDoubleMatrix(Shape shape) {
        super(shape);
        this.matrix = new double[shape.rows][shape.columns];
    }

    @Override
    public IDoubleMatrix tryTimes(double scalar) {
        FullDoubleMatrix res = FullDoubleMatrix.create(this.shape());
        for(int i = 0; i < this.shape().rows; i++){
            for (int j = 0; j < this.shape().columns; j++){
                res.matrix[i][j] =  (scalar) * matrix[i][j];
            }
        }
        return res;
    }

    private FullDoubleMatrix(double[][] values) {
        super(Shape.matrix(values.length, values[0].length));
        this.matrix = values;
    }

    public static FullDoubleMatrix create(Shape shape) {
        return new FullDoubleMatrix(shape);
    }

    public static FullDoubleMatrix create(double[][] values) {
        assert (values != null && values.length > 0 && values[0].length > 0);
        for(int i = 0; i < values.length; i++){
            assert (values[0].length == values[i].length);
        }
        return new FullDoubleMatrix(values);
    }

    public void matrixSet(int x, int y, double value){
            this.matrix[x][y] = value;
    }

    @Override
    public double get(int row, int column) {
        shape().assertInShape(row, column);
        return matrix[row][column];
    }

    @Override
    public double normOne() {
        double[] columns = new double[this.shape().columns];
        double res = 0;
        for (int i = 0; i < columns.length; i++) {
            for (int j = 0; j < this.shape().rows; j++) {
                columns[i] += Math.abs(matrix[j][i]);
            }
            if (i == 0) {
                res = columns[i];
            } else {
                res = Math.max(res, columns[i]);
            }
        }
        return res;
    }

    @Override
    public double normInfinity() {
        double[] rows = new double[this.shape().rows];
        double res = 0;
        for (int i = 0; i < rows.length; i++) {
            for (int j = 0; j < this.shape().columns; j++) {
                rows[i] += Math.abs(matrix[i][j]);
            }
            if (i == 0) {
                res = rows[i];
            } else {
                res = Math.max(res, rows[i]);
            }
        }
        return res;
    }

    @Override
    public double frobeniusNorm() {
        double res = 0;
        for (int i = 0; i < this.shape().rows; i++) {
            for (int j = 0; j < this.shape().columns; j++) {
                res += matrix[i][j] * matrix[i][j];
            }
        }
        return Math.sqrt(res);
    }

    @Override
    public IDoubleMatrix negative() {
        return this.tryTimes(-1);
    }

    @Override
    public IDoubleMatrix copy() {
        FullDoubleMatrix res = FullDoubleMatrix.create(this.shape());
        for (int i = 0; i < this.shape().rows; i++) {
            for (int j = 0; j < this.shape().columns; j++) {
                res.matrix[i][j] = this.matrix[i][j];
            }
        }
        return res;
    }
}
