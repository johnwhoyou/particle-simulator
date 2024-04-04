import java.awt.*;

public class Wall {
    private double startX, startY, endX, endY;

    public Wall(double startX, double startY, double endX, double endY) {
        this.startX = startX;
        this.startY = startY;
        this.endX = endX;
        this.endY = endY;
    }

    public double getStartX() {
        return startX;
    }

    public double getStartY() {
        return startY;
    }

    public double getEndX() {
        return endX;
    }

    public double getEndY() {
        return endY;
    }

    public void draw(Graphics g) {
        double adjustedStartY = ParticleSimulatorController.CANVAS_HEIGHT - getStartY();
        double adjustedEndY = ParticleSimulatorController.CANVAS_HEIGHT - getEndY();
        g.drawLine((int)getStartX(), (int) adjustedStartY, (int)getEndX(), (int)adjustedEndY);
    }
}
