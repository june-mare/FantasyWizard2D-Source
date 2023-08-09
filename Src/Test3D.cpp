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
* Vertex Array Objectを作成する.
*
* @param vboPosition VAOに関連付けられる座標データ.
* @param vboColor    VAOに関連付けられるカラーデータ.
*
* @return 作成したVAO.
*/
GLuint CreateVertexArray(GLuint vboPosition, GLuint vboColor)
{
	GLuint id = 0;
	glCreateVertexArrays(1, &id);

	return id;
}