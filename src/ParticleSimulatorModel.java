import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ParticleSimulatorModel {
    private List<Particle> particles = new CopyOnWriteArrayList<>();
    private List<Wall> walls = new CopyOnWriteArrayList<>();
    private List<Sprite> sprites = new CopyOnWriteArrayList<>();
    private final int numThreads;
    ExecutorService executor;

    public ParticleSimulatorModel(int numThreads) {
        this.numThreads = numThreads;

        addWall(0, 0, 1280, 0);
        addWall(0, 720, 1280, 720);
        addWall(0, 0, 0, 720);
        addWall(1280, 0, 1280, 720);

        this.executor = Executors.newFixedThreadPool(numThreads);
    }

    // Multi-threaded moveParticles method
    public void moveParticles(double deltaTime) {
        int particlesPerThread = particles.size() / numThreads;
        int remainingParticles = particles.size() % numThreads;

        int startIndex = 0;

        for (int i = 0; i < numThreads; i++) {
            int particlesToProcess = particlesPerThread + (i < remainingParticles ? 1 : 0);
            int endIndex = startIndex + particlesToProcess;
            List<Particle> particlesSubset = new ArrayList<>(particles.subList(startIndex, endIndex));

            // Submit task to thread pool for processing
            executor.submit(() -> {
                for (Particle particle : particlesSubset) {
                    particle.moveParticle(walls, deltaTime);
                }
            });

            startIndex = endIndex;
        }
    }

    //Adding of Sprites to the list
    public void addSprite(int id){
        sprites.add(new Sprite(id, 640, 360));
    }

    public void addParticle(double x, double y, double angle, double velocity) {
        particles.add(new Particle(x, y, angle, velocity));
    }

    public void addParticleBatchByPoints(int numParticles,
                                         double angle, double velocity,
                                         double startX, double startY,
                                         double endX, double endY) {

        double deltaX = endX - startX;
        double deltaY = endY - startY;
        double totalDistance = Math.sqrt(deltaX * deltaX + deltaY * deltaY);
        double particleSpacing = totalDistance / (numParticles - 1);

        for (int i = 0; i < numParticles; i++) {
            double x;
            double y;
            if (deltaX == 0 && deltaY == 0) {
                x = startX;
                y = startY;
            }
            else {
                x = startX + (deltaX / totalDistance) * particleSpacing * i;
                y = startY + (deltaY / totalDistance) * particleSpacing * i;
            }

            particles.add(new Particle(x, y, angle, velocity));
        }
    }

    public void addParticleBatchByAngles(int numParticles,
                                         double startAngle, double endAngle,
                                         double velocity,
                                         double x, double y) {

        double angleDifference = endAngle - startAngle;
        double angleSpacing = (angleDifference >= 360) ? angleDifference / numParticles : angleDifference / (numParticles - 1);

        for (int i = 0; i < numParticles; i++) {
            double angle = startAngle + angleSpacing * i;
            particles.add(new Particle(x, y, angle, velocity));
        }
    }

    public void addParticleBatchByVelocities(int numParticles,
                                             double startVelocity, double endVelocity,
                                             double angle,
                                             double x, double y) {
        double velocityDifference = endVelocity - startVelocity;
        double velocitySpacing = velocityDifference / (numParticles - 1);

        for (int i = 0; i < numParticles; i++) {
            double velocity = startVelocity + velocitySpacing * i;
            particles.add(new Particle(x, y, angle, velocity));
        }
    }

    public void addWall(double startX, double startY, double endX, double endY) {
        walls.add(new Wall(startX, startY, endX, endY));
    }

    public List<Particle> getParticles() {
        return particles;
    }

    public int getParticleCount() {
        return particles.size();
    }

    public List<Wall> getWalls() {
        return walls;
    }
    
    public List<Sprite> getSprites() {
        return sprites;
    }

    public int getWallCount() {
        return walls.size();
    }

    public void clearParticles() {
        particles.clear();
    }

    public void clearWalls() {
        walls.clear();
        addWall(0, 0, 1280, 0);
        addWall(0, 720, 1280, 720);
        addWall(0, 0, 0, 720);
        addWall(1280, 0, 1280, 720);
    }
}
