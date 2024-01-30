package ija.ija2022.homework1.common;

public interface Field {
    public static enum Direction {
        D,
        L,
        R,
        U
    }

    void setMaze(Maze maze);
    Field nextField(Field.Direction dirs);
    boolean put(MazeObject object);
    boolean remove(MazeObject object);
    boolean isEmpty();
    MazeObject get();
    boolean canMove();
}