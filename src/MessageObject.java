import java.util.ArrayList;
import java.util.List;

public class MessageObject {
    List<Particle> particles = new ArrayList<>();
    List<Sprite> sprites = new ArrayList<>();


    public MessageObject(List<Particle> filteredParticles, List<Sprite> otherSprites) {
        this.sprites = new ArrayList<>(otherSprites);
        this.particles = new ArrayList<>(filteredParticles);
    }
}
