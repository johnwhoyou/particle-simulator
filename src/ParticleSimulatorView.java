import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ActionListener;
import java.util.List;
import java.util.concurrent.*;

public class ParticleSimulatorView extends JFrame {
    private SimulatorCanvasPanel simulatorCanvas;
    private final int numThreads;

    private JButton clearParticlesBtn;

    private JTextField xParticleField;
    private JTextField yParticleField;
    private JTextField angleParticleField;
    private JTextField velocityParticleField;
    private JButton addParticleBtn;

    private JTextField numBatchPointField;
    private JTextField startXBatchPointField;
    private JTextField startYBatchPointField;
    private JTextField endXBatchPointField;
    private JTextField endYBatchPointField;
    private JTextField angleBatchPointField;
    private JTextField velocityBatchPointField;
    private JButton addBatchPointBtn;

    private JTextField numBatchAngleField;
    private JTextField startAngleBatchAngleField;
    private JTextField endAngleBatchAngleField;
    private JTextField velocityBatchAngleField;
    private JTextField xBatchAngleField;
    private JTextField yBatchAngleField;
    private JButton addBatchAngleBtn;

    private JTextField numBatchVelField;
    private JTextField startVelocityBatchVelField;
    private JTextField endVelocityBatchVelField;
    private JTextField angleBatchVelField;
    private JTextField xBatchVelField;
    private JTextField yBatchVelField;
    private JButton addBatchVelBtn;

    private JLabel particleCounterLabel;
    private JLabel fpsLabel;

    public ParticleSimulatorView(int numThreads, List<Particle> particles, List<Wall> walls) {
        super("Particle Simulator");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        setPreferredSize(screenSize);
        setResizable(false);
        setVisible(true);

        simulatorCanvas = new SimulatorCanvasPanel(particles, walls/*, sprites */);
        JPanel eastPanel = createEastPanel();
        JPanel westPanel = createWestPanel(simulatorCanvas);

        add(westPanel, BorderLayout.CENTER);
        add(eastPanel, BorderLayout.EAST);

        this.numThreads = numThreads;

        pack();
    }

    public void updateParticleCounter(int count) {
        particleCounterLabel.setText("Particle Counter: " + count);
    }

    public String getXParticleField() {
        return xParticleField.getText();
    }

    public String getYParticleField() {
        return yParticleField.getText();
    }

    public String getAngleParticleField() {
        return angleParticleField.getText();
    }

    public String getVelocityParticleField() {
        return velocityParticleField.getText();
    }

    public String getNumBatchPointField() {
        return numBatchPointField.getText();
    }

    public String getStartXBatchPointField() {
        return startXBatchPointField.getText();
    }

    public String getStartYBatchPointField() {
        return startYBatchPointField.getText();
    }

    public String getEndXBatchPointField() {
        return endXBatchPointField.getText();
    }

    public String getEndYBatchPointField() {
        return endYBatchPointField.getText();
    }

    public String getAngleBatchPointField() {
        return angleBatchPointField.getText();
    }

    public String getVelocityBatchPointField() {
        return velocityBatchPointField.getText();
    }

    public String getNumBatchAngleField() {
        return numBatchAngleField.getText();
    }

    public String getStartAngleBatchAngleField() {
        return startAngleBatchAngleField.getText();
    }

    public String getEndAngleBatchAngleField() {
        return endAngleBatchAngleField.getText();
    }

    public String getVelocityBatchAngleField() {
        return velocityBatchAngleField.getText();
    }

    public String getXBatchAngleField() {
        return xBatchAngleField.getText();
    }

    public String getYBatchAngleField() {
        return yBatchAngleField.getText();
    }

    public String getNumBatchVelField() {
        return numBatchVelField.getText();
    }

    public String getStartVelocityBatchVelField() {
        return startVelocityBatchVelField.getText();
    }

    public String getEndVelocityBatchVelField() {
        return endVelocityBatchVelField.getText();
    }

    public String getAngleBatchVelField() {
        return angleBatchVelField.getText();
    }

    public String getXBatchVelField() {
        return xBatchVelField.getText();
    }

    public String getYBatchVelField() {
        return yBatchVelField.getText();
    }

    public void updateFPS (String fps) {
        fpsLabel.setText(fps);
    }
    public void setActionListeners(ActionListener listener)
    {
        clearParticlesBtn.addActionListener(listener);
        addParticleBtn.addActionListener(listener);
        addBatchPointBtn.addActionListener(listener);
        addBatchAngleBtn.addActionListener(listener);
        addBatchVelBtn.addActionListener(listener);
    }

