#pragma once
#ifndef GLCONTEXT_H_INCLUDED
#define GLCONTEXT_H_INCLUDED
#include <glad/glad.h>

/*三次元座標型*/
struct StPosition
{
	float x, y, z;
};

/*色彩*/
struct StColor
{
	float r, g, b, a;
};

namespace GLContext
{
	GLuint CreateBuffer(GLsizeiptr size, const GLvoid* data);

	/*頂点アトリビュート*/
	/*
	番号 サイズ 型     割り当て番号|	番号   バッファID　　ストライド
	0    3      float  0           |	0      座標バッファ
	1    4      float  1           |	1　　　カラー
	2    3      float  2		   |	2　　　法線バッファ＆
	3    2      float  2		   |	　　　 テクスチャ座標バッファ
	*/
	/*
	VPバーテックスパイプラインには、16個のデータの入り口があり
	頂点アトリビュートはこの入り口に送るデータを指定できる
	*/
	GLuint CreateVertexArray(GLuint cboPosition, GLuint vboColor);

	GLuint CreateProgram(GLenum type, const GLchar* code);
}

#endif