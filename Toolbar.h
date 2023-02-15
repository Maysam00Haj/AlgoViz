#ifndef ALGOVIZ_TOOLBAR_H
#define ALGOVIZ_TOOLBAR_H
#include <memory>
#include <SFML/Graphics.hpp>

#define CLICKED_BUTTON_COLOR (sf::Color(189, 195, 199, 100))
#define DEFAULT_BUTTON_COLOR (sf::Color::White)

enum ButtonStates
{
    BUTTON_IDLE = 0,
    BUTTON_ACTIVE
};

enum ButtonId
{
    CURSOR,
    ADD_NODE,
    ADD_EDGE,
    ERASE,
    CHANGE_START_NODE,
    RUN_BFS, RUN_DFS,
    RUN_DIJKSTRA,
    END,
    RESET,
    CLEAR_WINDOW
};

class Button {
    ButtonStates buttonState;
    sf::RectangleShape shape;
    sf::Text text;
    ButtonId id;

public:
    Button(float x, float y, float width, float height, const std::string& txt, ButtonId id);
    ~Button();
    bool isPressed() const;
    ButtonId getId() const;
    void setId(ButtonId id);
    bool update(const sf::Vector2i& mousePosWindow);
    void setButtonDisabled();
    void setButtonEnabled();
    void render(sf::RenderTarget& target);
};


class Toolbar {
private:
    std::shared_ptr<Button> active_button;
    sf::RectangleShape rectangle;
    std::vector<sf::RectangleShape*> horizontal_separators;

public:
    Toolbar();
    ~Toolbar();
    ButtonId getActiveButtonId() const;
    bool updateActiveButton(const sf::Vector2i& mousePosWindow);
    void render(sf::RenderTarget& target);
    void resetActiveButton(); //used after 'CLEAN'

    std::vector<std::shared_ptr<Button>> buttons;
};




#endif //ALGOVIZ_TOOLBAR_H
