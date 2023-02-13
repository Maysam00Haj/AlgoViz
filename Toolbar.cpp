#include "Toolbar.h"
#include <iostream>
#include <filesystem>

extern bool algo_thread_is_running;

Button::Button(float x, float y, float width, float height, const std::string& txt, ButtonId id) {
    this->buttonState = BUTTON_IDLE;
    this->id = id;
    auto* texture = new sf::Texture();
    texture->loadFromFile(txt);
    this->shape.setTexture(texture);
    this->shape.setPosition(sf::Vector2f(x, y));
    this->shape.setSize(sf::Vector2f(width, height));
    this->shape.setOutlineThickness(4.f);
}

Button::~Button() {
    delete this->shape.getTexture();
}

bool Button::isPressed() const {
    if (this->buttonState == BUTTON_ACTIVE)
        return true;

    return false;
}

ButtonId Button::getId() const {
    return this->id;
}

void Button::setId(ButtonId id) {
    this->id = id;
}

bool Button::update(const sf::Vector2i& mousePosWindow) {
    //Mouse click is in bounds
    if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {
        this->buttonState = BUTTON_ACTIVE;
        this->shape.setOutlineColor(sf::Color::Red);
        return true;
    }
    return false;
}

void Button::setButtonEnabled() {
    this->shape.setOutlineColor(CLICKED_BUTTON_COLOR);
}

void Button::setButtonDisabled() {
    this->shape.setOutlineColor(DEFAULT_BUTTON_COLOR);
}

void Button::render(sf::RenderTarget& target) {
    target.draw(this->shape);
    target.draw(this->text);
}

//----------------------------------------Toolbar Methods---------------------------------------------

Toolbar::Toolbar() {
    std::vector<std::string> icons = {"cursor", "add_node", "add_edge", "erase", "change_start_node", "change_edge_weight", "run_bfs", "run_dfs", "run_dijkstra", "end", "reset", "clear_window"};
    std::vector<ButtonId> id_list = {CURSOR, ADD_NODE, ADD_EDGE, ERASE, CHANGE_START_NODE, CHANGE_EDGE_WEIGHT, RUN_BFS, RUN_DFS, RUN_DIJKSTRA, END, RESET, CLEAR_WINDOW};
    for (unsigned int i = 0; i < icons.size(); i++) {
        this->buttons.push_back(std::make_shared<Button>(20, 20 + ((60 + 20) * i), 60, 60, "./icons/" + icons[i] + ".png", id_list[i]));
    }
    this->active_button = buttons[0];
}

ButtonId Toolbar::getActiveButtonId() const {
    return this->active_button->getId();
}

bool Toolbar::updateActiveButton(const sf::Vector2i& mousePosWindow) {
    for (auto& button : this->buttons) {
        if (button->update(mousePosWindow)) {
            this->active_button->setButtonDisabled();
            this->active_button = button;
            this->active_button->setButtonEnabled();
            return true;
        }
    }

    return false;
}

void Toolbar::render(sf::RenderTarget& target) {
    std::vector<ButtonId> not_to_render_while_running = {CURSOR, ADD_NODE, ADD_EDGE, ERASE, CHANGE_START_NODE, CHANGE_EDGE_WEIGHT, RUN_BFS, RUN_DFS, RUN_DIJKSTRA};
    for (const auto& button : this->buttons) {
        if (    algo_thread_is_running &&
                find(not_to_render_while_running.begin(), not_to_render_while_running.end(), button->getId()) !=
                not_to_render_while_running.end()
                ) {
            continue;
        }
        button->render(target);
    }
}

void Toolbar::resetActiveButton() {
    this->active_button->setButtonDisabled();
    this->active_button = buttons[0];
    this->active_button->setButtonEnabled();
}

