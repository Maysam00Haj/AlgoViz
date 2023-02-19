#include "MessagesBox.h"

#define BOX_COLOR sf::Color(107, 107, 107, 150)


MessagesBox::MessagesBox(sf::Font* font) : minimize(975, 812.5, 30, 30, "./images/minimize.png", MINIMIZE),
                                           maximize(10, 920, 50, 50, "./images/messages.png", MAXIMIZE)
{
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
}


MessagesBox& MessagesBox::operator=(const MessagesBox& other) {
    if (this != &other) {
        this->rectangle = other.rectangle;
        this->messages = other.messages;
        this->texts = other.texts;
        this->is_rendered = other.is_rendered;
        this->state = other.state;
        this->maximize = other.maximize;
        this->frame = other.frame;
        this->minimize = other.minimize;
    }
    return *this;
}

void MessagesBox::render(sf::RenderTarget& target) {
    if (this->state == MINIMIZED) {
        this->maximize.render(target);
    }
    else if (this->state == MAXIMIZED) {
        target.draw(this->rectangle);
        target.draw(this->frame);
        this->minimize.render(target);
        for (int i = 0; i < this->is_rendered.size(); i++)
            if (is_rendered[i]) {
                target.draw(*this->texts[0]);
            }
    }
}

void MessagesBox::minimizeBox() {
    this->state = MINIMIZED;
}

void MessagesBox::maximizeBox() {
    this->state = MAXIMIZED;
}

bool MessagesBox::inBoundsMinimize(float x, float y) {
    return this->minimize.update({x, y});
}

bool MessagesBox::inBoundsMaximize(float x, float y) {
    return this->maximize.update({x, y});
}
