#pragma once
#ifndef GLTEXT_FONT_H_INCLUDE
#define GLTEXT_FONT_H_INCLUDE
#include "GLFWWindow.h"
#include "GLContext.h"
#include <ft2build.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class GLText
{	
	SDL_Rect rect, //�摜�E�o�͈�
		scr_rect;//�摜�z�u�ʒu
	//SDL_Event event;
	SDL_Color white = {0,0,0,255};//�J���[�����O
	TTF_Font* font;//ttf�t�@�C���i�[	
	GLuint tex;

public :
	SDL_Surface* image;//�摜�Ƃ��ďo�����߂̕ϐ�

	GLText() = default;
	virtual ~GLText() = default;

	bool Initialize(std::string fileName, int size);

	void EventCall(std::string text, GLenum filterMode = GL_NEAREST);

	bool SetTTFText(std::string text);
	
	void Terminate();

	GLuint GetTex()noexcept;

};



#endif // !GLTEXT_FONT_H_INCLUDE