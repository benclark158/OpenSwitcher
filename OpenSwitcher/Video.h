
#pragma once
#ifndef VIDEO_H
#define VIDEO_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

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

class Video
{

private:
    //Variables
    sf::Texture       m_Texture;
    bool            m_bVideoLoaded;
    bool            m_bImageNeedsUpdate;
    AVFormatContext* m_pFormatCtx;
    sf::Uint8       m_iVideoStream;
    sf::Uint32      m_iFrameSize;
    AVCodecContext* m_pCodecCtx;
    AVFrame* m_pFrame;
    AVFrame* m_pFrameRGB;
    sf::Uint8* m_pBuffer;
    AVPacket        m_Packet;
    AVCodec* m_pCodec;
    SwsContext* img_convert_ctx;
    string          m_sFilename;
    float           m_fSecondsPerFrame;
    float           m_fTimePassedSinceLastFrameUpdate;

    //Functions
    void UpdateImage();
    void LoadNextFrame();
public:
    Video();
    explicit Video(const string& filename = "");
    bool LoadFromFile(const string& filename);
    void Update(float time);

    sf::Vector2i Size() const { return sf::Vector2i(GetWidth(), GetHeight()); }

    nuint GetWidth() const { return m_pCodecCtx->width; }
    nuint GetHeight() const { return m_pCodecCtx->height; }

    float GetFrameRate() const { return 1 / m_fSecondsPerFrame; }

    operator const sf::Texture& () const { return m_Texture; }

    ~Video();

    sf::Color GetPixel(nuint x, nuint y) const;

    void CloseVideo();

private:
    Video(const Video& rhs);
    Video& operator=(const Video& rhs);
};

#endif // VIDEO_H