    public void resetFrameCount() {
        simulatorCanvas.resetFrameCount();
    }

    public int getFrameCount() {
       return simulatorCanvas.getFrameCount();
    }

    public void repaintSimulatorCanvas() {
        simulatorCanvas.repaint();
    }


    class SimulatorCanvasPanel extends JPanel {
        private List<Particle> particles;
        private List<Wall> walls;
        private int frameCount = 0;
        //private List<Sprite> sprites;

        public SimulatorCanvasPanel(List<Particle> particles, List<Wall> walls) {
            setPreferredSize(new Dimension(ParticleSimulatorController.CANVAS_WIDTH, ParticleSimulatorController.CANVAS_HEIGHT)); // Set the size of the canvas\
            setMaximumSize(new Dimension(ParticleSimulatorController.CANVAS_WIDTH, ParticleSimulatorController.CANVAS_HEIGHT));
            setBackground(Color.WHITE);
            setDoubleBuffered(true);

            this.particles = particles;
            this.walls = walls;
        }

        public void resetFrameCount() {
            this.frameCount = 0;
        }

        public int getFrameCount () {
            return frameCount;
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            // Draw a border for the simulator
            g.drawRect(0, 0, ParticleSimulatorController.CANVAS_WIDTH, ParticleSimulatorController.CANVAS_HEIGHT);
            g.setColor(Color.BLUE);

            ExecutorService executor = Executors.newFixedThreadPool(numThreads);

            // Submit rendering tasks to the executor service
            int numWalls = walls.size();
            int wallsPerThread = numWalls / numThreads;
            for (int i = 0; i < numThreads; i++) {
                final int start = i * wallsPerThread;
                final int end = (i == numThreads - 1) ? numWalls : (i + 1) * wallsPerThread;
                executor.submit(() -> drawWalls(g, start, end));
            }

            int numParticles = particles.size();
            int particlesPerThread = numParticles / numThreads;
            for (int i = 0; i < numThreads; i++) {
                final int start = i * particlesPerThread;
                final int end = (i == numThreads - 1) ? numParticles : (i + 1) * particlesPerThread;
                executor.submit(() -> drawParticles(g, start, end));
            }

           // Shutdown the executor service to prevent new tasks from being submitted
            executor.shutdown();

            // Wait for all tasks to complete
            try {
                executor.awaitTermination(Long.MAX_VALUE, TimeUnit.NANOSECONDS);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            frameCount++;
        }

        private void drawWalls(Graphics g, int start, int end) {
            for (int i = start; i < end; i++) {
                walls.get(i).draw(g);
            }
        }

        private void drawParticles(Graphics g, int start, int end) {
            for (int i = start; i < end; i++) {
                particles.get(i).draw(g);
            }
        }

        private void drawSprites(Graphics g, int start, int end){
            for (int i = start; i < end; i++){
                sprites.get(i).draw(g);
            }
        }
    }

