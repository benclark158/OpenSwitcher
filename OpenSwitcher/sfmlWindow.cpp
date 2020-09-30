// g++ TEST_TEST_TEST.cpp -lsfml-system -lsfml-window -lsfml-graphics -lGL
/*
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <GLFW/glfw3.h>
#include <iostream>


void draw() {
    glClearColor(0.0, 0.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int window_width, window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, window_width, window_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_POLYGON);
    glColor4f(1.0, 0.0, 0.0, 0.5);
    float z = 0.0;
    glVertex3f(-25.0, -25.0, z);
    glVertex3f(25.0, -25.0, z);
    glVertex3f(25.0, 25.0, z);
    glVertex3f(-25.0, 25.0, z);
    glEnd();

    //glfwSwapBuffers(window);
    //Sleep(1);
}

int main()
{

    // create the window
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "OpenGL", sf::Style::None);
    window.setFramerateLimit(25); //Limits frame rate but has tearing
    //window.setVerticalSyncEnabled(true); //forces monitor refresh rate (60fps) no tearing!

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

    while (true)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                return 0;
            }
        }

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (frame % 2 == 0) {
            glClearColor(1.0, 0.0, 0.0, 0.0);
        }
        else {
            //glClearColor(0.0, 1.0, 0.0, 0.0);
        }
        frame++;

        draw();

        /*
        sf::Texture backgroundTexture;
        backgroundTexture.setSrgb(false);
        if (!backgroundTexture.loadFromFile("resources/background.jpg"))
            return EXIT_FAILURE;
        sf::Sprite background(backgroundTexture);
        */
        /*
        sf::Font font;
        font.loadFromFile("arial.ttf");

        sf::Text text;
        text.setString("Hello");
        text.setFont(font);
        text.setCharacterSize(200);
        text.setFillColor(sf::Color::Red);
        text.setPosition(80, 50);

        window.draw(text);*/

        // end the current frame (internally swaps the front and back buffers)
/*
        window.display();
    }

    return 0;
}*/