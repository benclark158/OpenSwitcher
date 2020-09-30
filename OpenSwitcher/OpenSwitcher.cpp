

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "MediaPlayer.h"

#include <iostream>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavdevice/avdevice.h>
    #include <libavfilter/avfilter.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libavfilter/buffersink.h>
    #include <libavfilter/buffersrc.h>
#include "Webcam.h"
}

void registerAll() {
    av_register_all();
    avcodec_register_all();
    avdevice_register_all();
    avfilter_register_all();
}

int framerate = 25;

int main()
{
    std::cout << "Starting switcher" << std::endl;
    
    registerAll();
    
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

    //hidden atm
    MediaPlayer media("H:\RAW_VIDEOS\\7. Matt.MP4");
    sf::Sprite sprite(media);

    //webcam
    Webcam webcam("test");
    webcam.showDshowDevice();

    webcam.init();
    sf::Sprite camSprite(webcam);
    
    //Not used yet!
    //sprite.setScale(0.5, 0.5);
    //sprite.setPosition(1920 / 2, 0);

    while (true)
    {   
        //updates media player
        media.Update(1.0 / framerate);
        webcam.update(1);


        // clear the buffers
        window.clear();
        glClearColor(0.0, 0.0, 0.0, 0.0);

        //draws media player sprite!
        window.draw(sprite);
        window.draw(camSprite);

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