    private JPanel createWestPanel(SimulatorCanvasPanel simulatorCanvas) {
        Font titleFont = new Font("Helvetica Neue", Font.BOLD, 12);
        Font counterFont = new Font("Helvetica Neue", Font.BOLD, 16);

        JPanel westPanel = new JPanel();
        westPanel.setLayout(new BoxLayout(westPanel, BoxLayout.Y_AXIS));
        westPanel.setBackground(Color.BLACK);

        JPanel innerPanel = new JPanel(new BorderLayout());
        innerPanel.setBorder(new EmptyBorder(20, 0, 0, 0));
        innerPanel.setBackground(Color.BLACK);
        innerPanel.setMaximumSize(new Dimension(1280, 760));
        innerPanel.add(simulatorCanvas, BorderLayout.CENTER);

        westPanel.add(innerPanel);

        JPanel basicInputPanel = new JPanel();
        basicInputPanel.setLayout(new FlowLayout());
        basicInputPanel.setBackground(Color.BLACK);

        JPanel infoPanel = new JPanel(new GridLayout(4, 1, 0, 10));
        infoPanel.setBackground(Color.BLACK);

        JLabel titleLabel = new JLabel("STDISCM Problem Set 1");
        titleLabel.setForeground(Color.WHITE);

        JLabel sectionLabel = new JLabel("Section: S11");
        sectionLabel.setForeground(Color.WHITE);

        JLabel p1Label = new JLabel("P1: Ibrahim Kahil");
        p1Label.setForeground(Color.WHITE);

        JLabel p2Label = new JLabel("P2: Shaun Vincent Ong");
        p2Label.setForeground(Color.WHITE);

        infoPanel.add(titleLabel);
        infoPanel.add(sectionLabel);
        infoPanel.add(p1Label);
        infoPanel.add(p2Label);

        basicInputPanel.add(infoPanel);

        JPanel emptyPanel0 = new JPanel();
        emptyPanel0.setPreferredSize(new Dimension(60, 160));
        emptyPanel0.setBackground(Color.BLACK);
        basicInputPanel.add(emptyPanel0);

        JPanel counterPanel = new JPanel(new GridLayout(3, 1, 0, 10));
        counterPanel.setBackground(Color.BLACK);

        particleCounterLabel = new JLabel("Particle Counter: 0");
        particleCounterLabel.setForeground(Color.WHITE);
        particleCounterLabel.setFont(counterFont);

        // Initialize FPS label
        fpsLabel = new JLabel("FPS: 0");
        fpsLabel.setFont(new Font("Helvetica Neue", Font.BOLD, 22));
        fpsLabel.setForeground(Color.WHITE);

        counterPanel.add(particleCounterLabel);
        counterPanel.add(fpsLabel);

        basicInputPanel.add(counterPanel);

        JPanel emptyPanel1 = new JPanel();
        emptyPanel1.setPreferredSize(new Dimension(60, 160));
        emptyPanel1.setBackground(Color.BLACK);
        basicInputPanel.add(emptyPanel1);

        JPanel clearPanel = new JPanel();
        clearPanel.setLayout(new GridLayout(3, 1, 0, 15));
        clearPanel.setBackground(Color.BLACK);
        clearPanel.setBorder(new EmptyBorder(10, 0, 0, 0));

        clearParticlesBtn = new JButton("Clear Particles");
        clearParticlesBtn.setBorder(new EmptyBorder(10, 15, 10, 15));
        clearParticlesBtn.setActionCommand("clear particles");


        clearPanel.add(clearParticlesBtn);
        basicInputPanel.add(clearPanel);

        JPanel emptyPanel2 = new JPanel();
        emptyPanel2.setPreferredSize(new Dimension(40, 160));
        emptyPanel2.setBackground(Color.BLACK);
        basicInputPanel.add(emptyPanel2);

   
        JPanel middlePanel = new JPanel();
        middlePanel.setPreferredSize(new Dimension(20, 160));
        middlePanel.setBackground(Color.BLACK);
        basicInputPanel.add(middlePanel);


        //--------------------------------------------------------------------------------------------------------------
        // ADD PARTICLE

        JPanel addParticlePanel = new JPanel();
        addParticlePanel.setLayout(new BorderLayout());
        addParticlePanel.setBackground(Color.BLACK);
        addParticlePanel.setPreferredSize(new Dimension(280, 160));
        addParticlePanel.setMaximumSize(new Dimension(280, 280));

        JLabel addParticleTitle = new JLabel("Add Particle");
        addParticleTitle.setForeground(Color.WHITE);
        addParticleTitle.setBorder(new EmptyBorder(5, 5, 5, 5));
        addParticleTitle.setHorizontalAlignment(SwingConstants.CENTER);
        addParticleTitle.setFont(titleFont);
        addParticlePanel.add(addParticleTitle, BorderLayout.NORTH);

        JPanel addParticleFieldsPanel = new JPanel();
        addParticleFieldsPanel.setLayout(new GridLayout(4, 2, 0, 15));
        addParticleFieldsPanel.setBackground(Color.BLACK);
        addParticleFieldsPanel.setBorder(new EmptyBorder(10, 20, 0, 20));

        JLabel xParticleLabel = new JLabel("X:");
        xParticleLabel.setForeground(Color.WHITE);
        xParticleField = new JTextField(5);

        JLabel yParticleLabel = new JLabel("Y:");
        yParticleLabel.setForeground(Color.WHITE);
        yParticleField = new JTextField(5);

        JLabel angleParticleLabel = new JLabel("Angle:");
        angleParticleLabel.setForeground(Color.WHITE);
        angleParticleField = new JTextField(5);

        JLabel velocityParticleLabel = new JLabel("Velocity:");
        velocityParticleLabel.setForeground(Color.WHITE);
        velocityParticleField = new JTextField(5);

        addParticleFieldsPanel.add(xParticleLabel);
        addParticleFieldsPanel.add(xParticleField);
        addParticleFieldsPanel.add(yParticleLabel);
        addParticleFieldsPanel.add(yParticleField);
        addParticleFieldsPanel.add(angleParticleLabel);
        addParticleFieldsPanel.add(angleParticleField);
        addParticleFieldsPanel.add(velocityParticleLabel);
        addParticleFieldsPanel.add(velocityParticleField);

        addParticlePanel.add(addParticleFieldsPanel, BorderLayout.CENTER);

        JPanel addParticleBtnPanel = new JPanel(new BorderLayout());
        addParticleBtnPanel.setBorder(new EmptyBorder(10, 10, 10, 10));
        addParticleBtnPanel.setBackground(Color.BLACK);

        addParticleBtn = new JButton("Add");
        addParticleBtn.setActionCommand("add particle");

        addParticleBtnPanel.add(addParticleBtn);
        addParticlePanel.add(addParticleBtnPanel, BorderLayout.EAST);

        basicInputPanel.add(addParticlePanel);

        westPanel.add(basicInputPanel);

        return westPanel;

    }

