#ifndef SFML_MAINMENUSTATE_H
#define SFML_MAINMENUSTATE_H

#include "GameState.h"
#include "EditorState.h"
#include "../Resource Files/Button.h"

class MainMenuState : public State {
public:
    MainMenuState(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys, std::stack<State *> *states);

    ~MainMenuState() override;

    //Functions
    void updateInput(const float &dt) override;

    void update(const float &dt) override;

    void render(sf::RenderTarget *target) override;

private:
    //Variables
    sf::Texture backgroundTexture;
    sf::RectangleShape background;
    sf::Font font;

    std::map<std::string, Button *> buttons;

    //Functions
    void initVariables();

    void initBackground();

    void initFonts();

    void initKeybinds() override;

    void initButtons();

    void updateButtons();

    void renderButtons(sf::RenderTarget &target);

public:
    void handleEvent(sf::Event &event, const float &dt) override;
};


#endif //SFML_MAINMENUSTATE_H
