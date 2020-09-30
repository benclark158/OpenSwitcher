

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "MediaPlayer.h"

#include <iostream>

/*
void draw() {
    //Enables alpha channels
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_POLYGON);
    glColor4f(1.0, 0.0, 0.0, 0.5);
    float z = 0.0;
    glVertex3f(-25.0, -25.0, z);
    glVertex3f(25.0, -25.0, z);
    glVertex3f(25.0, 25.0, z);
    glVertex3f(-25.0, 25.0, z);
    glEnd();
}
*/

int framerate = 25;

int main()
{
    std::cout << "Starting switcher" << std::endl;
    
    
    // create the window
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "OpenGL", sf::Style::None);
    window.setFramerateLimit(framerate); //Limits frame rate but has tearing
    window.setVerticalSyncEnabled(true); //forces monitor refresh rate (60fps) no tearing!

    // activate the window
    window.setActive(true);

    window.setPosition(sf::Vector2i(-1920, 0));
    //window.setPosition(sf::Vector2i(0, 0));
    window.setMouseCursorVisible(true);
    window.setMouseCursorGrabbed(false);

    int frame_counter = 0;

    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(1920, 0));

    sf::Clock clock;

    int frame = 0;

    //Video viddy("C:\\Users\\Ben Clark\\Videos\\2020-09-30 20-30-07.mp4");
    MediaPlayer media("H:\\IN-OUTS\\ScreenStillImage.png");
    sf::Sprite sprite(media);

    int w = media.GetWidth();
    int h = media.GetHeight();

    sprite.setScale(0.5, 0.5);
    sprite.setPosition(1920 / 2, 0);

    while (true)
    {

        media.Update(1.0 / framerate);

        // clear the buffers
        window.clear();
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*
        if (frame % 2 == 0) {
            glClearColor(1.0, 0.0, 0.0, 0.0);
        }
        else {
            //glClearColor(0.0, 1.0, 0.0, 0.0);
        }
        frame++;*/

        window.draw(sprite);

        window.display();

        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                return 0;
            }
        }
    }
    
    return 0;
}