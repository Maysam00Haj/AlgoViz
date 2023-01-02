#ifndef ALGOVIZ_TOOLBAR_H
#define ALGOVIZ_TOOLBAR_H
#include <memory>
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
    bool isPressed() const;
    int getId() const;
    void setId(int id);
    bool update(const sf::Vector2i& mousePosWindow);
    void render(sf::RenderTarget& target);
};


class Toolbar {
private:
    std::shared_ptr<Button> activeButton;
    std::vector<std::shared_ptr<Button>> Buttons;

public:
    Toolbar(float x, float y, float width, float height, std::vector<std::string>& buttonIcons);
    ~Toolbar();
    int getActiveButtonId() const;
    void update(const sf::Vector2i& mousePosWindow);
    void render(sf::RenderTarget& target);
};




#endif //ALGOVIZ_TOOLBAR_H
