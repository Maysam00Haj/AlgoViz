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

#endif //ALGOVIZ_TEXTBOX_H
