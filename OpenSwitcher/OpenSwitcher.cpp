

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "MediaPlayer.h"

#include <iostream>
#include "Webcam.h"


extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavdevice/avdevice.h>
    #include <libavfilter/avfilter.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libavfilter/buffersink.h>
    #include <libavfilter/buffersrc.h>
#include "Mixer.h"
}

void registerAll() {
    av_register_all();
    avcodec_register_all();
    avdevice_register_all();
    avfilter_register_all();
}

int framerate = 24;

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
    MediaPlayer media("Y:\\4 - Final Videos\\UoNTrampoline.mp4");
    MediaPlayer media2("Y:\\4 - Final Videos\\Year in Review 2019 v8 FINAL.mp4");
    //sf::Sprite sprite(media);

    //webcam
    //Webcam webcam("test");
    //webcam.showDshowDevice();

    //webcam.init();
    //sf::Sprite camSprite(webcam);
    
    //Not used yet!
    //sprite.setScale(0.5, 0.5);
    //sprite.setPosition(1920 / 2, 0);

    Mixer mixer(8);
    mixer.setInput(0, &media);
    mixer.setInput(1, &media2);
    mixer.setInput(2, &media);
    mixer.setInput(3, &media2);
    mixer.setInput(4, &media);
    mixer.setInput(5, &media2);
    mixer.setInput(6, &media);
    mixer.setInput(7, &media2);
    mixer.convertInputsSprite();

    sf::Clock clock;

    mixer.updateInputs(100);// this has poor performance! Threads?

    while (true)
    {   
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();

        //updates media player
        //media.Update(1.0 / framerate);
        //webcam.update(1);

        //std::cout << "Time fps: " << 1.0 / framerate << " Time Elapsed: " << time << std::endl;

        //mixer.updateInputs(1.0 / framerate);
        //mixer.updateInputs(time);


        // clear the buffers
        window.clear();
        glClearColor(1.0, 1.0, 1.0, 0.0);

        //draws media player sprite!
        //window.draw(sprite);
        //window.draw(camSprite);

        mixer.drawMainMultiview(&window);

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