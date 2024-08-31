export module titlescreen;

import <SFML/Graphics.hpp>;
import textbox;
import <regex>;

export class TitleScreen {
public:
    TitleScreen(sf::RenderWindow& window);
    ~TitleScreen();

    std::string input;

    void render();
    void handleEvents();

private:
    sf::RenderWindow& window;
    sf::Clock blinkTimer;
    bool visible;

    bool isValidChar(char c);

    TextBox* titleTextBox;
    TextBox* nameLabelBox;
    TextBox* inputTextBox;
    TextBox* startTextBox;
};

TitleScreen::TitleScreen(sf::RenderWindow& window)
    : window(window) {
    titleTextBox = new TextBox(400.f, 150.f, sf::Color::White, 80, "Drewniak", true);
    nameLabelBox = new TextBox(400.f, 300.f, sf::Color::White, 30, "Wpisz swoje imie:", true);
    inputTextBox = new TextBox(255, 355.f, sf::Color::Black, 30, "", false);
    startTextBox = new TextBox(400.f, 450.f, sf::Color::White, 30, "Nacisnij Spacje, aby rozpoczac", true);
}

TitleScreen::~TitleScreen() {
    delete titleTextBox;
    delete nameLabelBox;
    delete inputTextBox;
    delete startTextBox;
}

void TitleScreen::render() {
    window.clear(sf::Color(34, 32, 52));

    if (visible) {
        startTextBox->render(window);
    }

    titleTextBox->render(window);
    nameLabelBox->render(window);

    sf::RectangleShape inputBoxShape(sf::Vector2f(300.f, 50.f));
    inputBoxShape.setFillColor(sf::Color::White);
    inputBoxShape.setOutlineColor(sf::Color::Black);
    inputBoxShape.setOutlineThickness(2.f);
    inputBoxShape.setPosition(250.f, 350.f);
    window.draw(inputBoxShape);

    inputTextBox->render(window);

    window.display();
}

void TitleScreen::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128 && event.text.unicode != 8) {
                if (isValidChar(static_cast<char>(event.text.unicode))) {
                    input += static_cast<char>(event.text.unicode);
                    inputTextBox->setText(input);
                }
            }
            else if (event.text.unicode == 8 && !input.empty()) {
                input.pop_back();
                inputTextBox->setText(input);
            }
        }
    }

    if (blinkTimer.getElapsedTime() >= sf::seconds(0.5f)) {
        visible = !visible;
        blinkTimer.restart();
    }
}

bool TitleScreen::isValidChar(char c) {
    std::string charStr(1, c);
    std::regex pattern("[A-Za-z]"); 

    return std::regex_match(charStr, pattern);
}