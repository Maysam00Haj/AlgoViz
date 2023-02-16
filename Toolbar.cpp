#include "Toolbar.h"
#include <iostream>
#include <filesystem>

extern bool algo_thread_is_running;

Button::Button(float x, float y, float width, float height, const std::string& txt, ButtonId id) : buttonState(BUTTON_IDLE), id(id) {
    auto* texture = new sf::Texture();
    texture->loadFromFile(txt);
    this->shape.setTexture(texture);
    this->shape.setPosition(sf::Vector2f(x, y));
    this->shape.setSize(sf::Vector2f(width, height));

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

bool Button::update(const sf::Vector2f& mousePosWindow) {
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
    std::vector<std::string> icons = {"cursor", "add_node", "add_edge", "erase", "change_start_node", "choose_target_node", "remove_target_node", "run_bfs", "run_dfs", "run_dijkstra", "end", "reset", "clear_window"};
    std::vector<ButtonId> id_list = {CURSOR, ADD_NODE, ADD_EDGE, ERASE, CHANGE_START_NODE, CHOOSE_TARGET_NODE, REMOVE_TARGET_NODE, RUN_BFS, RUN_DFS, RUN_DIJKSTRA, END, RESET, CLEAR_WINDOW};
    this->buttons.push_back(std::make_shared<Button>(32.5, 15, 30, 30, "./icons/cursor.png", CURSOR));
    this->buttons.push_back(std::make_shared<Button>(35, 60, 30, 30, "./icons/add_node.png", ADD_NODE));
    this->buttons.push_back(std::make_shared<Button>(35, 110, 30, 30, "./icons/add_edge.png", ADD_EDGE));
    this->buttons.push_back(std::make_shared<Button>(35, 162.5, 30, 30, "./icons/erase.png", ERASE));
    this->buttons.push_back(std::make_shared<Button>(35, 210, 30, 30, "./icons/change_start_node.png", CHANGE_START_NODE));
    this->buttons.push_back(std::make_shared<Button>(35, 260, 30, 30, "./icons/choose_target_node.png", CHOOSE_TARGET_NODE));
    this->buttons.push_back(std::make_shared<Button>(35, 310, 30, 30, "./icons/remove_target_node.png", REMOVE_TARGET_NODE));
    this->buttons.push_back(std::make_shared<Button>(25, 360, 50, 30, "./icons/run_bfs.png", RUN_BFS));
    this->buttons.push_back(std::make_shared<Button>(25, 410, 50, 30, "./icons/run_dfs.png", RUN_DFS));
    this->buttons.push_back(std::make_shared<Button>(15, 460, 70, 30, "./icons/run_dijkstra.png", RUN_DIJKSTRA));
    this->buttons.push_back(std::make_shared<Button>(35, 510, 30, 30, "./icons/end.png", END));
    this->buttons.push_back(std::make_shared<Button>(35, 560, 30, 30, "./icons/reset.png", RESET));
    this->buttons.push_back(std::make_shared<Button>(35, 615, 30, 30, "./icons/clear_window.png", CLEAR_WINDOW));
    this->rectangle.setPosition(10, 10);
    this->rectangle.setSize({80, 660});
    this->rectangle.setFillColor(sf::Color(107, 107, 107, 255));
    this->rectangle.setOutlineColor(sf::Color::White);
    this->rectangle.setOutlineThickness(2.f);

    for (int i = 0; i < icons.size(); i++) {
        sf::RectangleShape *s1 = new sf::RectangleShape();
        s1->setSize(sf::Vector2f(2, 60));
        s1->setFillColor(sf::Color(81, 78, 78, 200));
        s1->setPosition(80, 50 * (i + 1));
        s1->setRotation(90);
        this->horizontal_separators.push_back(s1);
    }

    this->active_button = buttons[0];
}

Toolbar::~Toolbar() {
    for (int i = 0; i < this->buttons.size(); i++) delete this->horizontal_separators[i];
}

ButtonId Toolbar::getActiveButtonId() const {
    return this->active_button->getId();
}

bool Toolbar::updateActiveButton(const sf::Vector2f& mousePosWindow) {
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
    std::vector<ButtonId> not_to_render_while_running = {CURSOR, ADD_NODE, ADD_EDGE, ERASE, CHANGE_START_NODE, CHOOSE_TARGET_NODE, REMOVE_TARGET_NODE, RUN_BFS, RUN_DFS, RUN_DIJKSTRA};
    target.draw(this->rectangle);

    for (int i = 0; i < this->horizontal_separators.size(); i++) {
        target.draw(*this->horizontal_separators[i]);
    }

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

