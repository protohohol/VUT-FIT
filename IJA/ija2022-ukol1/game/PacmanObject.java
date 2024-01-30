package ija.ija2022.homework1.game;

import ija.ija2022.homework1.common.Field;
import ija.ija2022.homework1.common.MazeObject;

public class PacmanObject extends Object implements MazeObject {
    private Field field;
    private PacmanObject(){}
    public PacmanObject(Field field) {
        PacmanObject pacman = new PacmanObject();
        field.put(pacman);
        this.field = field;
        pacman.field = field;
    }

    @Override
    public boolean canMove(Field.Direction dir) {
        return this.field.nextField(dir).canMove();
    }

    @Override
    public boolean move(Field.Direction dir) {
        if (canMove(dir)) {
            this.field.nextField(dir).put(this.field.get());
            this.field.remove(this.field.get());
            this.field = this.field.nextField(dir);
            return true;
        }
        return false;
    }
}
