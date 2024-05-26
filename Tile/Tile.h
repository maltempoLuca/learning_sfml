#ifndef SFML_TILE_H
#define SFML_TILE_H

#include<iostream>
#include<fstream>
#include<cstdlib>
#include<sstream>
#include<vector>

#include"SFML/System.hpp"
#include"SFML/Window.hpp"
#include"SFML/Graphics.hpp"
#include"../enums/tile_types.cpp"

class Tile {
private:
    std::string getTypesAsString() const;

    std::string getSpritesAsString() const;

    void initShapes(float x, float y, float gridSizeF, sf::Texture &textureSheet, const std::vector<sf::Vector2f> &texturePositions);

protected:
    std::vector<sf::RectangleShape> sprites;

    std::vector<TILE_TYPES> tileTypes;
public:

    Tile(float x, float y, float gridSizeF, sf::Texture &texture, const std::vector<sf::Vector2f> &texturePositions,
         const std::vector<TILE_TYPES> &tileTypes);

    virtual ~Tile();

    void update();

    void render(sf::RenderTarget &target);

    std::string getAsString(unsigned x, unsigned y, unsigned z) const;

    bool isOfType(TILE_TYPES type);
};

#endif //SFML_TILE_H
