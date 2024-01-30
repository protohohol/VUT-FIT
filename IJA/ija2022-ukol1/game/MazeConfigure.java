package ija.ija2022.homework1.game;

import ija.ija2022.homework1.common.Field;
import ija.ija2022.homework1.common.Maze;

public class MazeConfigure {
    private int rows, cols, curRow=1;
    private Field[][] board;
    private boolean started=false, error=false;

    public MazeConfigure() {}
    public void startReading(int rows, int cols) {
        this.rows = rows+2;
        this.cols = cols+2;
        board = new Field[this.rows][this.cols];
        for (int i = 0; i < this.cols; i++) {
            board[0][i] = new WallField(0, i);
            board[this.rows-1][i] = new WallField(this.rows-1, i);
        }
        for (int i = 1; i < this.rows-1; i++) {
            board[i][0] = new WallField(i, 0);
            board[i][this.cols-1] = new WallField(i, this.cols-1);
        }
        this.started = true;
    }

    public boolean processLine(String line) {
        if (started && !error && curRow <= rows-1 && line.length() == cols-2) {
            for (int i = 1; i < line.length()+1; i++) {
                switch (line.charAt(i-1)) {
                    case '.':
                        board[this.curRow][i] = new PathField(this.curRow, i);
                        break;

                    case 'S':
                        board[this.curRow][i] = new PathField(this.curRow, i);
                        PacmanObject pacman = new PacmanObject(board[this.curRow][i]);
                        board[this.curRow][i].put(pacman);
                        break;

                    case 'X':
                        board[this.curRow][i] = new WallField(this.curRow, i);
                        break;

                    default:
                        return false;
                }
            }
            this.curRow++;
            return true;
        }
        error = true;
        return false;
    }

    public boolean stopReading() {
        if (started && !error) {
            started = false;
            return true;
        }
        return false;
    }

    public Maze createMaze() {
        if (error) {
            return null;
        } else {
            PacmanMaze maze = new PacmanMaze(this.rows, this.cols);
            maze.board = board;
            for (int i = 1; i < this.rows; i++) {
                for (int k = 1; k < this.cols-1; k++) {
                    maze.board[i][k].setMaze(maze);
                }
            }
            return maze;
        }
    }
}
