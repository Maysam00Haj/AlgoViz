#include "MessagesBox.h"

#define BOX_COLOR sf::Color(107, 107, 107, 150)


MessagesBox::MessagesBox(sf::Font* font) {
    this->rectangle.setPosition(10, 820);
    this->rectangle.setSize({1000, 150});
    this->rectangle.setFillColor(BOX_COLOR);
    this->rectangle.setOutlineColor(sf::Color::White);
    this->rectangle.setOutlineThickness(1.f);
    this->frame.setPosition(10, 820);
    this->frame.setSize({1000, 20});
    this->frame.setFillColor(sf::Color(107, 107, 107, 255));
    this->messages =
    {
            "Click on the white circle to add a node"
    };
    sf::Text* text = new sf::Text();
    text->setString(messages[0]);
    text->setFont(*font);
    text->setFillColor(sf::Color::White);
    text->setCharacterSize(20);
    text->setPosition(20, 850);
    this->texts.push_back(text);
    this->is_rendered.push_back(true);
    this->minimized_rectangle.setPosition(10, 880);
    this->minimized_rectangle.setSize({10, 100});
    this->minimized_rectangle.setFillColor(BOX_COLOR);
}


MessagesBox& MessagesBox::operator=(const MessagesBox& other) {
    if (this != &other) {
        this->rectangle = other.rectangle;
        this->messages = other.messages;
        this->texts = other.texts;
        this->is_rendered = other.is_rendered;
        this->state = other.state;
        this->minimized_rectangle = other.minimized_rectangle;
        this->frame = other.frame;
    }
    return *this;
}

MessagesBox::MessagesBox(MessagesBox &other) {
    this->rectangle = other.rectangle;
    this->messages = other.messages;
    this->texts = other.texts;
    this->is_rendered = other.is_rendered;
    this->state = other.state;
    this->minimized_rectangle = other.minimized_rectangle;
    this->frame = other.frame;
}


void MessagesBox::render(sf::RenderTarget& target) {
    if (this->state == MINIMIZED) {
        target.draw(this->minimized_rectangle);
    }
    else if (this->state == REGULAR) {
        target.draw(this->rectangle);
        target.draw(this->frame);
        for (int i = 0; i < this->is_rendered.size(); i++)
            if (is_rendered[i]) {
                target.draw(*this->texts[0]);
            }
    }
}
