
#pragma once
#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Input.h"

using namespace std;

typedef unsigned int nuint;

#define __STDC_CONSTANT_MACROS

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <inttypes.h>
}

class MediaPlayer : public Input
{

private:
    //Variables
    
    bool            m_bVideoLoaded;
    bool            m_bImageNeedsUpdate;
    AVFormatContext* m_pFormatCtx;
    sf::Uint8       m_iVideoStream;
    sf::Uint32      m_iFrameSize;
    AVFrame* m_pFrame;
    AVFrame* m_pFrameRGB;
    sf::Uint8* m_pBuffer;
    AVPacket        m_Packet;
    AVCodec* m_pCodec;
    SwsContext* img_convert_ctx;
    std::string          m_sFilename;
    float           m_fSecondsPerFrame;
    float           m_fTimePassedSinceLastFrameUpdate;

    //Functions
    void UpdateImage();
    void LoadNextFrame();
public:
    MediaPlayer();
    explicit MediaPlayer(const string& filename = "");
    ~MediaPlayer();

    void update(float time);

    bool LoadFromFile(const string& filename);

    float GetFrameRate() const { return 1 / m_fSecondsPerFrame; }

    void CloseVideo();

public:
    MediaPlayer(const MediaPlayer& rhs);
    MediaPlayer& operator=(const MediaPlayer& rhs);
};

#endif // MEDIA_PLAYER_H
