//
// Created by User on 3/11/2023.
//

#ifndef ALGOVIZ_INTERFACE_H
#define ALGOVIZ_INTERFACE_H

#include <memory>
#include <SFML/Graphics.hpp>

class Button;
class Toolbar;
class TextBox;
class SavedGraphsList;


//-----------------------------------------------SavedGraphsList Class--------------------------------------------------

class SavedGraphsList {
    std::map<std::string, std::shared_ptr<sf::RectangleShape>> saved_graphs;
    unsigned int saved_graphs_num = 0;
    sf::Font* text_font = nullptr;
public:
    SavedGraphsList() = default;
    explicit SavedGraphsList(std::ifstream& file, sf::Font* font);
    ~SavedGraphsList() = default;
    void addGraph(const std::string& graph_name);
    void deleteGraph(const std::string& graph_name);
    bool contains(const std::string& graph_name);
    [[nodiscard]] bool empty() const;
    void render(sf::RenderWindow& window);
    std::string getClickedGraph(float pos_x, float pos_y);

};

//-------------------------------------------------Button Class---------------------------------------------------------

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
    LOAD_FROM_FILE
};

class Button {
    sf::RectangleShape shape;
    std::string icon_path;
    ButtonId id;

public:
    Button(float x, float y, float width, float height, const std::string& txt, ButtonId id);
    Button(): id(CURSOR){}
    ~Button();
    Button& operator=(const Button& other);
    bool update(const sf::Vector2f& mousePosWindow);
    void setButtonDisabled();
    void setButtonEnabled();
    void render(sf::RenderTarget& target);
    ButtonId getId() const;

};


//-------------------------------------------------Toolbar Class--------------------------------------------------------


class Toolbar {
private:
    std::shared_ptr<Button> active_button;
    std::vector<std::shared_ptr<Button>> buttons;
    sf::RectangleShape rectangle;
    std::vector<sf::RectangleShape> horizontal_separators;

public:
    Toolbar();
    ~Toolbar() = default;
    bool updateActiveButton(const sf::Vector2f& mousePosWindow);
    void render(sf::RenderWindow& window, bool mid_run = false);
    void resetActiveButton(); //used after 'CLEAN'
    ButtonId getActiveButtonId() const;
};



//-------------------------------------------------TextBox Class--------------------------------------------------------


class TextBox {
    sf::RectangleShape text_box;
    sf::RectangleShape text_field;
    sf::Font* text_font;
public:
    explicit TextBox(sf::RenderWindow& window, sf::Font* font);
    ~TextBox() = default;
    bool close(float event_x, float event_y);
    virtual void render(sf::RenderWindow& window, const std::string& txt);
};

class InputBox : TextBox {
    std::string input;
    static char unicodeToAscii(unsigned int uni_char);
public:
    explicit InputBox(sf::RenderWindow& window, sf::Font* font): TextBox(window, font){};
    void render(sf::RenderWindow& window, const std::string& txt) override;
    std::string getInput(sf::RenderWindow& window);
};

class OutputBox : TextBox {
    const std::string output;
public:
    explicit OutputBox(sf::RenderWindow& window, sf::Font* font): TextBox(window, font){};
    void render(sf::RenderWindow& window, const std::string& txt) override;
};


//----------------------------------------------------------------------------------------------------------------------
#endif //ALGOVIZ_INTERFACE_H
