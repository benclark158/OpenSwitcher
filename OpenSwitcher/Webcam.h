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
}

class Webcam {

private:
	sf::Texture       m_Texture;

	Webcam(const Webcam& rhs);
	Webcam& operator=(const Webcam& rhs);
public:
	Webcam();
	explicit Webcam(const string& name = "");

	//sf::Vector2i Size() const { return sf::Vector2i(GetWidth(), GetHeight()); }

	//nuint GetWidth() const { return m_pCodecCtx->width; }
	//nuint GetHeight() const { return m_pCodecCtx->height; }

	//operator const sf::Texture& () const { return m_Texture; }

	//sf::Color GetPixel(nuint x, nuint y) const;

	void showDshowDevice() {
		AVFormatContext* pFormatCtx = avformat_alloc_context();
		AVDictionary* options = NULL;
		av_dict_set(&options, "list_devices", "true", 0);
		AVInputFormat* iformat = av_find_input_format("dshow");
		printf("========Device Info=============\n");
		avformat_open_input(&pFormatCtx, "video=dummy", iformat, &options);
		printf("================================\n");

		//FULL HD 1080P Webcam
	}

	~Webcam();
};

#endif // WEBCAM_H
