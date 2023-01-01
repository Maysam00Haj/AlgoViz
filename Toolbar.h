#ifndef ALGOVIZ_TOOLBAR_H
#define ALGOVIZ_TOOLBAR_H

#include <SFML/Graphics.hpp>

enum button_states {BUTTON_IDLE = 0, BUTTON_ACTIVE};

class Button {
private:
    button_states buttonState;
    sf::RectangleShape shape;
    int id;

public:
    Button(float x, float y, float width, float height, std::string iconPath, int id = 0);
    ~Button();
    const bool isPressed() const;
    const int getId() const;
    void setId(int id);
    void update(const sf::Vector2i& mousePosWindow);
    void render(sf::RenderTarget& target);
};




#endif //ALGOVIZ_TOOLBAR_H
