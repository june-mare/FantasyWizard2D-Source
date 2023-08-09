#pragma once
#ifndef CHARACTERLIST_H_INCLUDE
#define CHARACTERLIST_H_INCLUDE
#include "MyMath.h"
#include "Sprite.h"
template <typename T>
std::vector<Texcoord> CharacterTCList()
{
	T charadata;
	std::vector<Texcoord> tcList;
	int setNum = 0;
	tcList.resize(charadata.countY * charadata.countX);
	for (int i = 0; i < charadata.countY; i++)
	{
		for (int j = 0; j < charadata.countX; j++) 
		{
			auto tc = MakeTexcoord(
				charadata.start.x + (charadata.size.x * j),
				charadata.start.y + (charadata.size.y * i),
				charadata.size.x, charadata.size.y,
				IMG_OBJECTS_SIZE, IMG_OBJECTS_SIZE);
			tcList[setNum] = tc;
			setNum++;
		}
	}
	return tcList;	
}
#pragma region Chara
//�s���l
class Chara1
{
public:
	MyMath::vec2 start = { 96,0 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4,countX = 3;
};

//�X�q���Ԃ��Ă�N
class Chara2	
{
public:
	MyMath::vec2 start = { 96,512 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};

//�����̌Z�����
class Chara3
{
public:
	MyMath::vec2 start = { 96,640 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};

//�I�I�l
class Chara4
{
public:
	MyMath::vec2 start = { 0,640 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//�����������
class Chara5
{
public:
	MyMath::vec2 start = { 0,768 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//���΂������
class Chara6
{
public:
	MyMath::vec2 start = { 96,768 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//���̂���
class Chara7
{
public:
	MyMath::vec2 start = { 0,896 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//���̂��o����
class Chara8
{
public:
	MyMath::vec2 start = { 192,640 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};

//�������q
class Chara9
{
public:
	MyMath::vec2 start = { 192,768 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};

#pragma endregion
#pragma region Enemy
//���mA
class Enemy1
{
public:
	MyMath::vec2 start = { 192,0 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//������
class Enemy2
{
public:
	MyMath::vec2 start = { 288,0 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//�u���b�N�X�P���g��
class Enemy3
{
public:
	MyMath::vec2 start = { 288,128 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//�܂����낭�낷��
class Enemy4
{
public:
	MyMath::vec2 start = { 288,384 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//�܂���
class Enemy5
{
public:
	MyMath::vec2 start = { 288,512 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//�S�[�X�g
class Enemy6
{
public:
	MyMath::vec2 start = { 0,512 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//���mB
class Enemy7
{
public:
	MyMath::vec2 start = { 192,512 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//�S�u�����E�B�U�[�h
class Enemy8
{
public:
	MyMath::vec2 start = { 96,896 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//�S�u����
class Enemy9
{
public:
	MyMath::vec2 start = { 192,896 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//�G�C���A��	
class Enemy10
{
public:
	MyMath::vec2 start = { 288,256 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
#pragma endregion
#pragma region Boss
//�G�C���A��
class Boss1
{
public:
	MyMath::vec2 start = { 0,128 };
	MyMath::vec2 size = { 96,96 };
	int countY = 1, countX = 3;
};
//�����m
class Boss2
{
public:
	MyMath::vec2 start = { 0,224 };
	MyMath::vec2 size = { 96,96 };
	int countY = 1, countX = 3;
};
//�܂����낭�낷��
class Boss3
{
public:
	MyMath::vec2 start = { 0,320 };
	MyMath::vec2 size = { 96,96 };
	int countY = 1, countX = 3;
};
//��
class Boss4
{
public:
	MyMath::vec2 start = { 0,416 };
	MyMath::vec2 size = { 96,96 };
	int countY = 1, countX = 3;
};
#pragma endregion
#endif