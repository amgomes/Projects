package assign1.minesweeper;

import junit.framework.TestCase;

import java.util.*;
import assign1.minesweeper.MineSweeper.CellState;
import assign1.minesweeper.MineSweeper.GameState;

public class MineSweeperTest extends TestCase
{
    private MineSweeper mineSweeperCommon, minesweeperToDesignExpose, minesweeperToDesignExposeNeighbor;
    private List<Integer> neighborCells;
    private boolean thisExposeAtCalled;

    public void testCanary()
    {
        assertTrue(true);
    }

    public void setUp()
    {        
        neighborCells = new ArrayList<Integer>();

        mineSweeperCommon = new MineSweeper();

        minesweeperToDesignExpose = new MineSweeper() {
            public void tryToExposeNeighboringCell(int row, int column) {
                neighborCells.add(row);
                neighborCells.add(column);
            }
        };

        minesweeperToDesignExposeNeighbor = new MineSweeper(){
            public void exposeAt(int row, int column) {
                thisExposeAtCalled = true;
            }
        };
    }

    public void testAnUnexposedCellCanBeExposed()
    {
        mineSweeperCommon.exposeAt(2, 6);
        assertEquals(MineSweeper.CellState.EXPOSED, mineSweeperCommon.cellStatus(2, 6));
    }

    public void testAnExposedCellCannotBeExposed()
    {
        mineSweeperCommon.exposeAt(2, 5);
        mineSweeperCommon.exposeAt(2, 5);
        assertEquals(CellState.EXPOSED, mineSweeperCommon.cellStatus(2, 5));
    }

    private void checkBounds(Runnable block) {
      try {
          block.run();
          fail("Expected exception for row index out of bound");
      }
      catch(ArrayIndexOutOfBoundsException ex) {
          assertTrue(true);
      }      
    }
    
    public void testRowOutOfBoundThrowsExceptionForExposeAt()
    {
      checkBounds(() -> mineSweeperCommon.exposeAt(10, 0));
    }
    
    public void testNegativeRowOutOfBoundThrowsExceptionForExposeAt()
    {
      checkBounds(() -> mineSweeperCommon.exposeAt(-1, 0));
    }
    
    public void testColumnOutOfBoundThrowsExceptionForExposeAt()
    {
      checkBounds(() -> mineSweeperCommon.exposeAt(8, 11));
    }
    
    public void testNegativeColumnOutOfBoundThrowsExceptionForExposeAt()
    {
      checkBounds(() -> mineSweeperCommon.exposeAt(8, -1));
    }    

    public void testExposeTriesToExposeNeighbors()
    {
        List<Integer> expectedNeighbors = Arrays.asList(0, 1, 0, 2, 0, 3, 1, 1, 1, 3, 2, 1, 2, 2, 2, 3);
        minesweeperToDesignExpose.exposeAt(1, 2);
        assertEquals(expectedNeighbors, neighborCells);
    }

    public void testExposeWillNotCallExposeNeighborsIfCellIsAlreadyExposed()
    {
        minesweeperToDesignExpose.exposeAt(1, 2);

        neighborCells.clear();
        minesweeperToDesignExpose.exposeAt(1, 2);
        List<Integer> empty = new ArrayList<>();
        assertEquals(empty, neighborCells);
    }

    public void testTryToExposeNeighborExposesNeighboringCell()
    {
        minesweeperToDesignExposeNeighbor.tryToExposeNeighboringCell(4, 6);
        assertTrue(thisExposeAtCalled);
    }

    public void testTryToExposeNeighborDoesNotExposeOutOfBoundRow()
    {
        minesweeperToDesignExposeNeighbor.tryToExposeNeighboringCell(10, 0);
        assertFalse(thisExposeAtCalled);
    }

    public void testTryToExposeNeighborDoesNotExposeNegativeRow()
    {
        minesweeperToDesignExposeNeighbor.tryToExposeNeighboringCell(-1, 0);
        assertFalse(thisExposeAtCalled);
    }

