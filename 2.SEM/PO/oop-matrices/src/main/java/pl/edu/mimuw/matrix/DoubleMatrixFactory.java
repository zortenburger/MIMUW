package pl.edu.mimuw.matrix;

public class DoubleMatrixFactory {

  private DoubleMatrixFactory() {
  }

  public static IDoubleMatrix sparse(Shape shape, MatrixCellValue... values){
    return SparseDoubleMatrix.create(shape, values);
  }

  public static IDoubleMatrix full(double[][] values) {
    return FullDoubleMatrix.create(values);
  }

  public static IDoubleMatrix identity(int size) {
    return IdentityDoubleMatrix.create(Shape.matrix(size,size));
  }

  public static IDoubleMatrix diagonal(double... diagonalValues) {
    return DiagonalDoubleMatrix.create(diagonalValues);
  }

  public static IDoubleMatrix antiDiagonal(double... antiDiagonalValues) {
    return AntiDiagonalDoubleMatrix.create(antiDiagonalValues);
  }

  public static IDoubleMatrix vector(double... values){
    return VectorDoubleMatrix.create(values);
  }

  public static IDoubleMatrix zero(Shape shape) {
    return ZeroDoubleMatrix.create(shape);
  }

  public static IDoubleMatrix constant(Shape shape, double value) {
    return ConstantDoubleMatrix.create(shape, value);
  }

  public static IDoubleMatrix column(int columns, double... column) {
    return ColumnDoubleMatrix.create(columns, column);
  }

  public static IDoubleMatrix row(int rows, double... row) {
    return RowDoubleMatrix.create(rows, row);
  }
}
