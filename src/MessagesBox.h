#ifndef ALGOVIZ_MESSAGESBOX_H
#define ALGOVIZ_MESSAGESBOX_H
#include "SFML/Graphics.hpp"
#include "Toolbar.h"

enum BoxState {
    REGULAR,
    MINIMIZED
};



class MessagesBox {
private:
    BoxState state = REGULAR;
    sf::RectangleShape minimized_rectangle;
    sf::RectangleShape rectangle;
    sf::RectangleShape frame;
    std::vector<std::string> messages;
    std::vector<sf::Text*> texts;


public:
    MessagesBox() = default;
    MessagesBox(sf::Font* font);
    MessagesBox(MessagesBox& other);
    ~MessagesBox() = default;
    void render(sf::RenderTarget& target);
    MessagesBox& operator=(const MessagesBox& other);


    std::vector<bool> is_rendered;
};




#endif //ALGOVIZ_MessagesSBOX_H
