#ifndef SFML_TEXTURESELECTOR_H
#define SFML_TEXTURESELECTOR_H


#include <Graphics/RectangleShape.hpp>
#include <Graphics/Sprite.hpp>

class TextureSelector {
private:
    sf::RectangleShape bounds;
    sf::Sprite sheet;
    sf::RectangleShape selector;
    sf::RectangleShape selected;
    float gridSize;
    bool active;

public:
    TextureSelector(float x, float y, float gridSize, const sf::Texture &texture_sheet);

    virtual ~TextureSelector();

    void update(sf::Vector2f mousePos);

    void render(sf::RenderTarget &target);

    bool isActive() const;

    /**
     * Restituisce la posizione della cella selezionata relativa al suo contenitore (TextureSelector)
     * @return
     */
    sf::Vector2f getSelectedRelativePosition();

    /**
     * Seleziona la tile in base alla posizione del mouse
     * @param mousePosWindow
     */
    void setSelectedTile(sf::Vector2f &mousePos);

    /**
     * Seleziona la tile prendendo una direzione
     * @param dir_x
     * @param dir_y
     */
    void setSelectedTile(int dir_x, int dir_y);

    /**
     * Restituisce il RectangleShape selezionato
     * @return
     */
    const sf::RectangleShape &getSelected() const;

    /**
     * Data una posizione assoluta (es: mousePosWindow),
     *  restituisce l'indice della cella che contiene quella posizione
     * @param absolutePosition
     * @return
     */
    sf::Vector2i getGridPosition(const sf::Vector2f &absolutePosition);
};


#endif //SFML_TEXTURESELECTOR_H
