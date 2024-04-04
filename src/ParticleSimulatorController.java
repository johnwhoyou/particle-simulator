import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;
import com.google.gson.Gson;

public class ParticleSimulatorController implements ActionListener {
    public static final int CANVAS_WIDTH = 1280; // Width of the simulator
    public static final int CANVAS_HEIGHT = 720; // Height of the simulator
    private long lastUpdateTime;
    private final int framesPerSecond = 70;
    private final Semaphore semaphore = new Semaphore(1);
    //private int spriteID = 0;

    private final ParticleSimulatorModel model;
    private final ParticleSimulatorView view;
    
    private final ExecutorService computationExecutor;
    private final ExecutorService renderingExecutor;
    private final ExecutorService sendExecutor;
    private final ExecutorService receiveExecutor;
    private final ExecutorService connectionExecutor;

    private final int SERVER_PORT = 8000;
    private final int SERVER_PORT_2 = 3000;
    private final int CLIENT_PORT = 9000;
    private final int HEARTBEAT_TIMEOUT = 10000; // 5 seconds
    DatagramSocket serverSocket;


    private static List<ClientHandler> clients = new ArrayList<>();
    private static int clientIdCounter = 0;
   
    public ParticleSimulatorController() {
        int numThreadsCompute = 32;
        int numThreadsRender = 2;

        this.model = new ParticleSimulatorModel(numThreadsCompute);
        this.view = new ParticleSimulatorView(numThreadsRender, model.getParticles(), model.getWalls(), model.getSprites());
        this.view.setActionListeners(this);

        lastUpdateTime = System.nanoTime();

        computationExecutor = Executors.newSingleThreadExecutor();
        renderingExecutor = Executors.newSingleThreadExecutor();
        sendExecutor = Executors.newSingleThreadExecutor();
        receiveExecutor = Executors.newSingleThreadExecutor();
        connectionExecutor = Executors.newSingleThreadExecutor();

        startComputation();
        startRendering();
        startAcceptingConnections();
        startReceivingData();
        startSendingData();

        System.out.println("Server is running...");

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

    public void startAcceptingConnections() {

        Runnable acceptingConnTask = () -> {
            try {
                ServerSocket serverSocket = new ServerSocket(SERVER_PORT_2);
                while (true) {
                    Socket clientSocket = serverSocket.accept();
                    System.out.println("New client connected.");
                    //System.out.println(clientSocket.getPort());
                    int newClientId = clientIdCounter++;
                    model.addSprite(newClientId);
                    ClientHandler clientHandler = new ClientHandler(clientSocket, newClientId);

                    clients.add(clientHandler);
                    new Thread(clientHandler).start();
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        };

        connectionExecutor.submit(acceptingConnTask);
    }

    private class ClientHandler implements Runnable {
        private Socket clientSocket;
        private final InetAddress clientAddress;

        private int clientId;

        public ClientHandler(Socket socket, int clientId) {
            this.clientSocket = socket;
            this.clientId = clientId;
            this.clientAddress = socket.getInetAddress();
        }

        public Socket getClientSocket() {
            return clientSocket;
        }

        public InetAddress getClientAddress() {
            return clientAddress;
        }

        public int getClientId() {
            return clientId;
        }

        @Override
        public void run() {
            try {
                PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
                BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

                //out.println(clientId);

                while (true) {
                    out.println("HEARTBEAT");
                    String response = in.readLine();
                    //System.out.println("response received");
                    //System.out.println(response);
                    if (response == null || !response.equals("HEARTBEAT")) {
                        System.out.println("Client disconnected: " + clientSocket.getInetAddress());
                        break;
                    }
                    Thread.sleep(HEARTBEAT_TIMEOUT); // Wait for 1 second before sending the next heartbeat
                }

                clientSocket.close();
                model.removeSprite(this.clientId);
                clients.remove(this); // Remove the client handler from the list when the client disconnects
            } catch (IOException | InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void startReceivingData() {
 
        Runnable receivingTask = () -> {
            try {
                serverSocket = new DatagramSocket(SERVER_PORT);
                byte[] receiveData = new byte[512];
                while (true) {
                    // receive sprite locations
                    // if address is not yet in list, then add it to the clients array
                    // if address has note sent a packet in a while, disconnect it
                    DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
                    serverSocket.receive(receivePacket);
                    System.out.println(receivePacket.getAddress());

                    String receivedMessage = new String(receivePacket.getData(), 0, receivePacket.getLength());
                    InetAddress receiveAddress = receivePacket.getAddress();

                    int i = 0;
                    boolean isClientFound = false;
                    int clientIndex = -1;
                    while (i < clients.size() && !isClientFound) {
                        if (compareInetAddresses(clients.get(i).getClientAddress(), receiveAddress)) {
                            isClientFound = true;
                            clientIndex = i;
                        }
                    }

                    model.moveSpriteWithIndex(clientIndex, receivedMessage);

                    // get client id of the received packet
                    // get direction of the received packet
                    // model.moveSprite(clientId, direction)

                    System.out.println("Received from client: " + receivedMessage);
                    receiveData = new byte[512];
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        };
        receiveExecutor.submit(receivingTask);
    }

    public static boolean compareInetAddresses(InetAddress address1, InetAddress address2) {
        // Get byte arrays representing IP addresses
        byte[] byteAddress1 = address1.getAddress();
        byte[] byteAddress2 = address2.getAddress();

        // Compare byte arrays
        if (byteAddress1.length != byteAddress2.length) {
            return false;
        }

        for (int i = 0; i < byteAddress1.length; i++) {
            if (byteAddress1[i] != byteAddress2[i]) {
                return false;
            }
        }

        return true;
    }

    public void startSendingData() {
        Runnable sendingTask = () -> {
            try {
                DatagramSocket socket = new DatagramSocket();
                double scaledWidth = 1280.0 / 33.0;
                double scaledHeight = 720.0 / 19.0;

                // Create a thread pool to manage threads for each client
                ExecutorService clientExecutor = Executors.newFixedThreadPool(clients.size());

                while (true) {
                    for (int i = 0; i < clients.size(); i++) {
                        final int clientId = i;
                        clientExecutor.submit(() -> {
                            try {
                                Sprite sprite = model.getSprites().get(clientId);
                                List<Particle> filteredParticles = new ArrayList<>();
                                for (Particle particle : model.getParticles()) {
                                    if (particle.getX() >= sprite.getX() - 16 && particle.getX() <= sprite.getX() + 16 && particle.getY() >= sprite.getY() - 9 && particle.getY() <= sprite.getY() + 9) {
                                        double adjustedPosX = 640 + (particle.getX() - sprite.getX()) * scaledWidth;
                                        double adjustedPosY = 360 + (particle.getY() - sprite.getY()) * scaledHeight;
                                        filteredParticles.add(new Particle(adjustedPosX, adjustedPosY));
                                    }
                                }
                                if (!filteredParticles.isEmpty()) {
                                    // serialize filtered particles
                                    Gson gson = new Gson();
                                    String jsonString = gson.toJson(filteredParticles);
                                    byte[] sendData = jsonString.getBytes();

                                    // send filtered particles to clients
                                    DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, clients.get(clientId).getClientAddress(), CLIENT_PORT);
                                    socket.send(sendPacket);
                                    //System.out.println(jsonString);
                                }
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        });
                    }
                }

            } catch (Exception e) {
                e.printStackTrace();
            }
        };

        sendExecutor.submit(sendingTask);
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
                                                                       //Temporary to test
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

            case "clear all":
                try {
                    semaphore.acquire();
                } catch (InterruptedException ex) {
                    throw new RuntimeException(ex);
                }
                model.clearParticles();
                model.clearWalls();
                view.updateParticleCounter(model.getParticleCount());
                semaphore.release();
                break;

        }
    }

    public static void main (String[] args) {
        ParticleSimulatorController controller = new ParticleSimulatorController();
    }

}
