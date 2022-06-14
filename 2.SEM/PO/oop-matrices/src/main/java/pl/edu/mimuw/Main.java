package pl.edu.mimuw;

import pl.edu.mimuw.matrix.DoubleMatrixFactory;
import pl.edu.mimuw.matrix.IDoubleMatrix;
import pl.edu.mimuw.matrix.MatrixCellValue;
import pl.edu.mimuw.matrix.Shape;

public class Main {

  public static void main(String[] args) {
      //Pomocnicze tablice do tworzenia macierzy
      MatrixCellValue[] cells = {MatrixCellValue.cell(1,2,7),
              MatrixCellValue.cell(1,7,2), MatrixCellValue.cell(3,5,6),
              MatrixCellValue.cell(4,4,111), MatrixCellValue.cell(7,5,13),
              MatrixCellValue.cell(9,9,8)};

      double[] val = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      double[][] values = {
              {1,2,3,4,5,6,7,8,9,0},
              {1,1,1,1,1,1,1,1,1,1},
              {7,3,7,3,7,3,7,3,7,3},
              {4,2,0,4,2,0,4,2,0,4},
              {2,1,3,7,2,1,3,7,2,1},
              {0,0,0,0,0,0,0,0,0,0},
              {9,6,9,6,9,6,9,6,9,6},
              {5,3,1,2,3,4,5,6,7,6},
              {3,5,3,4,3,2,1,2,3,4},
              {2,2,2,2,2,2,2,2,2,2}
      };

      //Tworzenie przykładowych macierzy każdego typu
      IDoubleMatrix sparse = DoubleMatrixFactory.sparse(Shape.matrix(10,10), cells);
      IDoubleMatrix full = DoubleMatrixFactory.full(values);
      IDoubleMatrix identity = DoubleMatrixFactory.identity(10);
      IDoubleMatrix diagonal = DoubleMatrixFactory.diagonal(val);
      IDoubleMatrix antiDiagonal = DoubleMatrixFactory.antiDiagonal(val);
      IDoubleMatrix vector = DoubleMatrixFactory.vector(val);
      IDoubleMatrix zero = DoubleMatrixFactory.zero(Shape.matrix(10,10));
      IDoubleMatrix constant = DoubleMatrixFactory.constant(Shape.matrix(10,10), 7);
      IDoubleMatrix row = DoubleMatrixFactory.row(10, val);
      IDoubleMatrix column = DoubleMatrixFactory.column(10, val);


      //Testowanie wypisywania
      System.out.println(sparse);
      System.out.println(full);
      System.out.println(identity);
      System.out.println(diagonal);
      System.out.println(antiDiagonal);
      System.out.println(vector);
      System.out.println(zero);
      System.out.println(constant);
      System.out.println(row);
      System.out.println(column);

  }
}
