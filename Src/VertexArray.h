#pragma once
#ifndef VERTEXARRAY_H_INCLUDE
#define VERTEXARRAY_H_INCLUDE
#include "glad/glad.h"
#include <memory>

//æséŒ¾
class VertexArray;
using VertexArrayPtr = std::shared_ptr<VertexArray>;

/*vao‚ğŠÇ—‚·‚éƒNƒ‰ƒX*/
class VertexArray
{
	GLuint vao = 0;
public:
	VertexArray() { glCreateVertexArrays(1, &vao); }
	~VertexArray() { glDeleteVertexArrays(1, &vao); }

	static VertexArrayPtr Create() { return std::make_shared<VertexArray>();}

	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	//ŠÇ—”Ô†‚ğæ“¾
	operator GLuint()const { return vao; }
};

#endif // !VERTEXARRAY_H_INCLUDE
