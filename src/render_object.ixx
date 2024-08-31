export module render_object;

import <SFML/Graphics.hpp>;

export class RenderObject {
public:
    virtual void render(sf::RenderWindow& window) = 0;
    virtual ~RenderObject() = default;
    void draw(sf::RenderWindow& window);
    void changeBrightness(const int offset);
protected:
    sf::Sprite sprite;
};

void RenderObject::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void RenderObject::changeBrightness(const int offset) {
    sf::Color color(offset, offset, offset, 255);
    sprite.setColor(color);
}