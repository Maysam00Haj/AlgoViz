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
    std::vector<std::string> saved_graphs;
public:
    SavedGraphsList() = default;
    ~SavedGraphsList() = default;
    void render(sf::RenderWindow& window);

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
    LOAD_FROM_FILE,
    MINIMIZE,
    MAXIMIZE
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
    SavedGraphsList saved_graphs_list;
    sf::RectangleShape rectangle;
    std::vector<sf::RectangleShape> horizontal_separators;

public:
    Toolbar();
    ~Toolbar() = default;
    bool updateActiveButton(const sf::Vector2f& mousePosWindow);
    void render(sf::RenderWindow& window, bool mid_run = false, bool load_list = false);
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


//-------------------------------------------------MessageBox Class-----------------------------------------------------


enum BoxState {
    MAXIMIZED,
    MINIMIZED
};

enum MessageType {
    EMPTY_GRAPH_M,
    CHANGE_START_NODE_M,
    DIJKSTRA_M,
};



class MessagesBox {
private:
    BoxState state = MAXIMIZED;
    Button maximize;
    Button minimize;
    sf::RectangleShape rectangle;
    sf::RectangleShape frame;
    std::map<MessageType, std::string> messages;
    std::vector<sf::Text*> texts;


public:
    MessagesBox() = default;
    explicit MessagesBox(sf::Font* font);
    void render(sf::RenderTarget& target);
    void minimizeBox();
    void maximizeBox();
    bool inBoundsMaximize(float x, float y);
    bool inBoundsMinimize(float x, float y);
    MessagesBox& operator=(const MessagesBox& other);


    std::map<MessageType, bool> is_rendered;
};


//----------------------------------------------------------------------------------------------------------------------
#endif //ALGOVIZ_INTERFACE_H