    public void testTryToExposeNeighborDoesNotExposeOutOfBoundColumn()
    {
        minesweeperToDesignExposeNeighbor.tryToExposeNeighboringCell(0, 10);
        assertFalse(thisExposeAtCalled);
    }

    public void testTryToExposeNeighborDoesNotExposeNegativeColumn()
    {
        minesweeperToDesignExposeNeighbor.tryToExposeNeighboringCell(0, -1);
        assertFalse(thisExposeAtCalled);
    }
    
    public void testUnexposedCellCanBeSealed()
    {
        mineSweeperCommon.sealAt(4, 5);
        assertEquals(CellState.SEALED, mineSweeperCommon.cellStatus(4, 5));
    }

    public void testExposedCellCannotBeSealed()
    {
        mineSweeperCommon.exposeAt(4, 5);
        mineSweeperCommon.sealAt(4, 5);
        assertEquals(CellState.EXPOSED, mineSweeperCommon.cellStatus(4, 5));
    }

    public void testSealedCellCannotBeSealed()
    {
        mineSweeperCommon.sealAt(3, 9);
        mineSweeperCommon.sealAt(3, 9);
        assertEquals(CellState.SEALED, mineSweeperCommon.cellStatus(3, 9));
    }

    public void testASealedCellCannotBeExposed()
    {
        mineSweeperCommon.sealAt(6, 2);
        mineSweeperCommon.exposeAt(6, 2);
        assertEquals(CellState.SEALED, mineSweeperCommon.cellStatus(6, 2));
    }

    public void testRowOutOfBoundsCannotBeSealed()
    {
        checkBounds(() -> mineSweeperCommon.sealAt(10, 5));
    }

    public void testNegativeRowCannotBeSealed()
    {
        checkBounds(() -> mineSweeperCommon.sealAt(-1, 5));
    }

    public void testColumnsOutOfBoundsCannotBeSealed()
    {
        checkBounds(() -> mineSweeperCommon.sealAt(5, 10));
    }

    public void testNegativeColumnsCannotBeSealed()
    {
        checkBounds(() -> mineSweeperCommon.sealAt(5, -1));
    }

    public void testExposingASealedCellDoesNotTryToExposeNeighbors()
    {
        minesweeperToDesignExposeNeighbor.sealAt(5, 6);
        minesweeperToDesignExposeNeighbor.exposeAt(5, 6);
        List<Integer> emptyList = new ArrayList<Integer>();
        assertEquals(emptyList, neighborCells);
    }

    public void testSealedCellCanBeUnsealed()
    {
        mineSweeperCommon.sealAt(4, 5);
        mineSweeperCommon.unsealAt(4, 5);
        assertEquals(CellState.UNEXPOSED, mineSweeperCommon.cellStatus(4, 5));
    }

    public void testUnsealedCellCannotBeUnsealed()
    {
        mineSweeperCommon.unsealAt(4, 5);
        assertEquals(CellState.UNEXPOSED, mineSweeperCommon.cellStatus(4, 5));
    }

    public void testExposedCellCannotBeUnsealed()
    {
        mineSweeperCommon.exposeAt(4, 5);
        mineSweeperCommon.unsealAt(4, 5);
        assertEquals(CellState.EXPOSED, mineSweeperCommon.cellStatus(4, 5));
    }

    public void testRowsOutOfBoundCannotBeUnsealed()
    {
        checkBounds(() -> mineSweeperCommon.unsealAt(10, 2));
    }

    public void testNegativeRowsCannotBeUnsealed()
    {
        checkBounds(() -> mineSweeperCommon.unsealAt(-1, 2));
    }

    public void testColumnsOutOfBoundCannotBeUnsealed()
    {
        checkBounds(() -> mineSweeperCommon.unsealAt(2, 10));
    }

    public void testNegativeColumnsCannotBeUnsealed()
    {
        checkBounds(() -> mineSweeperCommon.unsealAt(1, -2));
    }

