import java.awt.*;
import java.awt.geom.Line2D;
import java.util.List;

public class Particle {
    private double x, y; // Position
    private double angle; // Angle in degrees
    private double velocity; // Velocity in pixels per second
    private static final int SIZE = 5; // Size of the particle

    public Particle(double x, double y, double angle, double velocity) {
        this.x = x;
        this.y = y;
        this.angle = angle;
        this.velocity = velocity;
    }

    public void moveParticle(List<Wall> walls, double deltaTime) {
        double dx = velocity * Math.cos(Math.toRadians(angle)) * deltaTime;
        double dy = velocity * Math.sin(Math.toRadians(angle)) * deltaTime;

        double updatedX = x + dx;
        double updatedY = y + dy;

        double adjustedParticleY = ParticleSimulatorController.CANVAS_HEIGHT - y;
        double adjustedUpdatedParticleY = ParticleSimulatorController.CANVAS_HEIGHT - updatedY;

        Line2D particlePath = new Line2D.Double(x, adjustedParticleY, updatedX, adjustedUpdatedParticleY);

        for (Wall wall : walls) {
            double startX = wall.getStartX();
            double startY = wall.getStartY();
            double endX = wall.getEndX();
            double endY = wall.getEndY();

            // Calculate intersection point between particle path and wall
            double[] intersectionPoint = getIntersectionPoint(startX, startY, endX, endY, x, y, updatedX, updatedY);

            if (intersectionPoint != null) {
                double intersectionX = intersectionPoint[0];
                double intersectionY = intersectionPoint[1];

                // Calculate the normal vector of the wall
                double wallNormalAngle = Math.atan2(endY - startY, endX - startX);

                // Calculate the angle of incidence
                double angleOfIncidence = Math.atan2(intersectionY - y, intersectionX - x);

                // Calculate the angle of reflection
                double reflectedAngle = 2 * wallNormalAngle - angleOfIncidence;

                // Update particle position
                x = intersectionX + Math.cos(reflectedAngle) * deltaTime;
                y = intersectionY + Math.sin(reflectedAngle) * deltaTime;

                // Update particle angle
                angle = Math.toDegrees(reflectedAngle);

                return; // Exit method to prevent multiple collisions in the same step
            }
        }

        // Update particle position if no collisions occurred
        x = updatedX;
        y = updatedY;
    }

    // Method to calculate intersection point between line segment and particle path
    private double[] getIntersectionPoint(double startX, double startY, double endX, double endY, double particleX, double particleY, double updatedParticleX, double updatedParticleY) {
        double[] intersectionPoint = new double[2];

        double a = (endX - startX) * (startY - particleY) - (endY - startY) * (startX - particleX);
        double b = (endX - startX) * (updatedParticleY - particleY) - (endY - startY) * (updatedParticleX - particleX);
        double c = (updatedParticleX - particleX) * (startY - particleY) - (updatedParticleY - particleY) * (startX - particleX);

        double alpha = a / b;
        double beta = c / b;


        if (b == 0) {
            return null; // Lines are parallel
        }

        if (alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1) {
            // Intersection point lies within both line segments
            intersectionPoint[0] = particleX + alpha * (updatedParticleX - particleX);
            intersectionPoint[1] = particleY + alpha * (updatedParticleY - particleY);
            return intersectionPoint;
        } else {
            return null; // No intersection
        }
    }

    public void draw(Graphics g) {
        // Adjust y position to reflect the inverted coordinate system
        double adjustedY = ParticleSimulatorController.CANVAS_HEIGHT - y;
        g.fillOval((int) x, (int) adjustedY, SIZE, SIZE);
    }
}
