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
	SDL_Rect rect, //画像摘出範囲
		scr_rect;//画像配置位置
	//SDL_Event event;
	SDL_Color white = {0,0,0,255};//カラーリング
	TTF_Font* font;//ttfファイル格納	
	GLuint tex;

public :
	SDL_Surface* image;//画像として出すための変数

	GLText() = default;
	virtual ~GLText() = default;

	bool Initialize(std::string fileName, int size);

	void EventCall(std::string text, GLenum filterMode = GL_NEAREST);

	bool SetTTFText(std::string text);
	
	void Terminate();

	GLuint GetTex()noexcept;

};



#endif // !GLTEXT_FONT_H_INCLUDE