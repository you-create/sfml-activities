#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>

using namespace sf;

int main()
{
    RenderWindow window(sf::VideoMode(800, 600), "My window");
    window.setFramerateLimit(60);
    Event event;

    Clock clock; //restarts every frame

    //speed settings
    constexpr float xInit {50};
    constexpr float yInit {xInit};
    Vector2f v(xInit,yInit);
    constexpr float incr {10}; //acc

    //float gravity {100};

    //circle instantiation
    CircleShape circle(50);
    circle.setFillColor(Color::Black);
    circle.setPosition(0, 0); //sets initial position

    bool playingUp {false};
    bool playingLeft {false};
    bool playingDown {false};
    bool playingRight {false};

    while (window.isOpen()) //each run-through is a frame
    {
        Time dt = clock.restart(); //delta time
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed://close button
                    window.close();
                    break;
            }
        }

        //movement
        //origin is top-left. imagine window as 1st cartesian quadrant flipped on x-axis

        //up
        if (Keyboard::isKeyPressed(Keyboard::Key::Up))
        {//while holding, accelerate
            v.y -= incr;
            circle.move(0,v.y*dt.asSeconds());
            playingUp = true;
        } else if (playingUp)
        {//after letting go
            if ((v.y < incr && v.y > -incr) || Keyboard::isKeyPressed(Keyboard::Key::Down))
            {//near zero v OR opp key pressed -> stop this control
                playingUp = false;
            }
            else if (v.y >= incr)
            {//decelerate
                v.y -= incr;
                circle.move(0,v.y*dt.asSeconds());
            }
            else if (v.y <= -incr)
            {//decelerate
                v.y += incr;
                circle.move(0,v.y*dt.asSeconds());
            }
        }

        //down
        else if (Keyboard::isKeyPressed(Keyboard::Key::Down))
        {
            v.y += incr;
            circle.move(0,v.y*dt.asSeconds());
            playingDown = true;
        } else if (playingDown)
        {
            if ((v.y < incr && v.y > -incr) || Keyboard::isKeyPressed(Keyboard::Key::Up))
            {
                playingDown = false;
            }
            else if (v.y >= incr)
            {
                v.y -= incr;
                circle.move(0,v.y*dt.asSeconds());
            }
            else if (v.y <= -incr)
            {
                v.y += incr;
                circle.move(0,v.y*dt.asSeconds());
            }
        }

        //left
        if (Keyboard::isKeyPressed(Keyboard::Key::Left))
        {
            v.x -= incr;
            circle.move(v.x*dt.asSeconds(),0);
            playingLeft = true;
        } else if (playingLeft)
        {
            if ((v.x < incr && v.x > -incr) || Keyboard::isKeyPressed(Keyboard::Key::Right))
            {
                playingLeft = false;
            }
            else if (v.x >= incr)
            {
                v.x -= incr;
                circle.move(v.x*dt.asSeconds(),0);
            }
            else if (v.x <= -incr)
            {
                v.x += incr;
                circle.move(v.x*dt.asSeconds(),0);
            }
        }

        //right
        else if (Keyboard::isKeyPressed(Keyboard::Key::Right))
        {
            v.x += incr;
            circle.move(v.x*dt.asSeconds(),0);
            playingRight = true;
        } else if (playingRight)
        {
            if ((v.x < incr && v.x > -incr) || Keyboard::isKeyPressed(Keyboard::Key::Left))
            {
                playingRight = false;
            }
            else if (v.x >= incr)
            {
                v.x -= incr;
                circle.move(v.x*dt.asSeconds(),0);
            }
            else if (v.x <= -incr)
            {
                v.x += incr;
                circle.move(v.x*dt.asSeconds(),0);
            }
        }

        //effect of 'gravity'
        //circle.move(0,gravity*dt.asSeconds());

        //repositioning when out of frame
        if (circle.getPosition().y > 600) {
            circle.setPosition(circle.getPosition().x, 0);
        }
        if (circle.getPosition().y < 0) {
            circle.setPosition(circle.getPosition().x, 600);
        }
        if (circle.getPosition().x > 800) {
            circle.setPosition(0, circle.getPosition().y);
        }
        if (circle.getPosition().x < 0) {
            circle.setPosition(800, circle.getPosition().y);
        }

        window.clear(Color::Blue);
        window.draw(circle);
        window.display();
    }
    return 0;
}
