#include "Test3D.h"
#include "MyMath.h"


GLuint CreateBuffer(GLsizeiptr size, const GLvoid* data)
{
	GLuint id = 0;
	glCreateBuffers(1, &id);
	glNamedBufferStorage(id, size, data, 0);
	return id;
}

/**
* Vertex Array Object���쐬����.
*
* @param vboPosition VAO�Ɋ֘A�t��������W�f�[�^.
* @param vboColor    VAO�Ɋ֘A�t������J���[�f�[�^.
*
* @return �쐬����VAO.
*/
GLuint CreateVertexArray(GLuint vboPosition, GLuint vboColor)
{
	GLuint id = 0;
	glCreateVertexArrays(1, &id);

	return id;
}