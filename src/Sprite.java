public class Sprite {
    private int id; //Sprite ID
    private double x, y; //Position of Sprite
    private final double STEP = 5;

    public Sprite(int id, double x, double y){
        this.id = id;
        this.x = x;
        this.y = y;
    }

    public void moveUp(){
        if (this.y + STEP < 720){
            y += STEP;
        }
        else{
            y = 720;
        }
    }

    public void moveDown(){
        if (this.y - STEP > 0){
            y -= STEP;
        }
        else{
            y = 0;
        }
    }

    public void moveLeft(){
        if (this.x + STEP > 0){
            x -= STEP;
        }
        else{
            x = 0;
        }
    }

    public void moveRight(){
        if (this.x + STEP < 1280){
            x += STEP;
        }
        else{
            x = 1280;
        }
    }

    public void reset(){
        this.x = 640;
        this.y = 360;
    }
}
