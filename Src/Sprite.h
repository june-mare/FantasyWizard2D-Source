#pragma once
#ifndef SPRITE_H_INCLUDE
#define SPRITE_H_INCLUDE
#include <vector>
#include <string>
#define IMG_OBJECTS_SIZE 1088
/*�}�`�ɕ\������͈͂�\���\����*/
	/*���l�̓e�L�X�`�����W�n�ł��Ă�����*/
enum class side
{
	non = 0,
	down = 3,
	left = 6,
	right = 9,
	up = 12,
};

struct Texcoord
{
	
	float u, v;	//�e�L�X�`�����W
	float sx, sy;//�摜�̑傫��	
	bool operator==(const Texcoord& a)const
	{
		return u == a.u && v == a.v && sx == a.sx && sy == a.sy;
	}
};

/*�摜�\���p�f�[�^�i�X�v���C�g�j���i�[����\����*/
struct Sprite
{
	float x, y, z, w; /*�摜��\��������W*/
	Texcoord texcoord;	//�e�L�X�`�����W�Ƒ傫��
	float angle = 1.0f / 180 / 3.14;
	float scale = 1.0f;
	float scaleX = 1.0f, scaleY = 1.0f;
	float alpha = 1.0f;
	float damy;
	
	//std::string name;
};

/*�X�v���C�g�z��^*/
using SpriteList = std::vector<Sprite>;

/*�e�N�X�`�����W���쐬����(���W�̓s�N�Z�����Ŏw��)*/
inline Texcoord MakeTexcoord(float u, float v, float sx, float sy) {
	return Texcoord{ u / 512, v / 512, sx / 512, sy / 512 };
};
inline Texcoord MakeTexcoord(float u, float v, float sx, float sy,float sizex,float sizey) {
	return Texcoord{ u / sizex, v / sizey, sx / sizex, sy / sizey };
};

#endif // !SPRITE_H_INCLUDE
