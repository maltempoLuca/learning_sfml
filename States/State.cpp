#include "State.h"

State::State(State::StateData &stateData) : stateData(stateData) {
    this->quit = false;
}

State::~State() = default;

void State::updateMousePositions() {
    this->mousePosScreen = sf::Mouse::getPosition();
    this->mousePosWindow = sf::Mouse::getPosition(*this->stateData.window);
    this->mousePosView = this->stateData.window->mapPixelToCoords(sf::Mouse::getPosition(*this->stateData.window));
}

__attribute__((unused)) void State::debugMousePosition() const {
    system("cls");
    std::cout << "posScreen x" << this->mousePosScreen.x << "\n";
    std::cout << "posWindow x" << this->mousePosWindow.x << "\n";
    std::cout << "posView x" << this->mousePosView.x << "\n";
}

void State::endState() {
    this->quit = true;
}

void State::pollEvents(const float &dt) {
    sf::Event event{};

    while (this->stateData.window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            this->stateData.window->close();

        this->handleEvent(event, dt);
    }
}

void State::update(const float &dt) {
    this->pollEvents(dt);
}

void State::initKeybinds(std::string keybindsFilePath) {
    std::ifstream ifs(keybindsFilePath);

    if (ifs.is_open()) {
        std::string key;
        std::string key_value;
        while (ifs >> key >> key_value) {
            this->keybinds[key] = this->stateData.supportedKeys->at(key_value);
        }
    }

    ifs.close();
}

bool State::isQuit() const {
    return quit;
}



