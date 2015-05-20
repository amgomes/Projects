package assign1.minesweeper;

import java.util.Random;

public class MineSweeper
{
    public final static int MAX_ROWS = 10;
    public final static int MAX_COLUMNS = 10;
    public final static int SIZE = 10;

    public enum CellState {UNEXPOSED, EXPOSED, SEALED};
    public enum GameState {IN_PROGRESS, LOST, WON};

    CellState[][] cellStates = new CellState[MAX_ROWS][MAX_COLUMNS];
    boolean[][] mines = new boolean[MAX_ROWS][MAX_COLUMNS];

    public MineSweeper()
    {
        for (int row = 0; row < MAX_ROWS; row++) {
            for (int col = 0; col < MAX_COLUMNS; col++) {
                cellStates[row][col] = CellState.UNEXPOSED;
                mines[row][col] = false;
            }
        }
    }

    public static MineSweeper create() {
      MineSweeper mineSweeper = new MineSweeper();
      mineSweeper.createMines(SIZE);
      return mineSweeper;
    }


    public CellState cellStatus(int row, int column)
    {
        return cellStates[row][column];
    }

    public boolean isMineAt(int row, int column)
    {
        if (((row > MAX_ROWS - 1) || (column > MAX_COLUMNS - 1) || (row < 0) || (column < 0)))
            return false;    
        else
            return mines[row][column];
    }

    public boolean isAdjacentCellAt(int row, int column)
    {
        return (getNumberOfAdjacentMinesAt(row, column) != 0 && !isMineAt(row, column));
    }

    public boolean isEmptyCellAt(int row, int column)
    {
        return ((getNumberOfAdjacentMinesAt(row, column) == 0) && !isMineAt(row, column));
    }

    public void exposeAt(int row, int column)
    {
        verifyBounds(row, column);

        if (cellStates[row][column] == CellState.UNEXPOSED) {
            cellStates[row][column] = CellState.EXPOSED;

            if (!isMineAt(row, column)) {
                if (getNumberOfAdjacentMinesAt(row, column) == 0) {
                    tryToExposeNeighboringCell(row - 1, column - 1);
                    tryToExposeNeighboringCell(row - 1, column);
                    tryToExposeNeighboringCell(row - 1, column + 1);
                    tryToExposeNeighboringCell(row, column - 1);
                    tryToExposeNeighboringCell(row, column + 1);
                    tryToExposeNeighboringCell(row + 1, column - 1);
                    tryToExposeNeighboringCell(row + 1, column);
                    tryToExposeNeighboringCell(row + 1, column + 1);
                }
            }
        }
    }

    public void verifyBounds(int row, int column)
    {
        if ((row > MAX_ROWS - 1) || (column > MAX_COLUMNS - 1) || (row < 0) || (column < 0)) {
            throw new ArrayIndexOutOfBoundsException("Index is out of bound");
        }
    }

    public void tryToExposeNeighboringCell(int row, int column)
    {
        if (!((row > MAX_ROWS - 1) || (column > MAX_COLUMNS - 1) || (row < 0) || (column < 0)))
            exposeAt(row, column);
    }

    public void sealAt(int row, int column)
    {
        verifyBounds(row, column);
        if (cellStates[row][column] == CellState.UNEXPOSED)
            cellStates[row][column] = CellState.SEALED;
    }

    public void unsealAt(int row, int column)
    {
        verifyBounds(row, column);
        if(cellStates[row][column] == CellState.SEALED){
            cellStates[row][column] = CellState.UNEXPOSED;
        }
    }

    private void createMines(int numberOfMines)
    {
        Random rand = new Random();
        int count = 0;

        while (count < SIZE) {
            int randomRow = rand.nextInt(MAX_ROWS);
            int randomCol = rand.nextInt(MAX_COLUMNS);
            if (!mines[randomRow][randomCol]) {
                mines[randomRow][randomCol] = true;
                count++;
            }
        }
    }

    public int getNumberOfAdjacentMinesAt(int row, int column)
    {
        int numberOfAdjacentMines = 0;

        for (int i = -1; i <2; i++) {
            for (int j = -1; j < 2; j++ ) {
                if (!((row + i > MAX_ROWS - 1) || (column + j > MAX_COLUMNS - 1) 
                    || (row + i < 0) || (column + j < 0))) {
                    if (!(i == 0 && j == 0)) {
                        if (isMineAt(row + i, column + j)) {
                            numberOfAdjacentMines++;
                        }
                    }
                }
            }
        }
        return numberOfAdjacentMines;
    }

    public GameState getGameStatus()
    {
        GameState gameState = GameState.WON;

        for (int row = 0; row < MAX_ROWS; row++) {
            for (int col = 0; col < MAX_COLUMNS; col++) {
                if (mines[row][col]) {
                    if (cellStates[row][col] == CellState.EXPOSED)
                        return GameState.LOST;
                    else if (cellStates[row][col] != CellState.SEALED)
                        gameState = GameState.IN_PROGRESS;
                }
                else if (cellStates[row][col] == CellState.UNEXPOSED)
                    gameState = GameState.IN_PROGRESS;
            }

        }
        return gameState;
    }
}
