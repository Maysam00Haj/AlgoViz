#ifndef ALGOVIZ_TOOLBAR_H
#define ALGOVIZ_TOOLBAR_H
#include <memory>
#include <SFML/Graphics.hpp>

enum ButtonId {
    CURSOR,
    ADD_NODE,
    ADD_EDGE,
    ERASE,
    CHANGE_START_NODE,
    CHOOSE_TARGET_NODE,
    REMOVE_TARGET_NODE,
    RUN_BFS, RUN_DFS,
    RUN_DIJKSTRA,
    END,
    RESET,
    CLEAR_WINDOW
};

class Button {
    sf::RectangleShape shape;
    sf::Text text;
    ButtonId id;

public:
    Button(float x, float y, float width, float height, const std::string& txt, ButtonId id);
    ~Button();
    bool update(const sf::Vector2f& mousePosWindow);
    void setButtonDisabled();
    void setButtonEnabled();
    void render(sf::RenderTarget& target);
    ButtonId getId() const;
};


class Toolbar {
private:
    std::shared_ptr<Button> active_button;
    sf::RectangleShape rectangle;
    std::vector<sf::RectangleShape> horizontal_separators;

public:
    Toolbar();
    ~Toolbar() = default;
    bool updateActiveButton(const sf::Vector2f& mousePosWindow);
    void render(sf::RenderTarget& target);
    void resetActiveButton(); //used after 'CLEAN'
    ButtonId getActiveButtonId() const;

    std::vector<std::shared_ptr<Button>> buttons;
};




#endif //ALGOVIZ_TOOLBAR_H
