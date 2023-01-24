#ifndef ALGOVIZ_TOOLBAR_H
#define ALGOVIZ_TOOLBAR_H
#include <memory>
#include <SFML/Graphics.hpp>

enum button_states {BUTTON_IDLE = 0, BUTTON_ACTIVE};
enum button_id {ADD_NODE, ADD_EDGE, ERASE, CHANGE_START_NODE, START, PAUSE, END, RESET, CLEAR_WINDOW};

class Button {
    button_states buttonState;
    sf::RectangleShape shape;
    sf::Text text;
    button_id id;

public:
    Button(float x, float y, float width, float height, const std::string& txt, button_id id);
    ~Button();
    bool isPressed() const;
    button_id getId() const;
    void setId(button_id id);
    bool update(const sf::Vector2i& mousePosWindow);
    void render(sf::RenderTarget& target);
};


class Toolbar {
private:
    std::shared_ptr<Button> active_button;

public:
    Toolbar();
    ~Toolbar() = default;
    button_id getActiveButtonId() const;
    void update(const sf::Vector2i& mousePosWindow);
    void render(sf::RenderTarget& target);
    void updateActiveButton(); //used after 'CLEAN'

    std::vector<std::shared_ptr<Button>> buttons;
};




#endif //ALGOVIZ_TOOLBAR_H
