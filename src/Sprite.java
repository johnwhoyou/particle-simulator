import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.logging.Logger;

import javax.imageio.*;

public class Sprite {
    private transient int id; //Sprite ID
    private double x, y; //Position of Sprite
    private final int WIDTH = 25;   //Change to scaled value
    private final int HEIGHT =  25;  //Change to scaled value
    private transient final double STEP = 5;
    private transient BufferedImage img;

    public Sprite(int id, double x, double y){
        this.id = id;
        this.x = x;
        this.y = y;
        this.img = loadImage();
    }

    private BufferedImage loadImage(){
        try {
            img = ImageIO.read(new FileInputStream("res/sprite.png"));
        }
        catch(IOException e){
            Logger logger = java.util.logging.Logger.getLogger(this.getClass().getName());
            logger.info("IOException"+ e);
        }
        return img;
    }

    public int getId() {
        return id;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    public void moveUp(){
        if ((this.y - (HEIGHT / 2)) + STEP < 720){
            y += STEP;
        }
        else{
            y = 720 + (HEIGHT / 2);
        }
    }

    public void moveDown(){
        if ((this.y - (HEIGHT / 2)) - STEP > 0){
            y -= STEP;
        }
        else{
            y = 0 + (HEIGHT / 2);
        }
    }

    public void moveLeft(){
        if ((this.x + (WIDTH / 2)) - STEP > 0){
            x -= STEP;
        }
        else{
            x = 0 - (WIDTH / 2);
        }
    }

    public void moveRight(){
        if ((this.x + (WIDTH / 2)) + STEP < 1280){
            x += STEP;
        }
        else{
            x = 1280 - (WIDTH / 2);
        }
    }

    public void reset(){
        this.x = 640;
        this.y = 360;
    }

    public void draw(Graphics g) {
        //scale to be fixed
        double adjustedY = ParticleSimulatorController.CANVAS_HEIGHT - y;
        g.drawImage(this.img, (int) this.x - 5, (int) adjustedY, WIDTH, HEIGHT,  null);
    }
}
