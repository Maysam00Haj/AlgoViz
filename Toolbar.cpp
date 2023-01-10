#include "Toolbar.h"
#include <iostream>

Button::Button(float x, float y, float width, float height, std::string txt, int id) {
    this->buttonState = BUTTON_IDLE;
    this->id = id;
    this->shape.setPosition(sf::Vector2f(x, y));
    this->shape.setSize(sf::Vector2f(width, height));
    this->shape.setOutlineThickness(1.f);
    //Todo: fix this.
    //text isn't being printed
    this->text = sf::Text();
    this->text.setString(txt);
    this->text.setPosition(sf::Vector2f(x, y));
    this->text.setFillColor(sf::Color::Red);
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
    target.draw(this->text);
}

//-----------------------------------------------------------------------------------------------------

Toolbar::Toolbar() {
    std::vector<std::string> icons = {"add node", "add edge", "erase"};
    for (unsigned int i = 0; i < icons.size(); i++) {
        this->Buttons.push_back(std::make_shared<Button>(20, 20 + ((50 + 10) * i), 50, 50, icons[i], i));
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

