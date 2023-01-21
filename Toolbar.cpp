#include "Toolbar.h"
#include <iostream>

Button::Button(float x, float y, float width, float height, const std::string& txt, button_id id) {
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

bool Button::isPressed() const {
    if (this->buttonState == BUTTON_ACTIVE)
        return true;

    return false;
}

button_id Button::getId() const {
    return this->id;
}

void Button::setId(button_id id) {
    this->id = id;
}

bool Button::update(const sf::Vector2i& mousePosWindow) {
    //Mouse click is in bounds
    if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {
        this->buttonState = BUTTON_ACTIVE;
        return true;
    }
    return false;
}

void Button::render(sf::RenderTarget& target) {
    target.draw(this->shape);
    target.draw(this->text);
}

//-----------------------------------------------------------------------------------------------------

Toolbar::Toolbar() {
    std::vector<std::string> icons = {"add node", "add edge", "erase", "change start node", "start", "pause", "end", "clean"};
    std::vector<button_id> id_list = {ADD_NODE, ADD_EDGE, ERASE, CHANGE_START_NODE, START, PAUSE, END, CLEAN};
    for (unsigned int i = 0; i < icons.size(); i++) {
        this->buttons.push_back(std::make_shared<Button>(20, 20 + ((50 + 10) * i), 50, 50, icons[i], id_list[i]));
    }
    this->active_button = buttons[0];
}

button_id Toolbar::getActiveButtonId() const {
    return this->active_button->getId();
}

void Toolbar::update(const sf::Vector2i& mousePosWindow) {
    for (auto& button : this->buttons) {
        if (button->update(mousePosWindow)) this->active_button = button;
    }
}

void Toolbar::render(sf::RenderTarget& target) {
    for (const auto& button : this->buttons) {
        button->render(target);
    }
}

