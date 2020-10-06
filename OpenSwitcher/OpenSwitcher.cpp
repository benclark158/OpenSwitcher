

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

#include "MediaPlayer.h"
#include "Webcam.h"
#include "Mixer.h"
#include "NDI.cpp"

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavdevice/avdevice.h>
    #include <libavfilter/avfilter.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libavfilter/buffersink.h>
    #include <libavfilter/buffersrc.h>
}

Mixer mixer;
bool running;
int framerate = 24;

DWORD WINAPI mediaUpdateLoop(PVOID lpParam)
{   
    MediaPlayer* player = (MediaPlayer*)lpParam;

    sf::Clock clock;
    while (running) {

        float time = clock.getElapsedTime().asSeconds();
        clock.restart();

        //std::cout << "Time elapsed: " << time << endl;
        (*player).update(time);
    }

    std::cout << "Thread ended!" << endl;
    delete player;
    return 1;
}

void registerAll() {
    av_register_all();
    avcodec_register_all();
    avdevice_register_all();
    avfilter_register_all();
}

void openWindow() {
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
    MediaPlayer media1("Y:\\4 - Final Videos\\UoNTrampoline.mp4");
    MediaPlayer media2("Y:\\4 - Final Videos\\Year in Review 2019 v8 FINAL.mp4");
    MediaPlayer media3("Y:\\4 - Final Videos\\Spain 2015.mp4");
    MediaPlayer media4("Y:\\4 - Final Videos\\RobinHood7s V2 12-6-2019.mp4");
    MediaPlayer media5("Y:\\4 - Final Videos\\BAYWATCH 24-09-2018.mp4");
    MediaPlayer media6("Y:\\4 - Final Videos\\HoliOnTheDowns2018 NEW.mp4");
    MediaPlayer media7("Y:\\3 - Archive\\2020\\PromoCovid2020\\10 - OUT\\UniInReviewFINAL-FINAL.mp4");
    MediaPlayer media8("Y:\\4 - Final Videos\\BUCS Big Wednesday 2018 - Fleeting 2018.mp4");
    //sf::Sprite sprite(media);

    //webcam
    //Webcam webcam("test");
    //webcam.showDshowDevice();

    //webcam.init();
    //sf::Sprite camSprite(webcam);

    //Not used yet!
    //sprite.setScale(0.5, 0.5);
    //sprite.setPosition(1920 / 2, 0);

    mixer = Mixer(8);
    mixer.setInput(0, &media1);
    mixer.setInput(1, &media2);
    mixer.setInput(2, &media3);
    mixer.setInput(3, &media4);
    mixer.setInput(4, &media5);
    mixer.setInput(5, &media6);
    mixer.setInput(6, &media7);
    mixer.setInput(7, &media8);
    mixer.convertInputsSprite();

    running = true;

    DWORD tid;
    HANDLE t1 = CreateThread(NULL, 0, mediaUpdateLoop, &media1, 0, &tid);
    HANDLE t2 = CreateThread(NULL, 0, mediaUpdateLoop, &media2, 0, &tid);
    HANDLE t3 = CreateThread(NULL, 0, mediaUpdateLoop, &media3, 0, &tid);
    HANDLE t4 = CreateThread(NULL, 0, mediaUpdateLoop, &media4, 0, &tid);
    HANDLE t5 = CreateThread(NULL, 0, mediaUpdateLoop, &media5, 0, &tid);
    HANDLE t6 = CreateThread(NULL, 0, mediaUpdateLoop, &media6, 0, &tid);
    HANDLE t7 = CreateThread(NULL, 0, mediaUpdateLoop, &media7, 0, &tid);
    HANDLE t8 = CreateThread(NULL, 0, mediaUpdateLoop, &media8, 0, &tid);

    sf::Clock clock;

    while (true)
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();

        //std::cout << "Time elapsed: " << time << endl;
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
                return;
            }
        }
    }
    return;
}

int main()
{
    std::cout << "Starting switcher" << std::endl;
    
    //Registers all ffmpeg devices, filters, av etc
    registerAll();
    std::cout << "Registered AV" << std::endl;
    
    receiveNDI();

    return 0;
}