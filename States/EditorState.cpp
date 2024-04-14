#include "EditorState.h"

EditorState::EditorState(StateData &stateData) :
        State(stateData),
        pauseMenuState(PauseMenuState(stateData)) {
    this->initVariables();
    this->initView();
    State::initKeybinds("Config/editorstate_keybinds.ini");
    this->initButtons();
    this->initTexts();
    this->initGui();
}

EditorState::~EditorState() {
    for (auto &button: this->buttons) {
        delete button.second;
    }

    delete this->tileMap;
    delete this->textureSelector;
}

void EditorState::update(const float &dt) {

    if (!pauseMenuState.isPaused()) {
        State::update(dt);
        this->updateGui();
        this->updateButtons();
        if (this->clock.getElapsedTime() > this->textureSelectorTimer) {
            this->showTextureSelector = false;
        }
    } else {
        pauseMenuState.update(dt);
    }
}

void EditorState::render(sf::RenderTarget *target) {
    if (!target) {
        target = this->stateData.window;
    }

    this->renderButtons(target);
    this->renderGui(target);

    if (!pauseMenuState.isPaused() && !this->textureSelector->isActive()) {
        target->draw(this->cursorText);
        target->draw(this->previewTexture);
    }

    if (pauseMenuState.isPaused()) {
        pauseMenuState.render(target);
    }
}


void EditorState::initButtons() {
    this->buttons["TOGGLE_TEXTURE_SELECTOR"] = new GUI::Button(this->stateData.window->getSize().x - 50, 0, 50, 50,
                                                               this->stateData.font, "TS", 30,
                                                               sf::Color(120, 50, 80, 200),
                                                               sf::Color(150, 50, 80, 250),
                                                               sf::Color(90, 40, 60, 50),
                                                               sf::Color(120, 50, 80, 100),
                                                               sf::Color(150, 50, 80, 100),
                                                               sf::Color(90, 40, 60, 100));

    this->buttons["SAVE_TEXTURE_MAP"] = new GUI::Button(this->stateData.window->getSize().x - 50,
                                                        this->stateData.gridSize + 10, 50, 50,
                                                        this->stateData.font, "SV", 30,
                                                        sf::Color(120, 50, 80, 200),
                                                        sf::Color(150, 50, 80, 250),
                                                        sf::Color(90, 40, 60, 50),
                                                        sf::Color(120, 50, 80, 100),
                                                        sf::Color(150, 50, 80, 100),
                                                        sf::Color(90, 40, 60, 100));

    this->buttons["TOGGLE_COLLISIONS"] = new GUI::Button(this->stateData.window->getSize().x - 50,
                                                         (this->stateData.gridSize + 10) * 2, 50, 50,
                                                         this->stateData.font, "COL", 30,
                                                         sf::Color(120, 50, 80, 200),
                                                         sf::Color(150, 50, 80, 250),
                                                         sf::Color(90, 40, 60, 50),
                                                         sf::Color(120, 50, 80, 100),
                                                         sf::Color(150, 50, 80, 100),
                                                         sf::Color(90, 40, 60, 100));

    this->buttons["CLEAR_MAP"] = new GUI::Button(this->stateData.window->getSize().x - 50,
                                                 (this->stateData.gridSize + 10) * 3, 50, 50,
                                                 this->stateData.font, "R", 30,
                                                 sf::Color(120, 50, 80, 200),
                                                 sf::Color(150, 50, 80, 250),
                                                 sf::Color(90, 40, 60, 50),
                                                 sf::Color(120, 50, 80, 100),
                                                 sf::Color(150, 50, 80, 100),
                                                 sf::Color(90, 40, 60, 100));
}

void EditorState::renderButtons(sf::RenderTarget *target) {
    for (auto &button: this->buttons) {
        button.second->render(*target);
    }
}

void EditorState::updateButtons() {
    /*Updates all buttons in state and handles their functionality*/
    for (auto &button: this->buttons) {
        button.second->update(this->mousePosView);
    }

    if (this->buttons["TOGGLE_TEXTURE_SELECTOR"]->isPressed()) {
        this->buttons["TOGGLE_TEXTURE_SELECTOR"]->reset();
        this->showTextureSelector = !this->showTextureSelector;
        this->clock.restart();
    }

    if (this->buttons["SAVE_TEXTURE_MAP"]->isPressed()) {
        this->buttons["SAVE_TEXTURE_MAP"]->reset();
        this->tileMap->saveToFile("Resources/map/map.slmp");
    }

    if (this->buttons["TOGGLE_COLLISIONS"]->isPressed()) {
        this->buttons["TOGGLE_COLLISIONS"]->reset();
        auto i = std::find(this->tileTypes.begin(), this->tileTypes.end(), TILE_TYPES::COLLISION);
        if (i != this->tileTypes.end()) {
            this->tileTypes.erase(i);
        } else {
            this->tileTypes.push_back(TILE_TYPES::COLLISION);
        }
    }

    if (this->buttons["CLEAR_MAP"]->isPressed()) {
        this->buttons["CLEAR_MAP"]->reset();
        this->tileMap->clear();
    }

    this->tileMap->update();
}

//Initializer Functions
void EditorState::initVariables() {
    this->showTextureSelector = false;
    this->tileTexturePath = "Resources/images/tiles/nuovo_tilesheet.png";
    this->tileMap = new Tilemap("Resources/map/map.slmp");
    this->tileTypes.push_back(TILE_TYPES::DEFAULT);
    this->tileTypes.push_back(TILE_TYPES::COLLISION);
}

