package assign1.minesweeper.ui;

import assign1.minesweeper.MineSweeper;

import javax.swing.*;

public class MineSweeperCell extends JButton {
    public final int row;
    public final int column;

    public MineSweeperCell(int theRow, int theColumn) {
        row = theRow;
        column = theColumn;
        setSize(5, 5);
    }
}