    public void testMinedCellIsLabeledAMine()
    {
        mineSweeperCommon.mines[2][3] = true;
        assertTrue(mineSweeperCommon.isMineAt(2, 3));
    }

    public void testNonMinedCellIsNotLabeledAMine()
    {
        mineSweeperCommon.mines[2][3] = false;
        assertFalse(mineSweeperCommon.isMineAt(2, 3));
    }

    public void testOutOfBoundRowCoordinatesIsNotAMine()
    {
        assertFalse(mineSweeperCommon.isMineAt(10, 0));     
    }

    public void testOutOfBoundColumnCoordinatesIsNotAMine()
    {
        assertFalse(mineSweeperCommon.isMineAt(1, 10));     
    }

    public void testNegativeRowCoordinatesIsNotAMine()
    {
        assertFalse(mineSweeperCommon.isMineAt(-1, 0));     
    }

    public void testNegativeColumnCoordinatesIsNotAMine()
    {
        assertFalse(mineSweeperCommon.isMineAt(0, -1));     
    }

    public void testMinedCellCanBeExposed()
    {
        mineSweeperCommon.mines[4][0] = true;
        mineSweeperCommon.exposeAt(4, 0);
        assertEquals(CellState.EXPOSED, mineSweeperCommon.cellStatus(4, 0));
    }

    public void testExposingAMinedCellDoesNotTryToExposeNeighbors()
    {
        minesweeperToDesignExposeNeighbor.mines[0][0] = true;
        minesweeperToDesignExposeNeighbor.exposeAt(0, 0);
        List<Integer> emptyList = new ArrayList<Integer>();
        assertEquals(emptyList, neighborCells);
    }

    public void testTryToExposeNeighborDoesNotExposeMinedNeighbor()
    {
        mineSweeperCommon.mines[7][7] = true;
        mineSweeperCommon.exposeAt(7, 6);
        assertEquals(CellState.UNEXPOSED, mineSweeperCommon.cellStatus(7, 7));
    }

    public void testAMinedCellCanBeSealed()
    {
        mineSweeperCommon.mines[6][9] = true;
        mineSweeperCommon.sealAt(6, 9);
        assertEquals(CellState.SEALED, mineSweeperCommon.cellStatus(6, 9));
    }

    public void testAMinedCellCanBeUnsealed()
    {
        mineSweeperCommon.mines[6][9] = true;
        mineSweeperCommon.sealAt(6, 9);
        mineSweeperCommon.unsealAt(6, 9);
        assertEquals(CellState.UNEXPOSED, mineSweeperCommon.cellStatus(6, 9));
    }

    public void testThereAre10Mines()
    {
        mineSweeperCommon = MineSweeper.create();
        int numberOfMines = 0;
        for (int row = 0 ; row < mineSweeperCommon.MAX_ROWS; row++){
            for (int col = 0; col < mineSweeperCommon.MAX_COLUMNS; col++) {
                if (mineSweeperCommon.isMineAt(row, col))
                    numberOfMines++;
            }
        }
        assertEquals(10, numberOfMines);
    }

    public void testMinesAreRandomlyPlaced()
    {
        MineSweeper mineSweeper1 = MineSweeper.create();
        MineSweeper mineSweeper2 = MineSweeper.create();
        boolean areMinesAtDifferentLocation = false;
        for (int i = 0; i < mineSweeper1.MAX_ROWS; i++) {
            for (int j = 0; j < mineSweeper1.MAX_ROWS; j++) {
                if(mineSweeper1.isMineAt(i, j) && (!mineSweeper2.isMineAt(i,j)))
                    areMinesAtDifferentLocation = true;
            }
        }
        assertTrue(areMinesAtDifferentLocation);
    }

    public void testCellNextToAMineIsAnAdjacentCell()
    {
        mineSweeperCommon.mines[5][4] = true;
        assertTrue(mineSweeperCommon.getNumberOfAdjacentMinesAt(5, 3) > 0);
    }

    public void testNumberOfAdjacentMinesIs0()
    {
        assertEquals(0, mineSweeperCommon.getNumberOfAdjacentMinesAt(4, 5));
    }

