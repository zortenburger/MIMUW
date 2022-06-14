package pl.edu.mimuw.matrix;

public abstract class DoubleMatrix implements IDoubleMatrix{
    private Shape shape;

    public DoubleMatrix(Shape shape) {
        this.shape = shape;
    }

    @Override
    public IDoubleMatrix times(IDoubleMatrix other) {
        assert(this.shape().columns == other.shape().rows);
        if(other.getClass() == ZeroDoubleMatrix.class) {
            return other.revTimes(this);
        }
        if(other.getClass() == IdentityDoubleMatrix.class) {
            return other.revTimes(this);
        }
        double[][] thisMatrix = this.data();
        double[][] otherMatrix = other.data();
        Shape shape = Shape.matrix(this.shape().rows, other.shape().columns);
        FullDoubleMatrix res = FullDoubleMatrix.create(shape);
        for(int i = 0; i < this.shape.rows; i++) {
            for(int j = 0; j < other.shape().columns; j++) {
                for(int k = 0; k < this.shape.columns; k++) {
                    res.matrixSet(i, j, thisMatrix[i][k] * otherMatrix[k][j] + res.get(i, j));
                }
            }
        }
        return res;
    }

    @Override
    public IDoubleMatrix revTimes(IDoubleMatrix other){
        return null;
    }

    @Override
    public IDoubleMatrix times(double scalar) {
        if(scalar == 0){
            return ZeroDoubleMatrix.create(this.shape());
        }
        if(scalar == 1){
            return this.copy();
        }
        return this.tryTimes(scalar);
    }

    abstract public IDoubleMatrix tryTimes(double scalar);

    @Override
    public IDoubleMatrix plus(IDoubleMatrix other) {
        assert(this.shape.equals(other.shape()));
        if(other.getClass() == ZeroDoubleMatrix.class) {
            return this.copy();
        }
        double[][] thisMatrix = this.data();
        double[][] otherMatrix = other.data();
        Shape shape = Shape.matrix(this.shape.rows, this.shape.columns);
        FullDoubleMatrix res = FullDoubleMatrix.create(shape);
        for(int i = 0; i < this.shape.rows; i++) {
            for(int j = 0; j < this.shape.columns; j++) {
                res.matrixSet(i, j, thisMatrix[i][j] + otherMatrix[i][j]);
            }
        }
        return res;
    }

    @Override
    public IDoubleMatrix plus(double scalar) {
        Shape shape = Shape.matrix(this.shape.rows, this.shape.columns);
        FullDoubleMatrix res = FullDoubleMatrix.create(shape);
        for(int i = 0; i < this.shape.rows; i++) {
            for(int j = 0; j < this.shape.columns; j++) {
                res.matrixSet(i, j, this.get(i, j) + scalar);
            }
        }
        return res;
    }

    @Override
    public IDoubleMatrix minus(IDoubleMatrix other) {
        return this.plus(other.negative());
    }

    @Override
     public IDoubleMatrix minus(double scalar){
        return this.plus((-1) * scalar);
    }

    @Override
    abstract public double get(int row, int column);

    @Override
    public double[][] data(){
        double [][] matrix = new double[this.shape.rows][this.shape.columns];
        for(int i = 0; i < this.shape.rows; i++) {
            for(int j = 0; j < this.shape.columns; j++) {
                matrix[i][j] = this.get(i, j);
            }
        }
        return matrix;
    }

    @Override
    abstract public double normOne();

    @Override
    abstract public double normInfinity();

    @Override
    abstract public double frobeniusNorm();

    @Override
    public Shape shape() {
        return this.shape;
    }

    @Override
    public String toString(){
        String res = this.shape().rows + " " + this.shape().columns + ":\n";
        double [][] matrix = this.data();
        for(int i = 0; i < this.shape().rows; i++){
            for(int j = 0; j < this.shape().columns; j++){
                res += matrix[i][j] + " ";
            }
            res += "\n";
        }
        return res;
    }

    @Override
    abstract public IDoubleMatrix negative();

    @Override
    abstract public IDoubleMatrix copy();

}
