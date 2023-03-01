//
// Created by User on 3/1/2023.
//
#ifndef ALGOVIZ_TEXTBOX_H
#define ALGOVIZ_TEXTBOX_H

#include <SFML/Graphics.hpp>
#include <string>

class TextBox {
    sf::RectangleShape text_box;
    sf::RectangleShape text_field;
public:
    explicit TextBox(sf::RenderWindow& window);
    ~TextBox() = default;
    bool close(float event_x, float event_y);
    virtual void render(sf::RenderWindow& window, const std::string& txt);
};

class InputBox : TextBox {
    std::string input;
public:
    explicit InputBox(sf::RenderWindow& window): TextBox(window){};
    void render(sf::RenderWindow& window, const std::string& txt) override;
    std::string getInput(sf::RenderWindow& window);
};

class OutputBox : TextBox {
    const std::string output;
public:
    explicit OutputBox(sf::RenderWindow& window): TextBox(window){};
    void render(sf::RenderWindow& window, const std::string& txt) override;
};

#endif //ALGOVIZ_TEXTBOX_H
