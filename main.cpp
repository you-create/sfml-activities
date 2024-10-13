#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    RenderWindow window(sf::VideoMode(800, 600), "My window");
    Event event;
    CircleShape circle(50);
    circle.setFillColor(Color::Black);
    while (window.isOpen())
    {

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed://close button
                    window.close();
                    break;
                case Event::KeyPressed://movement + q for quit
                    if (event.key.code == Keyboard::W)
                    {
                        circle.move(0,-20);
                    } else if (event.key.code == Keyboard::A)
                    {
                        circle.move(-20,0);
                    } else if (event.key.code == Keyboard::S)
                    {
                        circle.move(0,20);
                    } else if (event.key.code == Keyboard::D)
                    {
                        circle.move(20,0);
                    } else if (event.key.code == Keyboard::Q)
                    {
                        window.close();
                    }
                    break;
            }
        }
        window.clear(Color::Blue);
        window.draw(circle);
        window.display();
    }
    return 0;
}