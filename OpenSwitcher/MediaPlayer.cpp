
#include "MediaPlayer.h"

MediaPlayer::MediaPlayer() : 
m_bVideoLoaded(false),
m_bImageNeedsUpdate(false),
m_pFormatCtx(NULL),
m_iVideoStream(0),
m_iFrameSize(0),
m_pFrame(NULL),
m_pFrameRGB(NULL),
m_pBuffer(NULL),
m_Packet(),
m_pCodec(NULL),
img_convert_ctx(NULL),
m_sFilename(""),
m_fSecondsPerFrame(0),
m_fTimePassedSinceLastFrameUpdate(0)
{
    av_register_all();
}


MediaPlayer::MediaPlayer(const string& filename) : 
m_bVideoLoaded(false),
m_bImageNeedsUpdate(false),
m_pFormatCtx(NULL),
m_iVideoStream(0),
m_iFrameSize(0),
m_pFrame(NULL),
m_pFrameRGB(NULL),
m_pBuffer(NULL),
m_Packet(),
m_pCodec(NULL),
img_convert_ctx(NULL),
m_sFilename(""),
m_fSecondsPerFrame(0),
m_fTimePassedSinceLastFrameUpdate(0)
{
    av_register_all();

    if (!filename.empty())
    {
        if (!LoadFromFile(filename))
        {
            cout << "Could not load video!" << endl; //Probably should throw exception.
        }
    }
}

bool MediaPlayer::LoadFromFile(const string& filename)
{
    CloseVideo();
    m_sFilename = filename;
    const char* const file = m_sFilename.c_str();

    if (avformat_open_input(&m_pFormatCtx, file, NULL, NULL) != 0)
    {
        cout << "Unexisting file!" << endl;;
        return false;
    }

    if (avformat_find_stream_info(m_pFormatCtx, NULL) < 0)
    {
        cout << "Couldn't find stream information!" << endl;
        return false;
    }

    av_dump_format(m_pFormatCtx, 0, file, 0);

    m_iVideoStream = -1;
    for (nuint i = 0; i < m_pFormatCtx->nb_streams; i++)
    {
        if (m_pFormatCtx->streams[i]->codec->coder_type == AVMEDIA_TYPE_VIDEO)
        {
            m_iVideoStream = i;
            break;
        }
    }

    if (m_iVideoStream == -1)
        return false;

    m_pCodecCtx = m_pFormatCtx->streams[m_iVideoStream]->codec;

    m_pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);

    AVRational r = m_pFormatCtx->streams[m_iVideoStream]->avg_frame_rate;
    AVRational r2 = m_pFormatCtx->streams[m_iVideoStream]->r_frame_rate;
    if ((!r.num || !r.den) &&
        (!r2.num || !r2.den))
    {
        std::cerr << "Video_video::Initialize() - unable to get the video frame rate. Using 25 fps." << std::endl;
        m_fSecondsPerFrame = 1.f / 25;
    }
    else
    {
        if (r.num && r.den)
            m_fSecondsPerFrame = 1.f / ((float)r.num / r.den);
        else
            m_fSecondsPerFrame = 1.f / ((float)r2.num / r2.den);
    }

    if (m_pCodec == NULL)
    {
        cout << "Unsupported codec!" << endl;
        return false;
    }

    if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0)
    {
        cout << "Could not open Codec Context" << endl;
        return false;
    }

    m_iFrameSize = m_pCodecCtx->width * m_pCodecCtx->height * 3;


    m_pFrame = av_frame_alloc();

    m_pFrameRGB = av_frame_alloc();

    if (m_pFrameRGB == NULL || m_pFrame == NULL)
    {
        cout << "Error allocating frame" << endl;
        return false;
    }

    int numBytes = avpicture_get_size(AV_PIX_FMT_RGBA, m_pCodecCtx->width, m_pCodecCtx->height);

    m_pBuffer = (sf::Uint8*)av_malloc(numBytes * sizeof(sf::Uint8));

    avpicture_fill((AVPicture*)m_pFrameRGB, m_pBuffer, AV_PIX_FMT_RGBA,
        m_pCodecCtx->width, m_pCodecCtx->height);

    img_convert_ctx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height,
        m_pCodecCtx->pix_fmt,
        m_pCodecCtx->width, m_pCodecCtx->height,
        AV_PIX_FMT_RGBA, SWS_FAST_BILINEAR,
        NULL, NULL, NULL);
    m_bVideoLoaded = true;
    m_bImageNeedsUpdate = true;

    m_Texture.create(GetWidth(), GetHeight());

    update(10000);

    return true;
} //Load From File

void MediaPlayer::update(float time)
{
    if (m_bVideoLoaded)
    {
        m_fTimePassedSinceLastFrameUpdate += time;

        if (m_fTimePassedSinceLastFrameUpdate >= m_fSecondsPerFrame)
        {
            m_fTimePassedSinceLastFrameUpdate = 0;
            LoadNextFrame();
        }
        UpdateImage();
    }
}

void MediaPlayer::LoadNextFrame()
{
    do
    {
        av_free_packet(&m_Packet);
        int result = av_read_frame(m_pFormatCtx, &m_Packet);
        if (result < 0)
        {
            CloseVideo();
            LoadFromFile(m_sFilename);
            m_fTimePassedSinceLastFrameUpdate = 0;
            return;
        }
    } while (m_Packet.stream_index != m_iVideoStream);

    int frameFinished = 0;
    avcodec_decode_video2(m_pCodecCtx, m_pFrame, &frameFinished, &m_Packet);
    if (frameFinished)
    {
        sws_scale(img_convert_ctx, m_pFrame->data, m_pFrame->linesize,
            0, m_pCodecCtx->height, m_pFrameRGB->data, m_pFrameRGB->linesize);
        m_bImageNeedsUpdate = true;
    }

}
/*
sf::Color Input::GetPixel(nuint x, nuint y) const
{
    nuint i = 3 * (y * GetWidth() + x);
    sf::Uint8 red = m_pFrameRGB->data[0][i];
    sf::Uint8 green = m_pFrameRGB->data[0][i + 1];
    sf::Uint8 blue = m_pFrameRGB->data[0][i + 2];
    //return sf::Color(red, green, blue, 255);
    return sf::Color(0, 0, 255, 255);
}*/

void MediaPlayer::UpdateImage()
{
    if (m_bImageNeedsUpdate)
    {
        m_Texture.update(m_pBuffer);
        m_bImageNeedsUpdate = false;
    }
}

void MediaPlayer::CloseVideo()
{
    if (m_bVideoLoaded)
    {
        av_free_packet(&m_Packet);
        av_free(m_pBuffer);
        av_free(m_pFrameRGB);
        av_free(m_pFrame);
        avcodec_close(m_pCodecCtx);
        avformat_close_input(&m_pFormatCtx);
        sws_freeContext(img_convert_ctx);

        m_bVideoLoaded = false;
        m_bImageNeedsUpdate = false;
    }
}

MediaPlayer::~MediaPlayer()
{
    CloseVideo();
}
