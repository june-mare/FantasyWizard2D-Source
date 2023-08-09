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
//行商人
class Chara1
{
public:
	MyMath::vec2 start = { 96,0 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4,countX = 3;
};

//帽子かぶってる青年
class Chara2	
{
public:
	MyMath::vec2 start = { 96,512 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};

//金髪の兄ちゃん
class Chara3
{
public:
	MyMath::vec2 start = { 96,640 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};

//オオ様
class Chara4
{
public:
	MyMath::vec2 start = { 0,640 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//おじいちゃん
class Chara5
{
public:
	MyMath::vec2 start = { 0,768 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//おばあちゃん
class Chara6
{
public:
	MyMath::vec2 start = { 96,768 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//くのいち
class Chara7
{
public:
	MyMath::vec2 start = { 0,896 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//村のお姉さん
class Chara8
{
public:
	MyMath::vec2 start = { 192,640 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};

//魔女っ子
class Chara9
{
public:
	MyMath::vec2 start = { 192,768 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};

#pragma endregion
#pragma region Enemy
//兵士A
class Enemy1
{
public:
	MyMath::vec2 start = { 192,0 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//魔女教
class Enemy2
{
public:
	MyMath::vec2 start = { 288,0 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//ブラックスケルトン
class Enemy3
{
public:
	MyMath::vec2 start = { 288,128 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//まっくろくろすけ
class Enemy4
{
public:
	MyMath::vec2 start = { 288,384 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//まおう
class Enemy5
{
public:
	MyMath::vec2 start = { 288,512 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//ゴースト
class Enemy6
{
public:
	MyMath::vec2 start = { 0,512 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//兵士B
class Enemy7
{
public:
	MyMath::vec2 start = { 192,512 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//ゴブリンウィザード
class Enemy8
{
public:
	MyMath::vec2 start = { 96,896 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//ゴブリン
class Enemy9
{
public:
	MyMath::vec2 start = { 192,896 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
//エイリアン	
class Enemy10
{
public:
	MyMath::vec2 start = { 288,256 };
	MyMath::vec2 size = { 32,32 };
	int countY = 4, countX = 3;
};
#pragma endregion
#pragma region Boss
//エイリアン
class Boss1
{
public:
	MyMath::vec2 start = { 0,128 };
	MyMath::vec2 size = { 96,96 };
	int countY = 1, countX = 3;
};
//魔導士
class Boss2
{
public:
	MyMath::vec2 start = { 0,224 };
	MyMath::vec2 size = { 96,96 };
	int countY = 1, countX = 3;
};
//まっくろくろすけ
class Boss3
{
public:
	MyMath::vec2 start = { 0,320 };
	MyMath::vec2 size = { 96,96 };
	int countY = 1, countX = 3;
};
//竜
class Boss4
{
public:
	MyMath::vec2 start = { 0,416 };
	MyMath::vec2 size = { 96,96 };
	int countY = 1, countX = 3;
};
#pragma endregion
#endif