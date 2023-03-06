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
    CLEAR_WINDOW,
    SAVE_TO_FILE,
    LOAD_FROM_FILE,
    MINIMIZE,
    MAXIMIZE
};


class SavedGraphsList {
    std::vector<std::string> saved_graphs;
public:
    SavedGraphsList() = default;
    ~SavedGraphsList() = default;
    void render(sf::RenderWindow& window);

};


class Button {
    sf::RectangleShape shape;
    std::string icon_path;
    ButtonId id;

public:
    Button(float x, float y, float width, float height, const std::string& txt, ButtonId id);
    Button() = default;
    ~Button();
    Button& operator=(const Button& other);
    bool update(const sf::Vector2f& mousePosWindow);
    void setButtonDisabled();
    void setButtonEnabled();
    void render(sf::RenderTarget& target);
    ButtonId getId() const;

};


class Toolbar {
private:
    std::shared_ptr<Button> active_button;
    std::vector<std::shared_ptr<Button>> buttons;
    SavedGraphsList saved_graphs_list;
    sf::RectangleShape rectangle;
    std::vector<sf::RectangleShape> horizontal_separators;

public:
    Toolbar();
    ~Toolbar() = default;
    bool updateActiveButton(const sf::Vector2f& mousePosWindow);
    void render(sf::RenderWindow& window, bool load_list);
    void resetActiveButton(); //used after 'CLEAN'
    ButtonId getActiveButtonId() const;
};




#endif //ALGOVIZ_TOOLBAR_H
