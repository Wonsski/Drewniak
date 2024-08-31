export module game;

import <SFML/Graphics.hpp>;
import <vector>;
import render_object;
import player;
import branch;
import titlescreen;
import textbox;
import gameover;
import <thread>;
import <cstdlib>;


export class Game {
public:
    Game();
    void run();
    int score = 0;

private:
    void mainLoop();
    void handleEvents();
    void update(bool &end);
    void render();
    void spawnTree();
    void spawnBranch(int positionY);
    void chopTree();
    void updateTimer();
    void updateScore();
    void gameSetup();
    void startGame();
    void endGame();
    void restartGame();

    sf::RenderWindow window;
    std::vector<std::shared_ptr<RenderObject>> objects;
    std::shared_ptr<Player> player;
    sf::Color backgroundColor = sf::Color(180,240,60);
    std::atomic<bool> isRunning = false;

    int leftTreeSide = 175;
    int rightTreeSide = 475;
    int playerHeight = 180;
    int branchHeight = 75;
    int initalBranches = 4;
    int killBranchHeight = 375;

    int timer = 100;
    int timerSpeed = 1;
    std::shared_ptr<GameObject> timerObject;

    std::shared_ptr<TextBox> scoreboard;

    std::string playerName;


    std::vector<std::shared_ptr<Branch>> branches;

    std::map<std::string, std::string> spritesPaths = {
        {"background", "assets/background.jpeg"},
        {"playerLeft", "assets/graczL.png"},
        {"playerRight", "assets/graczP.png"},
        {"branchLeft", "assets/galazL.png"},
        {"branchRight", "assets/galazP.png"},
        {"tree", "assets/pien.png"},
        {"tombstone", "assets/nagrobek.png"}
    };

};

Game::Game() : window(sf::VideoMode(800, 600), "Drewniak") {
    startGame();
}

void Game::gameSetup() {
    auto background = std::make_shared<GameObject>(0.0, 0.0, spritesPaths["background"]);
    background->scale(0.8f, 0.5f);
    background->changeBrightness(220);

    auto tree = std::make_shared<GameObject>(320.0, -250.0, spritesPaths["tree"]);
    tree->scale(0.8f, 1);

    player = std::make_shared<Player>(playerHeight, 370.f, spritesPaths["playerLeft"], spritesPaths["playerRight"]);

    timerObject = std::make_shared<GameObject>(0, 570, sf::Color::Red, 800, 30);

    scoreboard = std::make_shared<TextBox>(400, 50, sf::Color::White, 80, score);
    scoreboard->center = true;

    objects.push_back(background);
    objects.push_back(player);
    objects.push_back(tree);
    objects.push_back(timerObject);
    objects.push_back(scoreboard);

    spawnTree();
}

void Game::run() {

    gameSetup();
    isRunning = true;

    std::thread timerThread(&Game::updateTimer, this);
    mainLoop();
    timerThread.join();
}

void Game::mainLoop() {
    bool end = false;
    while (window.isOpen() && isRunning) {
        handleEvents();
        update(end);
        render();
    }
    endGame();
}

void Game::handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left)
                    player->move(Direction::LEFT);
                else if (event.key.code == sf::Keyboard::Right)
                    player->move(Direction::RIGHT);
                else if (event.key.code == sf::Keyboard::Space)
                    chopTree();
            }
        }
}

void Game::update(bool& end) {
        //Colision detection
        for (auto it = branches.begin(); it != branches.end(); ) {
            if ((*it)->positionY == killBranchHeight) {
                if ((*it)->type == player->playerPosition) {
                    isRunning = false;
                }
                it = branches.erase(it);
            }
            else {
                ++it;
            }
        }

        //Branch delete
        for (auto it = objects.begin(); it != objects.end(); ) {
            if (auto branch = std::dynamic_pointer_cast<Branch>(*it)) {
                if (branch->positionY == killBranchHeight) {
                    it = objects.erase(it);
                }
                else {
                    ++it;
                }
            }
            else {
                ++it;
            }
        }       

        //End game
        if (isRunning == false) {
            endGame();
        }
}

void Game::render() {
        window.clear(backgroundColor);
        for (auto& obj : objects) {
            obj->render(window);
        }
        window.display();
}

void Game::spawnTree() {
    int height = 0;

    for (int i = 0; i < initalBranches; i++) {
        spawnBranch(height);
        height += branchHeight;
    }
}

void Game::spawnBranch(int positionY = 0) {
    /*
        Initial branch spawn : branchType = 
        0 - left branch
        1 - right branch
        2 - no branch
    */
    int branchType = std::rand() % 3;

    int positionX;
    std::string sprite;

    switch (branchType) {
    case 0:
        positionX = leftTreeSide;
        sprite = spritesPaths["branchLeft"];
        break;
    case 1:
        positionX = rightTreeSide;
        sprite = spritesPaths["branchRight"];
        break;
    case 2:
        return;
        break;
    }
    auto branch = std::make_shared<Branch>(positionX, positionY, sprite, branchType);
    branch->scale(1.5, 1.5);
    branches.push_back(branch);
    objects.push_back(branch);
}

void Game::chopTree() {

    updateScore();

    for (auto& branch : branches) {
        branch->chop(branchHeight);
    }
    spawnBranch();
}

void Game::updateTimer() {
    while (isRunning) {
        int speed = 60/timerSpeed;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        timer -= 1 * timerSpeed;
        float scale = static_cast<float>(timer) / 100.0f;
        timerObject->scale(scale, 1);

        if (timer <= 0) {
            isRunning = false;
        }
    }
}

void Game::updateScore() {
    score++;
    if (score % 10 == 0 && timerSpeed < 10) {
        timerSpeed++;
    }
    timer = 100;
    scoreboard->setText(score);
}

void Game::endGame() {
    player->loadTexture(spritesPaths["tombstone"]);
    render();
    sf::sleep(sf::seconds(2));

    GameOver gameover(window, playerName, score);
    while (window.isOpen()) {
        gameover.handleEvents();
        gameover.render();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            break;
        }
    }
    restartGame();
    sf::sleep(sf::seconds(0.5));
}

void Game::startGame() {
    TitleScreen titleScreen(window);
    while (window.isOpen()) {
        titleScreen.handleEvents();
        titleScreen.render();

        if (!isRunning && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            break;
        }
    }
    playerName = titleScreen.input;
    run();
}

void Game::restartGame() {
    objects.clear();
    branches.clear();

    score = 0;
    timer = 100;
    timerSpeed = 1;

    run();
}