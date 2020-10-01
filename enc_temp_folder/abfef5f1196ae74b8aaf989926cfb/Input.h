#pragma once

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavdevice/avdevice.h>
    #include <libavfilter/avfilter.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libavfilter/buffersink.h>
    #include <libavfilter/buffersrc.h>
    #include <libavcodec/avcodec.h> 
    #include <libswscale/swscale.h>
    #include <libavformat/avformat.h>
    #include <libavutil/imgutils.h>
}

using namespace std;

typedef unsigned int nuint;

class Input {
public:
    Input() : m_Texture(), m_pCodecCtx(NULL) {};
    ~Input() {};

    sf::Texture       m_Texture;
    AVCodecContext* m_pCodecCtx;

    operator const sf::Texture& () const { return m_Texture; }
    operator const sf::Texture* () const { return &m_Texture; }

    sf::Vector2i Size() const { return sf::Vector2i(GetWidth(), GetHeight()); }

    nuint GetWidth() const { return m_pCodecCtx->width; }
    nuint GetHeight() const { return m_pCodecCtx->height; }

    virtual void update(float time) {
        //do nothing
    }
    //sf::Color GetPixel(nuint x, nuint y) const;
private:


};