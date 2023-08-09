#include "GLContext.h"
#include <iostream>

namespace GLContext
{
	/*アドレスを取得して何のバッファでも作れるようにする*/
	GLuint CreateBuffer(GLsizeiptr size, const GLvoid* data)
	{
		GLuint id = 0;
		glCreateBuffers(1, &id);
		glNamedBufferStorage(id, size, data, 0);
		return id;
	}

	/*どのような方法でメモリの読み書きを覚えるか*/
	/*
	GL_DYNAMIC_STORAGE_BIT		glBufferSubData を使って更新することができる。
	GL_MAP_READ_BIT				glMapBufferRange を使って読み取ることができる。
	GL_MAP_WRITE_BIT			glMapBufferRange を使って書き込むことができる。
	GL_MAP_PERSISTENT_BIT		GPU が読み書きを行っている最中でも読み書きできる。
	GL_MAP_COHERENT_BIT			書き込みの結果が次の処理ですぐ利用できれる。
	GL_CLIENT_STORAGE_BIT		OpenGLに読み書き用のCPUメモリの確保を許可する。
	*/

	/*Vertex Array Objectを作成する*/
	GLuint CreateVertexArray(GLuint vboPosition, GLuint vboColor)
	{
		if (!vboPosition || !vboColor)
		{
			std::cerr << "[エラー]" << __func__ << ":バッファオブジェクトが0です\n";
			return 0;
		}
		GLuint id = 0;
		glCreateVertexArrays(1, &id);

		const GLuint positionIndex = 0;
		const GLuint positionBindIndex = 0;
		glEnableVertexArrayAttrib(id, positionIndex);
		glVertexArrayAttribFormat(id, positionIndex, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, positionIndex, positionBindIndex);

		glVertexArrayVertexBuffer(id, positionBindIndex, vboPosition, 0, sizeof(StPosition));

		const GLuint colorIndex = 1;
		const GLuint colorBindIndex = 1;
		glEnableVertexArrayAttrib(id, colorIndex);
		glVertexArrayAttribFormat(id, colorIndex, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(id, colorIndex, colorBindIndex);

		glVertexArrayVertexBuffer(id, colorBindIndex, vboColor, 0, sizeof(StColor));

		return id;
	}
}