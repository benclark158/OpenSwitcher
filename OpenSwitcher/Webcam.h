#pragma once
#ifndef WEBCAM_H
#define WEBCAM_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;

typedef unsigned int nuint;

#define __STDC_CONSTANT_MACROS

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

class Webcam {

private:
	sf::Texture       m_Texture;

	Webcam(const Webcam& rhs);
	Webcam& operator=(const Webcam& rhs);

	void loadNextFrame();
public:
	Webcam();
	explicit Webcam(const string& name = "", const int& framerate = 25);

	void capture();
	void init();
	void update(int i);

	sf::Vector2i Size() const { return sf::Vector2i(GetWidth(), GetHeight()); }

	nuint GetWidth() const { return m_pCodecCtx->width; }
	nuint GetHeight() const { return m_pCodecCtx->height; }

	operator const sf::Texture& () const { return m_Texture; }

	sf::Color GetPixel(nuint x, nuint y) const;

	void showDshowDevice() {
		AVFormatContext* pFormatCtx = avformat_alloc_context();
		AVDictionary* options = NULL;
		av_dict_set(&options, "list_devices", "true", 0);
		AVInputFormat* iformat = av_find_input_format("dshow");
		printf("========Device Info=============\n");
		avformat_open_input(&pFormatCtx, "video=dummy", iformat, &options);
		printf("================================\n");
	}

	~Webcam();

	//Variables
	string devName;
	int framerate;
	AVCodecContext* m_pCodecCtx;
	AVFrame* m_pFrame;
	AVFrame* m_pFrameRGB;
	AVPacket        m_Packet;
	AVFormatContext* pAVFormatContext;
	nuint inputVideoStream;
	bool m_bImageNeedsUpdate;
	sf::Uint8* m_pBuffer;
	SwsContext* img_convert_ctx;
};

#endif // WEBCAM_H
