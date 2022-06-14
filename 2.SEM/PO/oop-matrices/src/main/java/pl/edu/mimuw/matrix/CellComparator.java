package pl.edu.mimuw.matrix;

import java.util.Comparator;

public class CellComparator implements Comparator<MatrixCellValue> {
    @Override
    public int compare(MatrixCellValue obj1, MatrixCellValue obj2) {
        int r1 = ((MatrixCellValue) obj1).row;
        int r2 = ((MatrixCellValue) obj2).row;
        int c1 = ((MatrixCellValue) obj1).column;
        int c2 = ((MatrixCellValue) obj2).column;

        if (r1 > r2) {
            return 1;
        }
        if (r1 < r2){
            return -1;
        }
        if (c1 > c2) {
            return 1;
        }
        if (c1 < c2){
            return -1;
        } else
            return 0;
    }
}