    private JPanel createEastPanel() {
        Font titleFont = new Font("Helvetica Neue", Font.BOLD, 16);

        JPanel eastPanel = new JPanel();
        eastPanel.setLayout(new BoxLayout(eastPanel, BoxLayout.Y_AXIS));
        eastPanel.setBackground(Color.BLACK);
        eastPanel.setBorder(new EmptyBorder(40, 20, 40, 20));

        //--------------------------------------------------------------------------------------------------------------
        // BATCH POINT

        JPanel addBatchPointPanel = new JPanel();
        addBatchPointPanel.setLayout(new BorderLayout());
        addBatchPointPanel.setBackground(Color.BLACK);
        addBatchPointPanel.setPreferredSize(new Dimension(335, 100));
        addBatchPointPanel.setMaximumSize(new Dimension(335, 280));
        //addBatchPointPanel.setBorder(new EmptyBorder(0, 0, 10, 0));

        JLabel addBatchPointTitle = new JLabel("Add Particle Batch (Points)");
        addBatchPointTitle.setForeground(Color.WHITE);
        addBatchPointTitle.setBorder(new EmptyBorder(5, 5, 5, 5));
        addBatchPointTitle.setHorizontalAlignment(SwingConstants.CENTER);
        addBatchPointTitle.setFont(titleFont);
        addBatchPointPanel.add(addBatchPointTitle, BorderLayout.NORTH);

        JPanel addBatchPointFieldsPanel = new JPanel();
        addBatchPointFieldsPanel.setLayout(new BoxLayout(addBatchPointFieldsPanel, BoxLayout.X_AXIS));
        addBatchPointFieldsPanel.setBackground(Color.BLACK);
        addBatchPointFieldsPanel.setBorder(new EmptyBorder(0, 20, 0, 20));

        JPanel addBatchPointFieldsLeftPanel = new JPanel();
        addBatchPointFieldsLeftPanel.setLayout(new BoxLayout(addBatchPointFieldsLeftPanel, BoxLayout.Y_AXIS));
        addBatchPointFieldsLeftPanel.setBackground(Color.BLACK);
        addBatchPointFieldsLeftPanel.setPreferredSize(new Dimension(100, 100));
        addBatchPointFieldsLeftPanel.setMaximumSize(new Dimension(100, 150));

        JPanel addBatchPointFieldsRightPanel = new JPanel();
        addBatchPointFieldsRightPanel.setBackground(Color.BLACK);
        addBatchPointFieldsRightPanel.setLayout(new BoxLayout(addBatchPointFieldsRightPanel, BoxLayout.Y_AXIS));
        addBatchPointFieldsRightPanel.setPreferredSize(new Dimension(100, 160));
        addBatchPointFieldsRightPanel.setMaximumSize(new Dimension(100, 200));

        JPanel addBatchPointMiddlePanel = new JPanel();
        addBatchPointMiddlePanel.setBackground(Color.BLACK);
        addBatchPointMiddlePanel.setPreferredSize(new Dimension(50, 160));

        JLabel numBatchPointLabel = new JLabel("# of Particles:");
        numBatchPointLabel.setForeground(Color.WHITE);
        numBatchPointLabel.setBorder(new EmptyBorder(0, 0, 5, 0));
        numBatchPointField = new JTextField(4);
        numBatchPointField.setMaximumSize(new Dimension(180, 18));

        JLabel startXBatchPointLabel = new JLabel("Start X:");
        startXBatchPointLabel.setForeground(Color.WHITE);
        startXBatchPointLabel.setBorder(new EmptyBorder(0, 0, 5, 0));
        startXBatchPointField = new JTextField(4);
        startXBatchPointField.setMaximumSize(new Dimension(180, 18));

        JLabel startYBatchPointLabel = new JLabel("Start Y: ");
        startYBatchPointLabel.setForeground(Color.WHITE);
        startYBatchPointLabel.setBorder(new EmptyBorder(10, 0, 5, 0));
        startYBatchPointField = new JTextField(4);
        startYBatchPointField.setMaximumSize(new Dimension(180, 18));

        JLabel endXBatchPointLabel = new JLabel("End X:");
        endXBatchPointLabel.setForeground(Color.WHITE);
        endXBatchPointLabel.setBorder(new EmptyBorder(10, 0, 5, 0));
        endXBatchPointField = new JTextField(4);
        endXBatchPointField.setMaximumSize(new Dimension(180, 18));

        JLabel endYBatchPointLabel = new JLabel("End Y:");
        endYBatchPointLabel.setForeground(Color.WHITE);
        endYBatchPointLabel.setBorder(new EmptyBorder(10, 0, 5, 0));
        endYBatchPointField = new JTextField(4);
        endYBatchPointField.setMaximumSize(new Dimension(180, 18));

        JLabel angleBatchPointLabel = new JLabel("Angle:");
        angleBatchPointLabel.setForeground(Color.WHITE);
        angleBatchPointLabel.setBorder(new EmptyBorder(10, 0, 5, 0));
        angleBatchPointField = new JTextField(4);
        angleBatchPointField.setMaximumSize(new Dimension(180, 18));

        JLabel velocityBatchPointLabel = new JLabel("Velocity:");
        velocityBatchPointLabel.setForeground(Color.WHITE);
        velocityBatchPointLabel.setBorder(new EmptyBorder(10, 0, 5, 0));
        velocityBatchPointField = new JTextField(4);
        velocityBatchPointField.setMaximumSize(new Dimension(180, 18));

        addBatchPointFieldsLeftPanel.add(numBatchPointLabel);
        addBatchPointFieldsLeftPanel.add(numBatchPointField);
        addBatchPointFieldsLeftPanel.add(angleBatchPointLabel);
        addBatchPointFieldsLeftPanel.add(angleBatchPointField);
        addBatchPointFieldsLeftPanel.add(velocityBatchPointLabel);
        addBatchPointFieldsLeftPanel.add(velocityBatchPointField);

        addBatchPointFieldsRightPanel.add(startXBatchPointLabel);
        addBatchPointFieldsRightPanel.add(startXBatchPointField);
        addBatchPointFieldsRightPanel.add(startYBatchPointLabel);
        addBatchPointFieldsRightPanel.add(startYBatchPointField);
        addBatchPointFieldsRightPanel.add(endXBatchPointLabel);
        addBatchPointFieldsRightPanel.add(endXBatchPointField);
        addBatchPointFieldsRightPanel.add(endYBatchPointLabel);
        addBatchPointFieldsRightPanel.add(endYBatchPointField);

        addBatchPointFieldsPanel.add(Box.createHorizontalGlue());
        addBatchPointFieldsPanel.add(addBatchPointFieldsLeftPanel);
        addBatchPointFieldsPanel.add(addBatchPointMiddlePanel);
        addBatchPointFieldsPanel.add(addBatchPointFieldsRightPanel);
        addBatchPointFieldsPanel.add(Box.createHorizontalGlue());

        addBatchPointPanel.add(addBatchPointFieldsPanel, BorderLayout.CENTER);

        JPanel addBatchPointBtnPanel = new JPanel(new BorderLayout());
        addBatchPointBtnPanel.setBorder(new EmptyBorder(0, 80, 5, 80));
        addBatchPointBtnPanel.setBackground(Color.BLACK);

        addBatchPointBtn = new JButton("Add");
        addBatchPointBtn.setActionCommand("add batch by points");

        addBatchPointBtnPanel.add(addBatchPointBtn);
        addBatchPointPanel.add(addBatchPointBtnPanel, BorderLayout.SOUTH);

        eastPanel.add(addBatchPointPanel);

        JPanel emptyPanel1 = new JPanel();
        emptyPanel1.setMaximumSize(new Dimension(100, 25));
        emptyPanel1.setBackground(Color.BLACK);

        eastPanel.add(emptyPanel1);

        //------------------------------------------------------------------------------------------------------------
        // BATCH ANGLE

        JPanel addBatchAnglePanel = new JPanel();
        addBatchAnglePanel.setLayout(new BorderLayout());
        addBatchAnglePanel.setBackground(Color.BLACK);
        addBatchAnglePanel.setPreferredSize(new Dimension(335, 100));
        addBatchAnglePanel.setMaximumSize(new Dimension(335, 240));

        JLabel addBatchAngleTitle = new JLabel("Add Particle Batch (Angles)");
        addBatchAngleTitle.setForeground(Color.WHITE);
        addBatchAngleTitle.setBorder(new EmptyBorder(5, 5, 5, 5));
        addBatchAngleTitle.setHorizontalAlignment(SwingConstants.CENTER);
        addBatchAngleTitle.setFont(titleFont);
        addBatchAnglePanel.add(addBatchAngleTitle, BorderLayout.NORTH);

        JPanel addBatchAngleFieldsPanel = new JPanel();
        addBatchAngleFieldsPanel.setLayout(new BoxLayout(addBatchAngleFieldsPanel, BoxLayout.X_AXIS));
        addBatchAngleFieldsPanel.setBackground(Color.BLACK);
        addBatchAngleFieldsPanel.setBorder(new EmptyBorder(0, 20, 0, 20));

        JPanel addBatchAngleFieldsLeftPanel = new JPanel();
        addBatchAngleFieldsLeftPanel.setLayout(new BoxLayout(addBatchAngleFieldsLeftPanel, BoxLayout.Y_AXIS));
        addBatchAngleFieldsLeftPanel.setBackground(Color.BLACK);
        addBatchAngleFieldsLeftPanel.setPreferredSize(new Dimension(100, 100));
        addBatchAngleFieldsLeftPanel.setMaximumSize(new Dimension(100, 150));

        JPanel addBatchAngleFieldsRightPanel = new JPanel();
        addBatchAngleFieldsRightPanel.setBackground(Color.BLACK);
        addBatchAngleFieldsRightPanel.setLayout(new BoxLayout(addBatchAngleFieldsRightPanel, BoxLayout.Y_AXIS));
        addBatchAngleFieldsRightPanel.setPreferredSize(new Dimension(100, 150));
        addBatchAngleFieldsRightPanel.setMaximumSize(new Dimension(100, 150));

        JPanel addBatchAngleMiddlePanel = new JPanel();
        addBatchAngleMiddlePanel.setBackground(Color.BLACK);
        addBatchAngleMiddlePanel.setPreferredSize(new Dimension(50, 160));

        JLabel numBatchAngleLabel = new JLabel("# of Particles:");
        numBatchAngleLabel.setForeground(Color.WHITE);
        numBatchAngleLabel.setBorder(new EmptyBorder(0, 0, 5, 0));
        numBatchAngleField = new JTextField(4);
        numBatchAngleField.setMaximumSize(new Dimension(180, 18));

        JLabel startAngleBatchAngleLabel = new JLabel("Start Angle:");
        startAngleBatchAngleLabel.setForeground(Color.WHITE);
        startAngleBatchAngleLabel.setBorder(new EmptyBorder(10, 0, 5, 0));
        startAngleBatchAngleField = new JTextField(4);
        startAngleBatchAngleField.setMaximumSize(new Dimension(180, 18));

        JLabel endAngleBatchAngleLabel = new JLabel("End Angle:");
        endAngleBatchAngleLabel.setForeground(Color.WHITE);
        endAngleBatchAngleLabel.setBorder(new EmptyBorder(10, 0, 5, 0));
        endAngleBatchAngleField = new JTextField(4);
        endAngleBatchAngleField.setMaximumSize(new Dimension(180, 18));

        JLabel velocityBatchAngleLabel = new JLabel("Velocity:");
        velocityBatchAngleLabel.setForeground(Color.WHITE);
        velocityBatchAngleLabel.setBorder(new EmptyBorder(0, 0, 5, 0));
        velocityBatchAngleField = new JTextField(4);
        velocityBatchAngleField.setMaximumSize(new Dimension(180, 18));

        JLabel xBatchAngleLabel = new JLabel("X:");
        xBatchAngleLabel.setForeground(Color.WHITE);
        xBatchAngleLabel.setBorder(new EmptyBorder(10, 0, 5, 0));
        xBatchAngleField = new JTextField(4);
        xBatchAngleField.setMaximumSize(new Dimension(180, 18));

        JLabel yBatchAngleLabel = new JLabel("Y: ");
        yBatchAngleLabel.setForeground(Color.WHITE);
        yBatchAngleLabel.setBorder(new EmptyBorder(10, 0, 5, 0));
        yBatchAngleField = new JTextField(4);
        yBatchAngleField.setMaximumSize(new Dimension(180, 18));

        addBatchAngleFieldsLeftPanel.add(numBatchAngleLabel);
        addBatchAngleFieldsLeftPanel.add(numBatchAngleField);
        addBatchAngleFieldsLeftPanel.add(startAngleBatchAngleLabel);
        addBatchAngleFieldsLeftPanel.add(startAngleBatchAngleField);
        addBatchAngleFieldsLeftPanel.add(endAngleBatchAngleLabel);
        addBatchAngleFieldsLeftPanel.add(endAngleBatchAngleField);

        addBatchAngleFieldsRightPanel.add(velocityBatchAngleLabel);
        addBatchAngleFieldsRightPanel.add(velocityBatchAngleField);
        addBatchAngleFieldsRightPanel.add(xBatchAngleLabel);
        addBatchAngleFieldsRightPanel.add(xBatchAngleField);
        addBatchAngleFieldsRightPanel.add(yBatchAngleLabel);
        addBatchAngleFieldsRightPanel.add(yBatchAngleField);

        addBatchAngleFieldsPanel.add(Box.createHorizontalGlue());
        addBatchAngleFieldsPanel.add(addBatchAngleFieldsLeftPanel);
        addBatchAngleFieldsPanel.add(addBatchAngleMiddlePanel);
        addBatchAngleFieldsPanel.add(addBatchAngleFieldsRightPanel);
        addBatchAngleFieldsPanel.add(Box.createHorizontalGlue());

        addBatchAnglePanel.add(addBatchAngleFieldsPanel, BorderLayout.CENTER);

        JPanel addBatchAngleBtnPanel = new JPanel(new BorderLayout());
        addBatchAngleBtnPanel.setBorder(new EmptyBorder(0, 80, 5, 80));
        addBatchAngleBtnPanel.setBackground(Color.BLACK);

        addBatchAngleBtn = new JButton("Add");
        addBatchAngleBtn.setActionCommand("add batch by angles");

        addBatchAngleBtnPanel.add(addBatchAngleBtn);
        addBatchAnglePanel.add(addBatchAngleBtnPanel, BorderLayout.SOUTH);

        eastPanel.add(addBatchAnglePanel);

        JPanel emptyPanel2 = new JPanel();
        emptyPanel2.setMaximumSize(new Dimension(100, 25));
        emptyPanel2.setBackground(Color.BLACK);

        eastPanel.add(emptyPanel2);

        //------------------------------------------------------------------------------------------------------------
        // BATCH VELOCITY

        JPanel addBatchVelPanel = new JPanel();
        addBatchVelPanel.setLayout(new BorderLayout());
        addBatchVelPanel.setBackground(Color.BLACK);
        addBatchVelPanel.setPreferredSize(new Dimension(335, 100));
        addBatchVelPanel.setMaximumSize(new Dimension(335, 240));

        JLabel addBatchVelTitle = new JLabel("Add Particle Batch (Velocities)");
        addBatchVelTitle.setForeground(Color.WHITE);
        addBatchVelTitle.setBorder(new EmptyBorder(5, 5, 5, 5));
        addBatchVelTitle.setHorizontalAlignment(SwingConstants.CENTER);
        addBatchVelTitle.setFont(titleFont);
        addBatchVelPanel.add(addBatchVelTitle, BorderLayout.NORTH);

        JPanel addBatchVelFieldsPanel = new JPanel();
        addBatchVelFieldsPanel.setLayout(new BoxLayout(addBatchVelFieldsPanel, BoxLayout.X_AXIS));
        addBatchVelFieldsPanel.setBackground(Color.BLACK);
        addBatchVelFieldsPanel.setBorder(new EmptyBorder(0, 20, 0, 20));

        JPanel addBatchVelFieldsLeftPanel = new JPanel();
        addBatchVelFieldsLeftPanel.setLayout(new BoxLayout(addBatchVelFieldsLeftPanel, BoxLayout.Y_AXIS));
        addBatchVelFieldsLeftPanel.setBackground(Color.BLACK);
        addBatchVelFieldsLeftPanel.setPreferredSize(new Dimension(100, 100));
        addBatchVelFieldsLeftPanel.setMaximumSize(new Dimension(100, 150));

        JPanel addBatchVelFieldsRightPanel = new JPanel();
        addBatchVelFieldsRightPanel.setBackground(Color.BLACK);
        addBatchVelFieldsRightPanel.setLayout(new BoxLayout(addBatchVelFieldsRightPanel, BoxLayout.Y_AXIS));
        addBatchVelFieldsRightPanel.setPreferredSize(new Dimension(100, 150));
        addBatchVelFieldsRightPanel.setMaximumSize(new Dimension(100, 150));

        JPanel addBatchVelMiddlePanel = new JPanel();
        addBatchVelMiddlePanel.setBackground(Color.BLACK);
        addBatchVelMiddlePanel.setPreferredSize(new Dimension(50, 160));

        JLabel numBatchVelLabel = new JLabel("# of Particles:");
        numBatchVelLabel.setForeground(Color.WHITE);
        numBatchVelLabel.setBorder(new EmptyBorder(0, 0, 5, 0));
        numBatchVelField = new JTextField(4);
        numBatchVelField.setMaximumSize(new Dimension(180, 18));

        JLabel startVelocityBatchVelLabel = new JLabel("Start Velocity:");
        startVelocityBatchVelLabel.setForeground(Color.WHITE);
        startVelocityBatchVelLabel.setBorder(new EmptyBorder(10, 0, 5, 0));
        startVelocityBatchVelField = new JTextField(4);
        startVelocityBatchVelField.setMaximumSize(new Dimension(180, 18));

        JLabel endVelocityBatchVelLabel = new JLabel("End Velocity:");
        endVelocityBatchVelLabel.setForeground(Color.WHITE);
        endVelocityBatchVelLabel.setBorder(new EmptyBorder(10, 0, 5, 0));
        endVelocityBatchVelField = new JTextField(4);
        endVelocityBatchVelField.setMaximumSize(new Dimension(180, 18));

        JLabel angleBatchVelLabel = new JLabel("Angle:");
        angleBatchVelLabel.setForeground(Color.WHITE);
        angleBatchVelLabel.setBorder(new EmptyBorder(0, 0, 5, 0));
        angleBatchVelField = new JTextField(4);
        angleBatchVelField.setMaximumSize(new Dimension(180, 18));

        JLabel xBatchVelLabel = new JLabel("X:");
        xBatchVelLabel.setForeground(Color.WHITE);
        xBatchVelLabel.setBorder(new EmptyBorder(10, 0, 5, 0));
        xBatchVelField = new JTextField(4);
        xBatchVelField.setMaximumSize(new Dimension(180, 18));

        JLabel yBatchVelLabel = new JLabel("Y: ");
        yBatchVelLabel.setForeground(Color.WHITE);
        yBatchVelLabel.setBorder(new EmptyBorder(10, 0, 5, 0));
        yBatchVelField = new JTextField(4);
        yBatchVelField.setMaximumSize(new Dimension(180, 18));

        addBatchVelFieldsLeftPanel.add(numBatchVelLabel);
        addBatchVelFieldsLeftPanel.add(numBatchVelField);
        addBatchVelFieldsLeftPanel.add(startVelocityBatchVelLabel);
        addBatchVelFieldsLeftPanel.add(startVelocityBatchVelField);
        addBatchVelFieldsLeftPanel.add(endVelocityBatchVelLabel);
        addBatchVelFieldsLeftPanel.add(endVelocityBatchVelField);

        addBatchVelFieldsRightPanel.add(angleBatchVelLabel);
        addBatchVelFieldsRightPanel.add(angleBatchVelField);
        addBatchVelFieldsRightPanel.add(xBatchVelLabel);
        addBatchVelFieldsRightPanel.add(xBatchVelField);
        addBatchVelFieldsRightPanel.add(yBatchVelLabel);
        addBatchVelFieldsRightPanel.add(yBatchVelField);

        addBatchVelFieldsPanel.add(Box.createHorizontalGlue());
        addBatchVelFieldsPanel.add(addBatchVelFieldsLeftPanel);
        addBatchVelFieldsPanel.add(addBatchVelMiddlePanel);
        addBatchVelFieldsPanel.add(addBatchVelFieldsRightPanel);
        addBatchVelFieldsPanel.add(Box.createHorizontalGlue());

        addBatchVelPanel.add(addBatchVelFieldsPanel, BorderLayout.CENTER);

        JPanel addBatchVelBtnPanel = new JPanel(new BorderLayout());
        addBatchVelBtnPanel.setBorder(new EmptyBorder(0, 80, 5, 80));
        addBatchVelBtnPanel.setBackground(Color.BLACK);

        addBatchVelBtn = new JButton("Add");
        addBatchVelBtn.setActionCommand("add batch by velocities");

        addBatchVelBtnPanel.add(addBatchVelBtn);
        addBatchVelPanel.add(addBatchVelBtnPanel, BorderLayout.SOUTH);

        eastPanel.add(addBatchVelPanel);


        return eastPanel;
    }
}
