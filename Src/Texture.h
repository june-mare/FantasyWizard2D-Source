#pragma once
#ifndef TEXTURE_H_INCLUDE
#define TEXTURE_H_INCLUDE
#include "glad/glad.h"
#include <string>
#include <memory>

//先行宣言
class Texture;
using TexturePtr = std::shared_ptr<Texture>;

class Texture
	:public std::enable_shared_from_this<Texture>
{
	GLuint tex = 0;
	std::string filename;
public:
	uint8_t header[18];
	GLuint width, height;
	static TexturePtr Create(
		const std::string& filename, GLenum filterMode = GL_NEAREST);

	Texture(const std::string& filename, GLenum filterMode = GL_NEAREST);
	~Texture();
	inline const std::string GetName()noexcept { return filename; }
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	//管理番号の取得
	operator GLuint()const { return tex; }

};

#endif // !TEXTURE_H_INCLUDE
