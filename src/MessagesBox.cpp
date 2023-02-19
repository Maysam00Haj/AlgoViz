#include "MessagesBox.h"

#define BOX_COLOR sf::Color(107, 107, 107, 150)


MessagesBox::MessagesBox(sf::Font* font) {
    this->rectangle.setPosition(10, 820);
    this->rectangle.setSize({1000, 150});
    this->rectangle.setFillColor(BOX_COLOR);
    this->rectangle.setOutlineColor(sf::Color::White);
    this->rectangle.setOutlineThickness(1.f);
    this->messages =
    {
            "Click on the white circle to add a node"
    };
    sf::Text* text = new sf::Text();
    text->setString(messages[0]);
    text->setFont(*font);
    text->setFillColor(sf::Color::White);
    text->setCharacterSize(20);
    text->setPosition(20, 830);
    this->texts.push_back(text);
    this->is_rendered.push_back(true);
}


MessagesBox& MessagesBox::operator=(const MessagesBox& other) {
    if (this != &other) {
        this->rectangle = other.rectangle;
        this->messages = other.messages;
        this->texts = other.texts;
        this->is_rendered = other.is_rendered;
    }
    return *this;
}

MessagesBox::MessagesBox(MessagesBox &other) {
    this->rectangle = other.rectangle;
    this->messages = other.messages;
    this->texts = other.texts;
    this->is_rendered = other.is_rendered;
}


void MessagesBox::render(sf::RenderTarget& target) {
    target.draw(this->rectangle);
    for (int i = 0; i < this->is_rendered.size(); i++)
        if (is_rendered[i]) {
            target.draw(*this->texts[0]);
        }
}
