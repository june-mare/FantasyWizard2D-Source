#pragma once
#ifndef GLCONTEXT_H_INCLUDED
#define GLCONTEXT_H_INCLUDED
#include <glad/glad.h>

/*�O�������W�^*/
struct StPosition
{
	float x, y, z;
};

/*�F��*/
struct StColor
{
	float r, g, b, a;
};

namespace GLContext
{
	GLuint CreateBuffer(GLsizeiptr size, const GLvoid* data);

	/*���_�A�g���r���[�g*/
	/*
	�ԍ� �T�C�Y �^     ���蓖�Ĕԍ�|	�ԍ�   �o�b�t�@ID�@�@�X�g���C�h
	0    3      float  0           |	0      ���W�o�b�t�@
	1    4      float  1           |	1�@�@�@�J���[
	2    3      float  2		   |	2�@�@�@�@���o�b�t�@��
	3    2      float  2		   |	�@�@�@ �e�N�X�`�����W�o�b�t�@
	*/
	/*
	VP�o�[�e�b�N�X�p�C�v���C���ɂ́A16�̃f�[�^�̓����������
	���_�A�g���r���[�g�͂��̓�����ɑ���f�[�^���w��ł���
	*/
	GLuint CreateVertexArray(GLuint cboPosition, GLuint vboColor);

	GLuint CreateProgram(GLenum type, const GLchar* code);
}

#endif