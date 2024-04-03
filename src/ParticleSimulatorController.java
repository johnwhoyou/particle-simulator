import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.concurrent.*;

public class ParticleSimulatorController implements ActionListener {
    public static final int CANVAS_WIDTH = 1280; // Width of the simulator
    public static final int CANVAS_HEIGHT = 720; // Height of the simulator
    private final ParticleSimulatorModel model;
    private final ParticleSimulatorView view;
    private long lastUpdateTime;
    private final ExecutorService computationExecutor;
    private final ExecutorService renderingExecutor;
    private final int framesPerSecond = 70;
    private final Semaphore semaphore = new Semaphore(1);

    public ParticleSimulatorController() {
        int numThreadsCompute = 32;
        int numThreadsRender = 2;

        this.model = new ParticleSimulatorModel(numThreadsCompute);
        this.view = new ParticleSimulatorView(numThreadsRender, model.getParticles(), model.getWalls());
        this.view.setActionListeners(this);

        lastUpdateTime = System.nanoTime();

        computationExecutor = Executors.newSingleThreadExecutor();
        renderingExecutor = Executors.newSingleThreadExecutor();

        startComputation();
        startRendering();

        // Create a ScheduledExecutorService with a single thread for FPS update
        ScheduledExecutorService fpsScheduler = Executors.newSingleThreadScheduledExecutor();

        // Schedule the task to update FPS every 0.5 seconds
        fpsScheduler.scheduleAtFixedRate(() -> {
                long currentTime = System.nanoTime();
                double elapsedTime = (currentTime - lastUpdateTime) / 1e9; // Convert nanoseconds to seconds
                double fps = view.getFrameCount() / elapsedTime;
                view.updateFPS("FPS: " + String.format("%.2f", fps));
                view.resetFrameCount();
                lastUpdateTime = currentTime;
        }, 0, 500_000_000, TimeUnit.NANOSECONDS); // 500 milliseconds in nanoseconds

    }

    public void startComputation() {
        double deltaTime = 1.0 / framesPerSecond;

        // Define the simulation task
        Runnable computationTask = () -> {
            while (true) {
                try {
                    semaphore.acquire();
                    model.moveParticles(deltaTime);
                    semaphore.release();
                    Thread.sleep((long) (deltaTime * 1000)); // Sleep to control the simulation speed
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                }
            }
        };

        // Submit the simulation task to the computationExecutor
        computationExecutor.submit(computationTask);
    }

    public void startRendering() {
        // Define the rendering task
        Runnable renderingTask = () -> {
            while (true) {
                // Render particles here
                view.repaintSimulatorCanvas();

                try {
                    Thread.sleep(1000 / framesPerSecond); // Adjust to match desired frame rate
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                }
            }
        };

        // Submit the rendering task to the renderingExecutor
        renderingExecutor.submit(renderingTask);
    }


