//
// Created by malte on 19/01/2024.
//

#include "Tile.h"

Tile::Tile(float x, float y, float gridSizeF, sf::Texture &texture, sf::Vector2f &tileTexturePosition,
           bool collision, TILE_TYPES type) {
    this->collision = collision;
    this->type = type;
    this->shape.setSize(sf::Vector2f(gridSizeF, gridSizeF));
    this->shape.setPosition(x, y);
    this->shape.setTexture(&texture);
    this->shape.setOutlineColor(sf::Color::Black);
    this->shape.setOutlineThickness(1.f);
    this->shape.setTextureRect(sf::IntRect(tileTexturePosition.x, tileTexturePosition.y, gridSizeF, gridSizeF));
}

Tile::~Tile() {

}

void Tile::render(sf::RenderTarget &target) {
    target.draw(this->shape);
}

void Tile::update() {

}

std::string Tile::getAsString() const {
    std::stringstream ss;
    ss << this->shape.getTextureRect().left << " " << this->shape.getTextureRect().top << " " << this->collision << " "
       << (int) this->type;
    return ss.str();
}
