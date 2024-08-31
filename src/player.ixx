export module player;
import game_object;


export enum class Direction { LEFT, RIGHT };

export class Player : public GameObject {
    std::string spriteLeft;
    std::string spriteRight;

public:
    Player(float x, float y, const std::string& spritePath1, const std::string& spritePath2);
    void move(Direction direction);

    int playerPosition = 0;
};

Player::Player(float x, float y, const std::string& spritePath1, const std::string& spritePath2)
    : GameObject(x, y, spritePath1) {
    spriteLeft = spritePath1;
    spriteRight = spritePath2;
}

void Player::move(Direction direction) {
    if (direction == Direction::LEFT) {
        positionX = 180; 
        loadTexture(spriteLeft);
        playerPosition = 0;
    } else if (direction == Direction::RIGHT) {
        positionX = 484;
        loadTexture(spriteRight);
        playerPosition = 1;
    }
}