    public void testNumberOfAdjacentMinesIs1()
    {
        mineSweeperCommon.mines[4][4] = true;
        assertEquals(1, mineSweeperCommon.getNumberOfAdjacentMinesAt(4, 5));
    }

    public void testNumberOfAdjacentMinesIs2()
    {
        mineSweeperCommon.mines[3][4] = true;
        mineSweeperCommon.mines[4][4] = true;
        assertEquals(2, mineSweeperCommon.getNumberOfAdjacentMinesAt(4, 5));
    }

    public void testNumberOfAdjacentMinesIs3()
    {
        mineSweeperCommon.mines[3][4] = true;
        mineSweeperCommon.mines[3][5] = true;
        mineSweeperCommon.mines[3][6] = true;
        assertEquals(3, mineSweeperCommon.getNumberOfAdjacentMinesAt(4, 5));
    }

    public void testNumberOfAdjacentMinesIs4()
    {
        mineSweeperCommon.mines[3][4] = true;
        mineSweeperCommon.mines[3][5] = true;
        mineSweeperCommon.mines[3][6] = true;
        mineSweeperCommon.mines[4][4] = true;
        assertEquals(4, mineSweeperCommon.getNumberOfAdjacentMinesAt(4, 5));
    }

    public void testNumberOfAdjacentMinesIs5()
    {
        mineSweeperCommon.mines[3][4] = true;
        mineSweeperCommon.mines[3][5] = true;
        mineSweeperCommon.mines[3][6] = true;
        mineSweeperCommon.mines[4][4] = true;
        mineSweeperCommon.mines[4][6] = true;
        assertEquals(5, mineSweeperCommon.getNumberOfAdjacentMinesAt(4, 5));
    }

    public void testNumberOfAdjacentMinesIs6()
    {
        mineSweeperCommon.mines[3][4] = true;
        mineSweeperCommon.mines[3][5] = true;
        mineSweeperCommon.mines[3][6] = true;
        mineSweeperCommon.mines[4][4] = true;
        mineSweeperCommon.mines[4][6] = true;
        mineSweeperCommon.mines[5][4] = true;
        assertEquals(6, mineSweeperCommon.getNumberOfAdjacentMinesAt(4, 5));
    }

    public void testNumberOfAdjacentMinesIs7()
    {
        mineSweeperCommon.mines[3][4] = true;
        mineSweeperCommon.mines[3][5] = true;
        mineSweeperCommon.mines[3][6] = true;
        mineSweeperCommon.mines[4][4] = true;
        mineSweeperCommon.mines[4][6] = true;
        mineSweeperCommon.mines[5][4] = true;
        mineSweeperCommon.mines[5][5] = true;
        assertEquals(7, mineSweeperCommon.getNumberOfAdjacentMinesAt(4, 5));
    }

    public void testNumberOfAdjacentMinesIs8()
    {
        mineSweeperCommon.mines[3][4] = true;
        mineSweeperCommon.mines[3][5] = true;
        mineSweeperCommon.mines[3][6] = true;
        mineSweeperCommon.mines[4][4] = true;
        mineSweeperCommon.mines[4][6] = true;
        mineSweeperCommon.mines[5][4] = true;
        mineSweeperCommon.mines[5][5] = true;
        mineSweeperCommon.mines[5][6] = true;
        assertEquals(8, mineSweeperCommon.getNumberOfAdjacentMinesAt(4, 5));
    }

    public void testAdjacentCellCanBeExposed()
    {
        mineSweeperCommon.mines[4][9] = true;
        mineSweeperCommon.mines[4][8] = false;
        mineSweeperCommon.exposeAt(4, 8);
        assertEquals(CellState.EXPOSED, mineSweeperCommon.cellStatus(4, 8));
    }

