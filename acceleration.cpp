#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace sf;

//bool drag(Vector2f &v, float &hyp, float &rad, float &drg,
    //float &near0, bool playing, Time &dt, CircleShape &circle);
void setRad(Vector2f &v, float &rad, float &hyp);

int main()
{
    RenderWindow window(sf::VideoMode(800, 600), "My window");
    window.setFramerateLimit(60);
    Event event;

    Clock clock; //restarts every frame

    //movement settings
    //inputs and .move() modify cartesian coords
    //drag modifies polar coords
    Vector2f v(0,0); //velocity cartesian coords
    float hyp {0}; //hypotenuse of vel for polar coords
    float rad {0}; //angle cw from +ve x axis in radians of vel for polar coords
    constexpr float accUp {30}; //acceleration (how much is added to vel per frame)
    constexpr float accDown {15};
    constexpr float accLeft {15};
    constexpr float accRight {15};
    float drg {10}; //deceleration
    float near0 {5}; //how close to 0 controls stop (+-)
    float g {15}; //gravity

    //circle instantiation
    CircleShape circle(50);
    circle.setFillColor(Color::Black);
    circle.setPosition(350, 250);

    while (window.isOpen()) //each iteration is a frame
    {
        window.setFramerateLimit(60);
        Time dt = clock.restart(); //delta time

        //close button
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) window.close();
        }

        //effect of drag
        hyp = sqrt(v.y*v.y+v.x*v.x);
        if (v.x > 0)
        {
            rad = atan(v.y/v.x);
        } else if (v.x < 0) {
            rad = atan(v.y/v.x) + M_PI;
        } else if (abs(0 - v.x) < 0.1 && v.y > 0)
        {
            rad = M_PI/2;
        } else if (abs(0 - v.x) < 0.1 && v.y < 0)
        {
            rad = -M_PI/2;
        }
        // std::cout << std::setw(20) << std::left << "cos(rad) sin(rad)" << std::setw(20) << std::left <<  cos(rad)*hyp*dt.asSeconds() << std::setw(20) <<  sin(rad)*hyp*dt.asSeconds() << '\n';
        // std::cout << std::setw(20) << std::left <<   "hyp rad" << std::setw(20) << std::left <<   hyp << std::setw(20) <<  rad << '\n';
        // std::cout << std::setw(20) << std::left <<   "v.x v.y" << std::setw(20) << std::left <<   v.x << std::setw(20) <<  v.y << "\n\n";
        if (abs(0 - hyp) < near0)
        {
            //std::cout << std::setw(20) << hyp << "!\n";
            hyp = 0;
            v.y = 0;
            v.x = 0;
        } else
        {
            hyp -= drg;
            v.y = sin(rad)*hyp;
            v.x = cos(rad)*hyp;
            circle.move(v.x*dt.asSeconds(),v.y*dt.asSeconds());
        }

        //movement
        //origin is top-left. imagine window as 1st cartesian quadrant flipped on x-axis
        //up
        if (Keyboard::isKeyPressed(Keyboard::Key::Up))
        {//while holding, accelerate
            v.y -= accUp;
            circle.move(0,v.y*dt.asSeconds());
        }
        //down
        else if (Keyboard::isKeyPressed(Keyboard::Key::Down))
        {
            v.y += accDown;
            circle.move(0,v.y*dt.asSeconds());
        }
        //left
        if (Keyboard::isKeyPressed(Keyboard::Key::Left))
        {
            v.x -= accLeft;
            circle.move(v.x*dt.asSeconds(),0);
        }
        //right
        else if (Keyboard::isKeyPressed(Keyboard::Key::Right))
        {
            v.x += accRight;
            circle.move(v.x*dt.asSeconds(),0);
        }

        //effect of gravity (comment/uncomment to toggle)
        v.y += g;

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

        //prints polar coords
        //std::cout << '(' << hyp << ", " << rad << ")\n";

        window.clear(Color::Blue);
        window.draw(circle);
        window.display();
    }
    return 0;
}

//function implementations
/*
bool drag(Vector2f &v, float &hyp, float &rad, float &drg,
    float &near0, bool playing, Time &dt, CircleShape &circle)
{
    if ((hyp < near0 && hyp > -near0) || Keyboard::isKeyPressed(Keyboard::Key::Down))
    {//near zero v OR opp key pressed -> stop this control
        playing = false;
    }
    else if (hyp >= near0 || hyp <= -near0)
    {//decelerate
        hyp -= drg;
        circle.move(cos(rad)*hyp*dt.asSeconds(),sin(rad)*hyp*dt.asSeconds());
    }
    std::cout << playing << "\n";
    return playing;
}
*/
void setRad(Vector2f &v, float &rad, float &hyp)
{
    //formatting rad like this so that it is unique in each quadrant
    if (v.x > 0 && v.y == 0)
    {
        rad = 0;
    } else if (v.x > 0 && v.y > 0) //1st quadrant (0 to Pi/2)
    {
        rad = asin(v.y/hyp);
    } else if (v.x == 0 && v.y > 0) //Pi/2
    {
        rad = M_PI/2;
    } else if (v.x < 0 && v.y > 0) //2nd quadrant (Pi/2 to Pi)
    {
        rad = asin(v.y/hyp) + M_PI/2;
    } else if (v.x < 0 && v.y == 0) //Pi
    {
        rad = M_PI;
    } else if (v.x < 0 && v.y < 0) //3rd quadrant (Pi to 3*Pi/2)
    {
        rad = asin(abs(v.y)/hyp) + M_PI;
    } else if (v.x == 0 && v.y < 0) //3Pi/2
    {
        rad = 3*M_PI/2;
    } else if (v.x > 0 && v.y < 0) //4th quadrant (3Pi/2 to 0)
    {
        rad = asin(abs(v.y)/hyp) + 3*M_PI/2;
    }
}