void EditorState::initView() {
    this->view.setSize(sf::Vector2f(this->stateData.graphicsSettings->resolution.width,
                                    this->stateData.graphicsSettings->resolution.height));
    this->view.setCenter(this->stateData.graphicsSettings->resolution.width / 2.f,
                         this->stateData.graphicsSettings->resolution.height / 2.f);

}

void EditorState::handleEvent(sf::Event &event, const float &dt) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == this->keybinds["CLOSE"]) { this->endState(); }
        if (event.key.code == this->keybinds["PAUSE"]) { this->pauseMenuState.setPause(true); }
    }

    if (!this->pauseMenuState.isPaused()) {
        // se sidebar
        if (this->sideBar.getGlobalBounds().contains(this->mousePosView)) {
            for (auto &button: this->buttons) {
                button.second->handleEvent(event, mousePosView);
            }
        }

        // se non è sidebar
        if (!this->sideBar.getGlobalBounds().contains(this->mousePosView)) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (this->textureSelector->isActive()) {
                    this->textureSelector->setSelectedTile(mousePosWindow);
                } else {
                    TileData tileData;
                    tileData.index_x = this->mousePosGrid.x;
                    tileData.index_y = this->mousePosGrid.y;
                    tileData.index_z = 0;
                    tileData.textureRect = this->textureSelector->getSelectedRelativePosition();
                    tileData.types = this->tileTypes;


                    this->tileMap->addTile(tileData);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                if (this->textureSelector->isActive()) {

                } else {
                    this->tileMap->removeTile(this->mousePosGrid.x, this->mousePosGrid.y, 0);
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
                this->openTextureSelector();
                this->textureSelector->setSelectedTile(1, 0);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
                this->openTextureSelector();
                this->textureSelector->setSelectedTile(-1, 0);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
                this->openTextureSelector();
                this->textureSelector->setSelectedTile(0, -1);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
                this->openTextureSelector();
                this->textureSelector->setSelectedTile(0, 1);
            }
        }
    }
}

void EditorState::openTextureSelector() {
    clock.restart();
    showTextureSelector = true;
}

/**
 * Chiude EditorState se è stata richiesta la chiusura del gioco da menu di pausa o direttamente da gamestate
 * @return
 */
bool EditorState::isQuit() const {
    return this->quit || this->pauseMenuState.isQuit();
}

void EditorState::initGui() {
    auto resolution = this->stateData.graphicsSettings->resolution;
    //TODO:: analogo della dropdown, deve essere un componente
    this->sideBar.setPosition(static_cast<float>(resolution.width) - 50, 0);
    this->sideBar.setSize(sf::Vector2f(50.f, static_cast<float>(resolution.height)));
    this->sideBar.setFillColor(sf::Color(50, 50, 50, 100));
    this->sideBar.setOutlineColor(sf::Color(200, 200, 200, 150));
    this->sideBar.setOutlineThickness(1.f);
    this->textureSelector = new TextureSelector(
            this->stateData.window->getSize().x - this->tileMap->getTileTextureSheet().getSize().x -
            this->sideBar.getSize().x, 0.f, this->stateData.gridSize,
            this->tileMap->getTileTextureSheet());
    this->previewTexture.setSize(sf::Vector2f(this->stateData.gridSize, this->stateData.gridSize));
    this->previewTexture.setFillColor(sf::Color(255, 255, 255, 100));
    this->previewTexture.setOutlineThickness(1.f);
    this->previewTexture.setOutlineColor(sf::Color::Red);
    this->previewTexture.setTexture(&this->tileMap->getTileTextureSheet());
    auto a = this->textureSelector->getSelected().getPosition();
    this->previewTexture.setTextureRect(sf::IntRect(a.x, a.y, this->stateData.gridSize, this->stateData.gridSize));
}

void EditorState::updateGui() {
    if (this->showTextureSelector) {
        this->textureSelector->update(this->mousePosWindow);
    }

    if (!this->textureSelector->isActive()) {
        this->previewTexture.setPosition(this->mousePosGrid.x * this->stateData.gridSize,
                                         this->mousePosGrid.y * this->stateData.gridSize);
        auto tileTexturePosition = this->textureSelector->getSelectedRelativePosition();
        this->previewTexture.setTextureRect(
                sf::IntRect(tileTexturePosition.x, tileTexturePosition.y, this->stateData.gridSize,
                            this->stateData.gridSize));
    }

    std::stringstream ss;
    this->cursorText.setPosition(this->mousePosView.x + 20, this->mousePosView.y - 20);
    ss << this->mousePosView.x << " x " << this->mousePosView.y << "\n"
       << this->mousePosGrid.x << this->mousePosGrid.y << "\n";
    this->cursorText.setString(ss.str());
}

void EditorState::renderGui(sf::RenderTarget *target) {
    target->setView(this->view);
    this->tileMap->render(*target);
    target->setView(this->stateData.window->getDefaultView());

    if (this->showTextureSelector) {
        this->textureSelector->render(*target);
    }
    target->draw(this->sideBar);
}

void EditorState::initTexts() {
    this->cursorText.setFont(*this->stateData.font);
    this->cursorText.setCharacterSize(12);
    this->cursorText.setFillColor(sf::Color::White);
    this->cursorText.setPosition(this->mousePosView.x, this->mousePosView.y - 20);
}
