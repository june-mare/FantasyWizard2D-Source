#include "GLTextFont.h"


bool GLText::Initialize(std::string fileName,int size)
{
	TTF_Init();
	font = TTF_OpenFont(fileName.c_str(), size);
	
	return font;
}

void GLText::EventCall(std::string text, GLenum filterMode)
{	
	image = TTF_RenderUTF8_Blended(font, text.c_str(), white);
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	
	int n_w = 1;
	int n_h = 1;

	double l2_w = log10(image->w) / log10(2);
	double l2_h = log10(image->h) / log10(2);

	if (l2_w != floor(l2_w)) { n_w = 1 << (int)ceil(l2_w); }
	if (l2_h != floor(l2_h)) { n_h = 1 << (int)ceil(l2_h); }

	SDL_Surface* newImage = SDL_CreateRGBSurface(0, n_w, n_h, 8, 0xff000000, 0x00ff0000, 0x0000ff00, 0xff000);
	SDL_BlitSurface(image,0,newImage,0);


	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, n_w, n_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	
	glTextureStorage2D(tex, 1, GL_RGBA8, n_w, n_h);
	glTextureSubImage2D(tex, 0, 0, 0, n_w, n_h,
		GL_BGRA, GL_UNSIGNED_BYTE, image->pixels);

}

void GLText::Terminate()
{
	SDL_FreeSurface(image);
	TTF_CloseFont(font);
	TTF_Quit();

}

GLuint GLText::GetTex() noexcept
{
	return tex;
}

