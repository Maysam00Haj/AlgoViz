//
// Created by User on 3/1/2023.
//
#include "TextBox.h"
#include <iostream>

using std::cout;
using std::endl;

TextBox::TextBox(sf::RenderWindow& window, sf::Font* font): text_font(font) {
    this->text_box.setPosition((float)window.getPosition().x+(float)window.getSize().x/4, (float)window.getPosition().y+(float)window.getSize().y/4);
    this->text_field.setPosition((float)this->text_box.getPosition().x+15, (float)this->text_box.getPosition().y+70);
    this->text_box.setSize(sf::Vector2f(200, 120));
    this->text_field.setSize(sf::Vector2f(170, 30));
    this->text_box.setFillColor(sf::Color(100,100,100,255));
    this->text_field.setFillColor(sf::Color(150,150,150,255));
    this->text_box.setOutlineThickness(1);
    this->text_box.setOutlineColor(sf::Color::White);
}


bool TextBox::close(float event_x, float event_y) {
    return (!this->text_box.getGlobalBounds().contains(event_x, event_y));
}

void TextBox::render(sf::RenderWindow& window, const std::string& txt) {
    sf::Text t;
    t.setString(txt);
    t.setFont(*this->text_font);
    t.setFillColor(sf::Color::White);
    t.setCharacterSize(20);
    t.setPosition(this->text_field.getPosition().x, this->text_field.getPosition().y);
    window.draw(this->text_box);
    window.draw(this->text_field);
    window.draw(t);
    window.display();
}


void InputBox::render(sf::RenderWindow &window, const std::string &txt) {
    TextBox::render(window, txt);
}

std::string InputBox::getInput(sf::RenderWindow& window) {
    this->input = "";
    sf::Event event{};
    window.pollEvent(event);
    while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        if (event.type == sf::Event::KeyPressed && !sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            char c = unicodeToAscii(event.text.unicode);
            if (c == 8) {
                this->input = this->input.substr(0, this->input.size()-1);
            }
            else if (c != 0) {
                this->input += unicodeToAscii(event.text.unicode);
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::Resized) {
            if (close((float)event.mouseButton.x, (float)event.mouseButton.y)) return "";
        }
        this->render(window, this->input);
        window.pollEvent(event);
    }
    return this->input;
}


void OutputBox::render(sf::RenderWindow &window, const std::string &txt) {
    TextBox::render(window, txt);
}

char InputBox::unicodeToAscii(unsigned int uni_char) {
    if (uni_char <= 25) {
        return 'a' + uni_char;
    }
    else if (uni_char <= 35) {
        return '0' + uni_char-26;
    }
    else if (uni_char >= 75 && uni_char <= 84) {
        return '0' + uni_char-75;
    }
    else if (uni_char == 56) {
        return '-';
    }
    else if (uni_char == 57) {
        return ' ';
    }
    else if (uni_char == 59) {
        return 8;
    }

    return 0;
}
