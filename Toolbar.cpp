#include "Toolbar.h"
#include <iostream>

Button::Button(float x, float y, float width, float height, std::string iconPath, int id) {
    this->buttonState = BUTTON_IDLE;
    this->id = id;
    this->shape.setPosition(sf::Vector2f(x, y));
    this->shape.setSize(sf::Vector2f(width, height));
    this->shape.setOutlineThickness(1.f);

    sf::Texture texture;
    texture.loadFromFile(iconPath); // TODO: exception
    this->shape.setTexture(&texture);
}

Button::~Button() {}

const bool Button::isPressed() const {
    if (this->buttonState == BUTTON_ACTIVE)
        return true;

    return false;
}

const int Button::getId() const {
    return this->id;
}

void Button::setId(int id) {
    this->id = id;
}

void Button::update(const sf::Vector2i& mousePosWindow) {
    this->buttonState = BUTTON_IDLE;
    if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {
        //Pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            this->buttonState = BUTTON_ACTIVE;
        }
    }
}

void Button::render(sf::RenderTarget& target) {
    target.draw(this->shape);
}
