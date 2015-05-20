package assign1.minesweeper.ui;

import assign1.minesweeper.MineSweeper;
import assign1.minesweeper.MineSweeper.GameState;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class MineSweeperFrame extends JFrame {

    private final static int MAX_ROWS = MineSweeper.MAX_ROWS;
    private final static int MAX_COLUMNS = MineSweeper.MAX_COLUMNS;

    private MineSweeper mineSweeper;
    private MineSweeperCell cells[][];

    @Override
    protected void frameInit() {
        super.frameInit();
        mineSweeper = MineSweeper.create();
        cells = new MineSweeperCell[MAX_ROWS][MAX_COLUMNS];
        initializeCells();

        for (int i = 0; i < MAX_ROWS; i++) {
            for (int j = 0; j < MAX_COLUMNS; j++) {
                getContentPane().add(cells[i][j]);
                cells[i][j].addMouseListener(new CellClickedHandler());
            }
        }
    }

    public static void main(String[] args) {
        JFrame frame = new MineSweeperFrame();
        frame.setSize(500, 500);
        frame.setLayout(new GridLayout(MAX_ROWS, MAX_COLUMNS));
        frame.setVisible(true);
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
    }

    private void initializeCells() {
        for(int i = 0; i < MAX_ROWS; i++) {
            for (int j = 0; j < MAX_COLUMNS; j++) {
                cells[i][j] = new MineSweeperCell(i, j);
            }
        }
    }

    private class CellClickedHandler extends MouseAdapter {
        public void mouseClicked(MouseEvent mouseEvent) {
            MineSweeperCell cell;

            if (SwingUtilities.isLeftMouseButton(mouseEvent)) {
                cell = (MineSweeperCell) mouseEvent.getSource();
                changeGameBoardAfterExpose(cell);
            }
            else if (SwingUtilities.isRightMouseButton(mouseEvent) || mouseEvent.isControlDown()){
                cell = (MineSweeperCell) mouseEvent.getSource();
                changeGameBoardAfterSealOrUnseal(cell);
            }
        }

        private void changeGameBoardAfterExpose(MineSweeperCell cell)
        {
            mineSweeper.exposeAt(cell.row, cell.column);
            cell.setFocusPainted(false);
            displayExposedNeighborsIfAny();
            checkGameStatus(cell);
        }

        private void changeGameBoardAfterSealOrUnseal(MineSweeperCell cell){
            cell.setFocusPainted(false);
            if (mineSweeper.cellStatus(cell.row, cell.column) == MineSweeper.CellState.UNEXPOSED){
                mineSweeper.sealAt(cell.row, cell.column);
                cell.setBackground(Color.YELLOW);
                checkGameStatus(cell);
            }
            else{
                mineSweeper.unsealAt(cell.row, cell.column);
                cell.setBackground(new JButton().getBackground());
            }
        }

        private void checkGameStatus(MineSweeperCell cell) {
            if(mineSweeper.getGameStatus() == GameState.WON){
                JOptionPane.showMessageDialog(cell, "Congratulations. You won!");
            }
            else if (mineSweeper.getGameStatus() == GameState.LOST){
                JOptionPane.showMessageDialog(cell, "Game Over. You Lost!");
                exposeAllMinesIfGameIsLost();
            }
        }

        private void displayExposedNeighborsIfAny(){
            for (int row = 0; row < MAX_ROWS; row++) {
                for (int col = 0; col < MAX_COLUMNS; col++) {
                    if (mineSweeper.cellStatus(row, col) == MineSweeper.CellState.EXPOSED) {
                        if(mineSweeper.isAdjacentCellAt(row, col)) {
                            cells[row][col].setText(Integer.toString(mineSweeper.getNumberOfAdjacentMinesAt(row, col)));
                            cells[row][col].setEnabled(false);
                        }
                        else if(mineSweeper.isEmptyCellAt(row, col)) {
                            cells[row][col].setEnabled(false);
                        }
                        cells[row][col].setBorder(BorderFactory.createDashedBorder(Color.BLACK));
                    }
                }
            }
        }

        private void exposeAllMinesIfGameIsLost(){
            for (int row = 0; row < MAX_ROWS; row++) {
                for (int col = 0; col < MAX_COLUMNS; col++) {
                    if(mineSweeper.isMineAt(row, col)) {
                        cells[row][col].setText("*");
                        cells[row][col].setBackground(Color.RED);
                    }
                }
            }
        }
    }
}