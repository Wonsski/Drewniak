export module gameover;

import <SFML/Graphics.hpp>;
import textbox;
import <filesystem>;
import <fstream>;

namespace fs = std::filesystem;


export class GameOver {
public:
    GameOver(sf::RenderWindow& window, const std::string& playerName, int playerScore);
    ~GameOver();

    void render();
    void handleEvents();

private:
    sf::RenderWindow& window;
    sf::Clock blinkTimer;
    bool visible;

    TextBox* gameOverTextBox;
    TextBox* playerNameTextBox;
    TextBox* playerScoreTextBox;
    TextBox* highestScoreTextBox;
    TextBox* restartTextBox;

    void saveScoreToFile(const std::string& playerName, int score);
    std::pair<std::string, int> getHighestScoreFromFile();
};

GameOver::GameOver(sf::RenderWindow& window, const std::string& playerName, int playerScore)
    : window(window), visible(true) {
    gameOverTextBox = new TextBox(400.f, 150.f, sf::Color::White, 80, "Koniec Gry", true);
    playerNameTextBox = new TextBox(400.f, 250.f, sf::Color::White, 30, "Gracz: " + playerName, true);
    playerScoreTextBox = new TextBox(400.f, 300.f, sf::Color::White, 30, "Wynik: " + std::to_string(playerScore), true);
    highestScoreTextBox = new TextBox(400.f, 350.f, sf::Color::White, 30, "", true);
    restartTextBox = new TextBox(400.f, 450.f, sf::Color::White, 30, "Nacisnij Spacje, aby sprobowac ponownie", true);

    saveScoreToFile(playerName, playerScore);
}

GameOver::~GameOver() {
    delete gameOverTextBox;
    delete playerNameTextBox;
    delete playerScoreTextBox;
    delete highestScoreTextBox;
    delete restartTextBox;
}

void GameOver::render() {
    window.clear(sf::Color(34, 32, 52));

    auto highestScore = getHighestScoreFromFile();
    std::string highestScorer = highestScore.first;
    int highestScoreValue = highestScore.second;

    gameOverTextBox->render(window);
    playerNameTextBox->render(window);
    playerScoreTextBox->render(window);

    if (!highestScorer.empty()) {
        highestScoreTextBox->setText("Najwyzszy wynik: " + highestScorer + " (" + std::to_string(highestScoreValue) + ")");
        highestScoreTextBox->render(window);
    }

    if (visible) {
        restartTextBox->render(window);
    }

    window.display();
}

void GameOver::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    if (blinkTimer.getElapsedTime() >= sf::seconds(0.5f)) {
        visible = !visible;
        blinkTimer.restart();
    }
}

void GameOver::saveScoreToFile(const std::string& playerName, int score) {
    std::ofstream file("scores.txt", std::ios::app);
    if (file.is_open()) {
        file << playerName << ": " << score << "\n";
        file.close();
    }
}


std::pair<std::string, int> GameOver::getHighestScoreFromFile() {
    int highestScore = 0;
    std::string highestScorer;

    std::ifstream file("scores.txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string playerName;
            int score;
            if (iss >> playerName >> score) {
                if (score > highestScore) {
                    highestScore = score;
                    highestScorer = playerName;
                }
            }
        }
        file.close();
    }

    return std::make_pair(highestScorer, highestScore);
}