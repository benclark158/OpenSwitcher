#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Input.h"
#include <stdio.h>
#include <iostream>

extern "C"
{
	#include <libavcodec\avcodec.h>
}


class Mixer {
	
public:
	Mixer(int nInputs) {
		if (nInputs < 1) {
			std::cout << "Invalid number of inputs" << std::endl;
		}

		numberInputs = nInputs;
		inputs = new Input * [nInputs];
		sprites = new sf::RectangleShape[nInputs];
	}

	int numberInputs = -1;
	Input** inputs;
	int program = 1;
	int preview = 0;

	sf::RectangleShape* sprites;
	sf::RectangleShape programSprite;
	sf::RectangleShape previewSprite;

	void convertInputsSprite() {
		if (numberInputs > 0) {
			for (int y = 0; y < 2; y++) {
				for (int x = 0; x < 4; x++) {
					int i = (y * 4) + x;
					if (i < numberInputs && inputs[i] != NULL) {
						sf::RectangleShape sprite(sf::Vector2f(1920, 1080));

						sprite.setTexture(*inputs[i]);
						sprite.setScale(0.246, 0.246);

						int xPos = (x * 473) + ((x + 1) * 5) + 1;
						int yPos = 538 + (y * 271);
						sprite.setPosition(xPos, yPos);

						sprite.setOutlineThickness(12);
						sprite.setOutlineColor(sf::Color::White);

						sprites[i] = sprite;
					}
				}
			}

			programSprite.setSize(sf::Vector2f(1920, 1080));
			programSprite.setTexture(*inputs[program]);
			programSprite.setScale(0.48, 0.48);
			programSprite.setPosition(994, 5);
			programSprite.setOutlineThickness(0);


			previewSprite.setSize(sf::Vector2f(1920, 1080));
			previewSprite.setTexture(*inputs[preview]);
			previewSprite.setScale(0.48, 0.48);
			previewSprite.setPosition(5, 5);
			previewSprite.setOutlineThickness(0);
		}
	}

	void drawMainMultiview(sf::RenderWindow* window) {
		if (numberInputs > 0) {
			for (int i = 0; i < numberInputs; i++) {
				if (i == program) {
					sprites[i].setOutlineColor(sf::Color::Red);
				}
				else if (i == preview) {
					sprites[i].setOutlineColor(sf::Color::Green);
				}
				else {
					sprites[i].setOutlineColor(sf::Color::Transparent);
				}

				(*window).draw(sprites[i]);
			}

			(*window).draw(programSprite);
			(*window).draw(previewSprite);
		}
	}


	void updateInputs(float t) {
		if (numberInputs > 0) {
			for (int i = 0; i < numberInputs; i++) {
				(*inputs[i]).update(t);
			}
		}
	}

	//void draw16InputMultiview();
	
	void setInput(int id, Input* in) {
		if (numberInputs > 0 && id < numberInputs && id >= 0) {
			inputs[id] = in;
		}
		else {
			std::cout << "Mixer is not configured." << std::endl;
		}
	}

	void setProgram(int id) {
		if (id < numberInputs && id >= 0) {
			program = id;
		}
		else {
			std::cout << "Unsupported input ID: " << id << std::endl;
		}
	}

	void setPreview(int id) {
		if (id < numberInputs && id >= 0) {
			preview = id;
		}
		else {
			std::cout << "Unsupported input ID: " << id << std::endl;
		}
	}
private:
};