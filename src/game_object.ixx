export module game_object;
import render_object;
import <SFML/Graphics.hpp>;
import <string>;
import <filesystem>; 

export class GameObject : public RenderObject {
public:
    GameObject(float x, float y);
    GameObject(float x, float y, const sf::Color color, int width = 100, int height = 100);
    GameObject(float x, float y, const std::string& spritePath);
    void render(sf::RenderWindow& window) override;
    void scale(float factorX, float factorY);
    void loadTexture(const std::string& spritePath);
    float positionX, positionY;
protected:
    sf::Texture texture;
};

GameObject::GameObject(float x, float y)
    : positionX(x), positionY(y) {
}

GameObject::GameObject(float x, float y, const std::string& spritePath)
    : positionX(x), positionY(y) {
    loadTexture(spritePath);
    sprite.setPosition(positionX, positionY);
}

GameObject::GameObject(float x, float y, const sf::Color color, int width, int height)
    : positionX(x), positionY(y) {

    texture.create(width, height);
    sf::Image image;
    image.create(width, height, color);

    texture.update(image);
    sprite.setTexture(texture);

    sprite.setPosition(positionX, positionY);
}

void GameObject::render(sf::RenderWindow& window) {
    sprite.setPosition(positionX, positionY);
    this->draw(window);
}

void GameObject::scale(float factorX, float factorY) {
    sprite.setScale(factorX, factorY);
}

void GameObject::loadTexture(const std::string& spritePath) {
    if (!std::filesystem::exists(spritePath)) {
        throw std::runtime_error("Nie znaleziono sprite'a: " + spritePath);
    }
    if (!texture.loadFromFile(spritePath)) {
        throw std::runtime_error("Nie udalo sie zaladowac sprite'a: " + spritePath);
    }
    sprite.setTexture(texture);
}
