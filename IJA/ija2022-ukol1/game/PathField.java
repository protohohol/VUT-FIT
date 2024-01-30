package ija.ija2022.homework1.game;

import ija.ija2022.homework1.common.Field;
import ija.ija2022.homework1.common.Maze;
import ija.ija2022.homework1.common.MazeObject;

import java.util.Objects;

public class PathField extends Object implements Field {
    private int row, col;
    private MazeObject obj = null;
    private Maze maze;

    public PathField(int row, int col) {
        this.row = row;
        this.col = col;
    }
    @Override
    public void setMaze(Maze maze) {
        this.maze = maze;
    }

    @Override
    public Field nextField(Direction dirs) {
        switch (dirs) {
            case D:
                return this.maze.getField(row+1, col);
            case L:
                return this.maze.getField(row, col-1);
            case R:
                return this.maze.getField(row, col+1);
            case U:
                return this.maze.getField(row-1, col);
            default:
                return null;
        }
    }

    @Override
    public boolean put(MazeObject object) {
        if (this.obj == null) {
            this.obj = object;
            return true;
        }
        return false;
    }

    @Override
    public boolean remove(MazeObject object) {
        if (this.obj != null) {
            this.obj = null;
            return true;
        }
        return false;
    }

    @Override
    public boolean isEmpty() {
        if (this.obj == null) {
            return true;
        }
        return false;
    }

    @Override
    public MazeObject get() {
        return obj;
    }

    @Override
    public boolean canMove() {
        return true;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        PathField pathField = (PathField) o;
        return row == pathField.row && col == pathField.col;
    }
}
