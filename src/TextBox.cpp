//
// Created by User on 3/1/2023.
//
#include "TextBox.h"
#include <iostream>

using std::cout;
using std::endl;

TextBox::TextBox(sf::RenderWindow& window) {
    this->text_box.setPosition((float)window.getPosition().x+(float)window.getSize().x/4, (float)window.getPosition().y+(float)window.getSize().y/4);
    this->text_field.setPosition((float)this->text_box.getPosition().x+10, (float)this->text_box.getPosition().y+70);
    this->text_box.setSize(sf::Vector2f(200, 120));
    this->text_field.setSize(sf::Vector2f(180, 30));
    this->text_box.setFillColor(sf::Color(100,100,100,255));
    this->text_field.setFillColor(sf::Color(150,150,150,255));
}


bool TextBox::close(float event_x, float event_y) {
    return (!this->text_box.getGlobalBounds().contains(event_x, event_y));
}

void TextBox::render(sf::RenderWindow& window, const std::string& txt) {
    sf::Text t;
    t.setString(txt);
    t.setFillColor(sf::Color::White);
    t.setCharacterSize(20);
    t.setPosition(this->text_field.getPosition().x, this->text_field.getPosition().y);
    t.setLineSpacing(2);
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
            this->input += static_cast<char>(event.text.unicode);
            cout<< static_cast<char>(event.text.unicode)<<endl;
        }
        else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::Resized) {
            if (close((float)event.mouseButton.x, (float)event.mouseButton.y))
            return "";
        }
        this->render(window, this->input);
        window.pollEvent(event);
    }
    return this->input;
}


void OutputBox::render(sf::RenderWindow &window, const std::string &txt) {
    TextBox::render(window, txt);
}
