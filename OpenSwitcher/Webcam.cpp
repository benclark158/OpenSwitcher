#include "Webcam.h"

Webcam::Webcam() {

}

Webcam::Webcam(const string& deviceName, const int& framerate){
    this->devName = deviceName;
    this->framerate = framerate;
}

void Webcam::init() {
    //const char* filenameSrc = "video=FULL HD 1080P Webcam";
    const char* filenameSrc = "video=FHD Capture";

    AVCodecContext* pCodecCtx; //codec context
    AVFormatContext* pFormatCtx = avformat_alloc_context(); //format context
    AVCodec* pCodec; //codec
    AVInputFormat* iformat = av_find_input_format("dshow"); //input format = directshow
    AVFrame* pFrame, * pFrameRGB; //actual frame

    AVDictionary* options = NULL;

    //av_dict_set(&options, "framerate", std::to_string(framerate).c_str(), 0);
    av_dict_set(&options, "framerate", "30", 0);

    pAVFormatContext = NULL;

    // check video source
    if (avformat_open_input(&pAVFormatContext, filenameSrc, iformat, NULL) != 0)
    {
        cout << "\nOops, could'nt open video source\n\n";
        return;
    }
    else
    {
        cout << "\n Success Webcam '" << filenameSrc << "' opened!" << endl;

        av_dump_format(pAVFormatContext, 0, filenameSrc, 0);

        inputVideoStream = -1;
        for (nuint i = 0; i < pAVFormatContext->nb_streams; i++)
        {
            if (pAVFormatContext->streams[i]->codec->coder_type == AVMEDIA_TYPE_VIDEO)
            {
                inputVideoStream = i;
                break;
            }
        }

        if (inputVideoStream == -1) {
            cout << "No video stream" << endl;
            return;
        }

        m_pCodecCtx = pAVFormatContext->streams[inputVideoStream]->codec;

        AVCodec* m_pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);

        //AVRational r = pAVFormatContext->streams[inputVideoStream]->avg_frame_rate;
        //AVRational r2 = pAVFormatContext->streams[inputVideoStream]->r_frame_rate;

        if (m_pCodec == NULL)
        {
            cout << "Unsupported codec!" << endl;
            return;
        }

        if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0)
        {
            cout << "Could not open Codec Context" << endl;
            return;
        }

        sf::Uint32 m_iFrameSize = m_pCodecCtx->width * m_pCodecCtx->height * 3;

        m_pFrame = av_frame_alloc();
        m_pFrameRGB = av_frame_alloc();

        if (m_pFrameRGB == NULL || m_pFrame == NULL)
        {
            cout << "Error allocating frame" << endl;
            return;
        }

        int numBytes = avpicture_get_size(AV_PIX_FMT_RGBA, m_pCodecCtx->width, m_pCodecCtx->height);

        m_pBuffer = (sf::Uint8*)av_malloc(numBytes * sizeof(sf::Uint8));

        avpicture_fill((AVPicture*)m_pFrameRGB, m_pBuffer, AV_PIX_FMT_RGBA,
            m_pCodecCtx->width, m_pCodecCtx->height);

        bool m_bVideoLoaded = true;
        bool m_bImageNeedsUpdate = true;

        m_Texture.create(GetWidth(), GetHeight());

        img_convert_ctx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height,
            m_pCodecCtx->pix_fmt,
            m_pCodecCtx->width, m_pCodecCtx->height,
            AV_PIX_FMT_RGBA, SWS_FAST_BILINEAR,
            NULL, NULL, NULL);
    }
}

void Webcam::capture() {

    
    return;

}

void Webcam::update(float time) {
    //capture();
    int frameFinished = 0;
    avcodec_decode_video2(m_pCodecCtx, m_pFrame, &frameFinished, &m_Packet);
    if (frameFinished)
    {
        sws_scale(img_convert_ctx, m_pFrame->data, m_pFrame->linesize,
            0, m_pCodecCtx->height, m_pFrameRGB->data, m_pFrameRGB->linesize);
        m_bImageNeedsUpdate = true;
    }
}


sf::Color Webcam::GetPixel(nuint x, nuint y) const
{
    nuint i = 3 * (y * GetWidth() + x);
    sf::Uint8 red = m_pFrameRGB->data[0][i];
    sf::Uint8 green = m_pFrameRGB->data[0][i + 1];
    sf::Uint8 blue = m_pFrameRGB->data[0][i + 2];

    return sf::Color(red, green, blue, 255);
}

Webcam::~Webcam()
{
}
