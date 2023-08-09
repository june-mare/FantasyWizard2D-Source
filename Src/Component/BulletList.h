#pragma once
#ifndef BULLETLIST_H_INCLUDE
#define BULLETLIST_H_INCLUDE
#include "../MyMath.h"
#include "../Sprite.h"
//#define IMG_OBJECTS_SIZE 1024

template <typename T>
std::vector<Texcoord> BulletTCList()
{
	std::vector<Texcoord> tcList;
	T bulletdata;
	tcList.resize(bulletdata.count);
	for (int i = 0; i < bulletdata.count; i++) {
		auto tc = MakeTexcoord(bulletdata.start.x + (bulletdata.size.x * i), bulletdata.start.y,
			bulletdata.size.x, bulletdata.size.y,
			IMG_OBJECTS_SIZE, IMG_OBJECTS_SIZE);
		tcList[i] = tc;
	}
	return tcList;
}

class RedBullet1
{
public:
	MyMath::vec2 start = { 384,256 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;	
};

class RedBullet2
{
public:
	MyMath::vec2 start = { 384,266 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class RedBullet3
{
public:
	MyMath::vec2 start = { 384,276 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class RedBullet4
{
public:
	MyMath::vec2 start = { 384,286 };
	MyMath::vec2 size = { 10,10 };
	int count = 6;
};

class RedBullet5
{
public:
	MyMath::vec2 start = { 384,296 };
	MyMath::vec2 size = { 10,10 };
	int count = 5;
};

class RedBullet6
{
public:
	MyMath::vec2 start = { 384,306 };
	MyMath::vec2 size = { 10,14 };
	int count = 5;
};

class RedBullet7
{
public:
	MyMath::vec2 start = { 384,321 };
	MyMath::vec2 size = { 11,10 };
	int count = 4;
};

class RedBullet8
{
public:
	MyMath::vec2 start = { 384,331 };
	MyMath::vec2 size = { 10,20 };
	int count = 4;
};

class BuleBullet1
{
public:
	MyMath::vec2 start = { 424,256 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class BuleBullet2
{
public:
	MyMath::vec2 start = { 424,266 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class BuleBullet3
{
public:
	MyMath::vec2 start = { 424,276 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class BuleBullet4
{
public:
	MyMath::vec2 start = { 444,286 };
	MyMath::vec2 size = { 10,10 };
	int count = 6;
};

class BuleBullet5
{
public:
	MyMath::vec2 start = { 434,296 };
	MyMath::vec2 size = { 10,10 };
	int count = 5;
};

class BuleBullet6
{
public:
	MyMath::vec2 start = { 434,306 };
	MyMath::vec2 size = { 10,14 };
	int count = 6;
};

class BuleBullet7
{
public:
	MyMath::vec2 start = { 428,321 };
	MyMath::vec2 size = { 11,10 };
	int count = 4;
};

class BuleBullet8
{
public:
	MyMath::vec2 start = { 424,331 };
	MyMath::vec2 size = { 10,20 };
	int count = 4;
};

class GreenBullet1
{
public:
	MyMath::vec2 start = { 464,256 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class GreenBullet2
{
public:
	MyMath::vec2 start = { 464,266 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class GreenBullet3
{
public:
	MyMath::vec2 start = { 464,276 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class GreenBullet4
{
public:
	MyMath::vec2 start = { 504,286 };
	MyMath::vec2 size = { 10,10 };
	int count = 6;
};

class GreenBullet5
{
public:
	MyMath::vec2 start = { 484,296 };
	MyMath::vec2 size = { 10,10 };
	int count = 5;
};

class GreenBullet6
{
public:
	MyMath::vec2 start = { 494,306 };
	MyMath::vec2 size = { 10,14 };
	int count = 6;
};

class GreenBullet7
{
public:
	MyMath::vec2 start = { 472,321 };
	MyMath::vec2 size = { 11,10 };
	int count = 4;
};

class GreenBullet8
{
public:
	MyMath::vec2 start = { 464,331 };
	MyMath::vec2 size = { 10,20 };
	int count = 4;
};

class YellowBullet1
{
public:
	MyMath::vec2 start = { 504,256 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class YellowBullet2
{
public:
	MyMath::vec2 start = { 504,266 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class YellowBullet3
{
public:
	MyMath::vec2 start = { 504,276 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class YellowBullet4
{
public:
	MyMath::vec2 start = { 564,286 };
	MyMath::vec2 size = { 10,10 };
	int count = 6;
};

class YellowBullet5
{
public:
	MyMath::vec2 start = { 534,296 };
	MyMath::vec2 size = { 10,10 };
	int count = 5;
};

class YellowBullet6
{
public:
	MyMath::vec2 start = { 554,306 };
	MyMath::vec2 size = { 10,14 };
	int count = 6;
};

class YellowBullet7
{
public:
	MyMath::vec2 start = { 512,321 };
	MyMath::vec2 size = { 11,10 };
	int count = 4;
};

class YellowBullet8
{
public:
	MyMath::vec2 start = { 504,331 };
	MyMath::vec2 size = { 10,20 };
	int count = 4;
};

class PurpleBullet1
{
public:
	MyMath::vec2 start = { 544,256 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class PurpleBullet2
{
public:
	MyMath::vec2 start = { 544,266 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class PurpleBullet3
{
public:
	MyMath::vec2 start = { 544,276 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class PurpleBullet4
{
public:
	MyMath::vec2 start = { 624,286 };
	MyMath::vec2 size = { 10,10 };
	int count = 6;
};

class PurpleBullet5
{
public:
	MyMath::vec2 start = { 584,296 };
	MyMath::vec2 size = { 10,10 };
	int count = 5;
};

class PurpleBullet6
{
public:
	MyMath::vec2 start = { 714,306 };
	MyMath::vec2 size = { 10,14 };
	int count = 6;
};

class PurpleBullet7
{
public:
	MyMath::vec2 start = { 560,321 };
	MyMath::vec2 size = { 11,10 };
	int count = 4;
};

class PurpleBullet8
{
public:
	MyMath::vec2 start = { 544,331 };
	MyMath::vec2 size = { 10,20 };
	int count = 4;
};
//
class LightBlueBullet1
{
public:
	MyMath::vec2 start = { 584,256 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class LightBlueBullet2
{
public:
	MyMath::vec2 start = { 584,266 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class LightBlueBullet3
{
public:
	MyMath::vec2 start = { 584,276 };
	MyMath::vec2 size = { 10,10 };
	int count = 4;
};

class LightBlueBullet4
{
public:
	MyMath::vec2 start = { 684,286 };
	MyMath::vec2 size = { 10,10 };
	int count = 6;
};

class LightBlueBullet5
{
public:
	MyMath::vec2 start = { 634,296 };
	MyMath::vec2 size = { 10,10 };
	int count = 5;
};

class LightBlueBullet6
{
public:
	MyMath::vec2 start = { 674,306 };
	MyMath::vec2 size = { 10,14 };
	int count = 6;
};

class LightBlueBullet7
{
public:
	MyMath::vec2 start = { 604,321 };
	MyMath::vec2 size = { 11,10 };
	int count = 4;
};

class LightBlueBullet8
{
public:
	MyMath::vec2 start = { 584,331 };
	MyMath::vec2 size = { 10,20 };
	int count = 4;
};

class LaserRedSmall
{
public:
	MyMath::vec2 start = { 320,0 };
	MyMath::vec2 size = { 23,512 };
	int count = 4;
};

class LaserBlueSmall
{
public:
	MyMath::vec2 start = { 412,0 };
	MyMath::vec2 size = { 23,512 };
	int count = 4;
};

class LaserPinkBig
{
public:
	MyMath::vec2 start = { 320,512 };
	MyMath::vec2 size = { 64,535 };
	int count = 7;
};


#endif // !BULLETLIST_H_INCLUDE