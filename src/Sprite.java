import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.logging.Logger;

import javax.imageio.*;

public class Sprite {
    private int id; //Sprite ID
    private double x, y; //Position of Sprite
    private final double STEP = 5;
    private BufferedImage img;

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

    public void moveUp(){
        if (this.y - STEP > 0){
            y -= STEP;
        }
        else{
            y = 0;
        }
    }

    public void moveDown(){
        if (this.y + STEP < 720){
            y += STEP;
        }
        else{
            y = 720;
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

    public void draw(Graphics g) {
        //scale to be fixed
        g.drawImage(this.img, (int) this.x, (int) this.y, 30, 40,  null);
    }
}