    public void actionPerformed(ActionEvent e)
    {
        switch (e.getActionCommand())
        {
            case "add particle" :
                try {
                    double x = Double.parseDouble(view.getXParticleField());
                    double y = Double.parseDouble(view.getYParticleField());
                    double angle = Double.parseDouble(view.getAngleParticleField());
                    double velocity = Double.parseDouble(view.getVelocityParticleField());
                    if (x > 0 && x < 1280) {
                        if (y > 0 && y < 720) {
                            if (angle >= 0 && angle <= 360) {
                                if (velocity >= 0) {
                                    try {
                                        semaphore.acquire();
                                    } catch (InterruptedException ex) {
                                        throw new RuntimeException(ex);
                                    }

                                    model.addParticle(x, y, angle, velocity);
                                    view.updateParticleCounter(model.getParticleCount());
                                    semaphore.release();
                                }
                                else {
                                    JOptionPane.showMessageDialog(view, "Please enter a non-negative velocity");
                                }
                            }
                            else {
                                JOptionPane.showMessageDialog(view, "Please enter an angle between 0 and 360 (inclusive) only");
                            }
                        }
                        else {
                            JOptionPane.showMessageDialog(view, "Please enter a Y value between 1 and 719 (inclusive) only");
                        }
                    } else {
                        JOptionPane.showMessageDialog(view, "Please enter an X value between 1 and 1279 (inclusive) only");
                    }

                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(view, "No fields should be empty and all fields should have numerical input only");
                }

                break;

            case "add batch by points" :
                try {
                    int numParticles = Integer.parseInt(view.getNumBatchPointField());
                    double angle = Double.parseDouble(view.getAngleBatchPointField());
                    double velocity = Double.parseDouble(view.getVelocityBatchPointField());
                    double startX = Double.parseDouble(view.getStartXBatchPointField());
                    double startY = Double.parseDouble(view.getStartYBatchPointField());
                    double endX = Double.parseDouble(view.getEndXBatchPointField());
                    double endY = Double.parseDouble(view.getEndYBatchPointField());

                    if (numParticles > 1) {
                        if (angle >= 0 && angle <= 360) {
                            if (velocity >= 0) {
                                if (startX > 0 && startX < 1280 && endX > 0 && endX < 1280) {
                                    if (startY > 0 && startY < 720 && endY > 0 && endY < 720) {
                                        try {
                                            semaphore.acquire();
                                        } catch (InterruptedException ex) {
                                            throw new RuntimeException(ex);
                                        }
                                        model.addParticleBatchByPoints(numParticles, angle, velocity, startX, startY, endX, endY);
                                        view.updateParticleCounter(model.getParticleCount());
                                        semaphore.release();
                                    }
                                    else {
                                        JOptionPane.showMessageDialog(view, "Please enter Y values between 1 and 719 (inclusive) only");
                                    }
                                }
                                else {
                                    JOptionPane.showMessageDialog(view, "Please enter X values between 1 and 1279 (inclusive) only");
                                }
                            }
                            else {
                                JOptionPane.showMessageDialog(view, "Please enter a non-negative velocity");
                            }
                        }
                        else {
                            JOptionPane.showMessageDialog(view, "Please enter an angle between 0 and 360 (inclusive) only");
                        }
                    }
                    else {
                        JOptionPane.showMessageDialog(view, "# of Particles should be greater than one");
                    }
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(view, "No fields should be empty and all fields should have numerical input only");
                }
                break;

            case "add batch by angles" :
                try {
                    int numParticles = Integer.parseInt(view.getNumBatchAngleField());
                    double startAngle = Double.parseDouble(view.getStartAngleBatchAngleField());
                    double endAngle = Double.parseDouble(view.getEndAngleBatchAngleField());
                    double velocity = Double.parseDouble(view.getVelocityBatchAngleField());
                    double x = Double.parseDouble(view.getXBatchAngleField());
                    double y = Double.parseDouble(view.getYBatchAngleField());

                    if (numParticles > 1) {
                        if (startAngle >= 0 && startAngle <= 360 && endAngle >= 0 && endAngle <= 360) {
                            if (velocity >= 0) {
                                if (x > 0 && x < 1280) {
                                    if (y > 0 && y < 720) {
                                        try {
                                            semaphore.acquire();
                                        } catch (InterruptedException ex) {
                                            throw new RuntimeException(ex);
                                        }
                                        model.addParticleBatchByAngles(numParticles, startAngle, endAngle, velocity, x, y);
                                        view.updateParticleCounter(model.getParticleCount());
                                        semaphore.release();
                                    }
                                    else {
                                        JOptionPane.showMessageDialog(view, "Please enter a Y value between 1 and 719 (inclusive) only");
                                    }
                                }
                                else {
                                    JOptionPane.showMessageDialog(view, "Please enter an X value between 1 and 1279 (inclusive) only");
                                }
                            }
                            else {
                                JOptionPane.showMessageDialog(view, "Please enter a non-negative velocity");
                            }
                        }
                        else {
                            JOptionPane.showMessageDialog(view, "Please enter angle values between 0 and 360 (inclusive) only");
                        }
                    }
                    else {
                        JOptionPane.showMessageDialog(view, "# of Particles should be greater than one");
                    }
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(view, "No fields should be empty and all fields should have numerical input only");
                }
                break;

            case "add batch by velocities" :
                try {
                    int numParticles = Integer.parseInt(view.getNumBatchVelField());
                    double startVelocity = Double.parseDouble(view.getStartVelocityBatchVelField());
                    double endVelocity = Double.parseDouble(view.getEndVelocityBatchVelField());
                    double angle = Double.parseDouble(view.getAngleBatchVelField());
                    double x = Double.parseDouble(view.getXBatchVelField());
                    double y = Double.parseDouble(view.getYBatchVelField());

                    if (numParticles > 1) {
                        if (startVelocity >= 0 && endVelocity >= 0) {
                            if (angle >= 0 && angle <= 360) {
                                if (x > 0 && x < 1280) {
                                    if (y > 0 && y < 720) {
                                        try {
                                            semaphore.acquire();
                                        } catch (InterruptedException ex) {
                                            throw new RuntimeException(ex);
                                        }
                                        model.addParticleBatchByVelocities(numParticles, startVelocity, endVelocity, angle, x, y);
                                        view.updateParticleCounter(model.getParticleCount());
                                        semaphore.release();
                                    }
                                    else {
                                        JOptionPane.showMessageDialog(view, "Please enter a Y value between 1 and 719 (inclusive) only");
                                    }
                                }
                                else {
                                    JOptionPane.showMessageDialog(view, "Please enter an X value between 1 and 1279 (inclusive) only");
                                }
                            }
                            else {
                                JOptionPane.showMessageDialog(view, "Please enter an angle between 0 and 360 (inclusive) only");
                            }
                        }
                        else {
                            JOptionPane.showMessageDialog(view, "Please enter a non-negative velocity values");
                        }
                    }
                    else {
                        JOptionPane.showMessageDialog(view, "# of Particles should be greater than one");
                    }
                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(view, "No fields should be empty and all fields should have numerical input only");
                }
                break;

            case "add wall" :
                try {
                    double startX = Double.parseDouble(view.getStartXWallField());
                    double startY = Double.parseDouble(view.getStartYWallField());
                    double endX = Double.parseDouble(view.getEndXWallField());
                    double endY = Double.parseDouble(view.getEndYWallField());

                    if (startX >= 0 && startX <= 1280 && endX >= 0 && endX <= 1280) {
                        if (startY >= 0 && startY <= 720 && endY >= 0 && endY <= 720) {
                            try {
                                semaphore.acquire();
                            } catch (InterruptedException ex) {
                                throw new RuntimeException(ex);
                            }
                            model.addWall(startX, startY, endX, endY);
                            view.updateWallCounter(model.getWallCount());
                            semaphore.release();
                        }
                        else {
                            JOptionPane.showMessageDialog(view, "Please enter Y values between 0 and 720 (inclusive) only");
                        }
                    }
                    else{
                        JOptionPane.showMessageDialog(view, "Please enter X values between 0 and 1280 (inclusive) only");
                    }

                } catch (Exception exception) {
                    JOptionPane.showMessageDialog(view, "No fields should be empty and all fields should have numerical input only");
                }
                break;

            case "clear particles" :
                try {
                    semaphore.acquire();
                } catch (InterruptedException ex) {
                    throw new RuntimeException(ex);
                }
                model.clearParticles();
                view.updateParticleCounter(model.getParticleCount());
                semaphore.release();
                break;

            case "clear walls" :
                try {
                    semaphore.acquire();
                } catch (InterruptedException ex) {
                    throw new RuntimeException(ex);
                }
                model.clearWalls();
                view.updateWallCounter(model.getWallCount());
                semaphore.release();
                break;

            case "clear all":
                try {
                    semaphore.acquire();
                } catch (InterruptedException ex) {
                    throw new RuntimeException(ex);
                }
                model.clearParticles();
                model.clearWalls();
                view.updateParticleCounter(model.getParticleCount());
                view.updateWallCounter(model.getWallCount());
                semaphore.release();
                break;

        }
    }

    public static void main (String[] args) {
        ParticleSimulatorController controller = new ParticleSimulatorController();
    }

}
