package ija.ija2022.homework1.game;

import ija.ija2022.homework1.common.Field;
import ija.ija2022.homework1.common.Maze;

public class PacmanMaze extends Object implements Maze {
    private int rows, cols;
    public Field[][] board;

    public PacmanMaze(int rows, int cols) {
        this.rows = rows;
        this.cols = cols;
    }

    public static Maze create(int rows, int cols, Field[][] board) {
        PacmanMaze maze = new PacmanMaze(rows, cols);
        maze.board = board;
        return maze;
    }

    @Override
    public Field getField(int row, int col) {
        return board[row][col];
    }

    @Override
    public int numRows() {
        return rows;
    }

    @Override
    public int numCols() {
        return cols;
    }
}
