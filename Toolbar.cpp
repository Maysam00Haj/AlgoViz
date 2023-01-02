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

bool Button::isPressed() const {
    if (this->buttonState == BUTTON_ACTIVE)
        return true;

    return false;
}

int Button::getId() const {
    return this->id;
}

void Button::setId(int id) {
    this->id = id;
}

bool Button::update(const sf::Vector2i& mousePosWindow) {
    //Mouse is in bounds
    if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {
        //Pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            this->buttonState = BUTTON_ACTIVE;
            return true;
        }
    }
    return false;
}

void Button::render(sf::RenderTarget& target) {
    target.draw(this->shape);
}

//-----------------------------------------------------------------------------------------------------


Toolbar::Toolbar(float x, float y, float width, float height, std::vector<std::string>& buttonIcons) {
    for (unsigned int i = 0; i < buttonIcons.size(); i++) {
        this->Buttons.push_back(std::make_shared<Button>(x, y + (i + 1) * height, width, height,buttonIcons[i],i));
    }
}

Toolbar::~Toolbar() {}

void Toolbar::update(const sf::Vector2i& mousePosWindow) {
    for (auto& i : this->Buttons) {
        if (i->update(mousePosWindow)) this->activeButton = i;
    }
}

void Toolbar::render(sf::RenderTarget& target) {
    for (auto& i : this->Buttons) {
        i->render(target);
    }
}