    public void testExposingAdjacentCellDoesNotExposeNeighbors()
    {
        minesweeperToDesignExpose.mines[8][8] = true;
        minesweeperToDesignExpose.mines[7][7] = false;
        minesweeperToDesignExpose.exposeAt(7, 7);
        List<Integer> empty = new ArrayList<>();
        assertEquals(empty, neighborCells);
    }

    public void testAdjacentCellCanBeSealed()
    {
        mineSweeperCommon.mines[5][5] = true;
         minesweeperToDesignExpose.mines[5][6] = false;
        mineSweeperCommon.sealAt(5,6);
        assertEquals(CellState.SEALED, mineSweeperCommon.cellStatus(5, 6));
    }

    public void testAdjacentCellCanBeUnSealed()
    {
        mineSweeperCommon.mines[5][5] = true;
        minesweeperToDesignExpose.mines[5][6] = false;
        mineSweeperCommon.sealAt(5,6);
        mineSweeperCommon.unsealAt(5, 6);
        assertEquals(CellState.UNEXPOSED, mineSweeperCommon.cellStatus(5, 6));
    }

    public void testIsAdjacentCellAt(){
        mineSweeperCommon.mines[3][4] = true;
        mineSweeperCommon.mines[3][5] = false;
        assertTrue(mineSweeperCommon.isAdjacentCellAt(3, 5));
    }

    public void testIsNotAdjacentCellAtWhenThereIsMine(){
        mineSweeperCommon.mines[3][4] = true;
        mineSweeperCommon.mines[3][5] = true;
        assertFalse(mineSweeperCommon.isAdjacentCellAt(3, 5));
    }

    public void testIsEmptyCellAt()
    {
        assertTrue(mineSweeperCommon.isEmptyCellAt(4, 5));
    }

    public void testIsNotEmptyCellAtWhenThereIsMine()
    {
        mineSweeperCommon.mines[3][4] = true;
        assertFalse(mineSweeperCommon.isEmptyCellAt(3, 4));
    }

    public void testIsNotEmptyCellAtWhenThereIsAdjacentCell()
    {
        mineSweeperCommon.mines[3][4] = true;
        assertFalse(mineSweeperCommon.isEmptyCellAt(3, 5));
    }   

    public void testGameIsInProgressIfEmptyCellIsExposed()
    {
        mineSweeperCommon = MineSweeper.create();
        mineSweeperCommon.mines[4][5] = false;
        mineSweeperCommon.exposeAt(4,5);
        assertEquals(GameState.IN_PROGRESS, mineSweeperCommon.getGameStatus());
    }

    public void testGameIsInProgressIfACellIsNotExposed()
    {
        assertEquals(GameState.IN_PROGRESS, mineSweeperCommon.getGameStatus());
    }

    public void testGameIsInProgressWhenACellIsSealed()
    {
        mineSweeperCommon.sealAt(2, 3);
        assertEquals(GameState.IN_PROGRESS, mineSweeperCommon.getGameStatus());
    }

    public void testGameIsInProgressWhenACellIsUnSealed()
    {
        mineSweeperCommon.unsealAt(2, 3);
        assertEquals(GameState.IN_PROGRESS, mineSweeperCommon.getGameStatus());
    }

    public void testGameLostWhenAMineIsExposed()
    {
        mineSweeperCommon.mines[0][2] = true;
        mineSweeperCommon.exposeAt(0, 2);
        assertEquals(GameState.LOST, mineSweeperCommon.getGameStatus());
    }

    public void testGameWonWhenAllMinesAreSealedAndAllCellsAreExposed()
    {
        mineSweeperCommon = MineSweeper.create();
        for (int i = 0; i < mineSweeperCommon.MAX_ROWS; i++) {
            for (int j = 0; j < mineSweeperCommon.MAX_COLUMNS; j++) {
                if (mineSweeperCommon.isMineAt(i, j))
                    mineSweeperCommon.cellStates[i][j] = CellState.SEALED;
                else
                    mineSweeperCommon.cellStates[i][j] = CellState.EXPOSED;
            }
        }
        assertEquals(GameState.WON, mineSweeperCommon.getGameStatus());
    }
}
