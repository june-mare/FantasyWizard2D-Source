#include "GLContext.h"
#include <iostream>

namespace GLContext
{
	/*�A�h���X���擾���ĉ��̃o�b�t�@�ł�����悤�ɂ���*/
	GLuint CreateBuffer(GLsizeiptr size, const GLvoid* data)
	{
		GLuint id = 0;
		glCreateBuffers(1, &id);
		glNamedBufferStorage(id, size, data, 0);
		return id;
	}

	/*�ǂ̂悤�ȕ��@�Ń������̓ǂݏ������o���邩*/
	/*
	GL_DYNAMIC_STORAGE_BIT		glBufferSubData ���g���čX�V���邱�Ƃ��ł���B
	GL_MAP_READ_BIT				glMapBufferRange ���g���ēǂݎ�邱�Ƃ��ł���B
	GL_MAP_WRITE_BIT			glMapBufferRange ���g���ď������ނ��Ƃ��ł���B
	GL_MAP_PERSISTENT_BIT		GPU ���ǂݏ������s���Ă���Œ��ł��ǂݏ����ł���B
	GL_MAP_COHERENT_BIT			�������݂̌��ʂ����̏����ł������p�ł����B
	GL_CLIENT_STORAGE_BIT		OpenGL�ɓǂݏ����p��CPU�������̊m�ۂ�������B
	*/

	/*Vertex Array Object���쐬����*/
	GLuint CreateVertexArray(GLuint vboPosition, GLuint vboColor)
	{
		if (!vboPosition || !vboColor)
		{
			std::cerr << "[�G���[]" << __func__ << ":�o�b�t�@�I�u�W�F�N�g��0�ł�\n";
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