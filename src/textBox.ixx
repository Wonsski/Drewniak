export module textbox;

import game_object;

export class TextBox : public GameObject {
    
    sf::Font font;
    sf::Color color = sf::Color::White;
    int size = 40;

public:

    std::string text;
    sf::Text textObj;

    bool center = false;

    void render(sf::RenderWindow& window);
    void setColor(sf::Color col);
    void setSize(int x);
    void setText(std::string txt);
    void setText(int val);
    void centerText();

    TextBox(float x, float y);
    TextBox(float x, float y, sf::Color _color, int _size, std::string _text);
    TextBox(float x, float y, sf::Color _color, int _size, std::string _text, bool _center);
    TextBox(float x, float y, sf::Color _color, int _size, int val);
};

TextBox::TextBox(float x, float y) 
    : GameObject(x, y) {
    if (!font.loadFromFile("fonts/PixelifySans-Bold.ttf")) {
        return;
    }
}


TextBox::TextBox(float x, float y, sf::Color _color, int _size, std::string _text)
    : GameObject(x, y) {
    if (!font.loadFromFile("fonts/PixelifySans-Bold.ttf")) {
        return;
    }
    color = _color;
    size = _size;
    text = _text;
    sf::Text obj(text, font, size);
    textObj = obj;
    textObj.setFillColor(color);
}

TextBox::TextBox(float x, float y, sf::Color _color, int _size, std::string _text, bool _center)
    : TextBox(x, y, _color, _size, _text) {
    center = _center;
}

TextBox::TextBox(float x, float y, sf::Color _color, int _size, int val)
    : GameObject(x, y) {
    if (!font.loadFromFile("fonts/PixelifySans-Bold.ttf")) {
        return;
    }
    color = _color;
    size = _size;
    text = std::to_string(val);
    sf::Text obj(text, font, size);
    textObj = obj;
    textObj.setFillColor(color);
}

void TextBox::render(sf::RenderWindow& window) {
    textObj.setPosition(positionX, positionY);
    if (center) {
        centerText();
    }
    window.draw(textObj);
}

void TextBox::setColor(sf::Color col) {
    color = col;
    textObj.setFillColor(color);
}

void TextBox::setSize(int x) {
    size = x;
    sf::Text obj(text, font, size);
    textObj = obj;
}

void TextBox::setText(std::string txt) {
    text = txt;
    sf::Text obj(text, font, size);
    textObj = obj;
    textObj.setFillColor(color);
}

void TextBox::setText(int val) {
    text = std::to_string(val);
    int fontsize = size;
    if (val >= 1000) {
        fontsize -= 10;
    }
    sf::Text obj(text, font, fontsize);
    textObj = obj;
    textObj.setFillColor(color);
}

void TextBox::centerText() {
    sf::FloatRect textRect = textObj.getLocalBounds();
    textObj.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    textObj.setPosition(positionX, positionY);
}