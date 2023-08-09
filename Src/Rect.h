#pragma once
#ifndef RECT_H_INCLUDE
#define RECT_H_INCLUDE

/*2Dの長方形*/
class Square
{
public:
	float min[2];
	float max[2];
	Square() = default;
};

using SquareList = std::vector<Square>;

/*長方形の座標を移動させる*/
inline Square Add(const Square& r, float x, float y)
{
	return { r.min[0] + x,r.min[1] + y,r.max[0] + x ,r.max[1] + y };
}

/*2つの長方形が重なっているか調べる
@retval true 重なっている
@retval false 重なっていない
*/
inline bool Overlap(const Square& a, const Square& b)
{
	return a.min[0] <= b.max[0] && b.min[0] <= a.max[0] &&
		a.min[1] <= b.max[1] && b.min[1] <= a.max[1];
}

/*2つの長方形配列が重なっているか調べる
@retval true 重なっている
@retval false 重なっていない
*/
inline bool Overlap(const SquareList& ra, const SquareList& rb)
{
	for (const auto& a : ra)
	{
		for (const auto& b : rb)
		{
			if (Overlap(a, b))return true;
		}
	}
	return false;
}

/*2つの長方形配列が重なっているか調べる
@retval true 重なっている
@retval false 重なっていない
*/
inline bool Overlap(const SquareList& ra, const Square& rb)
{
	for (const auto& a : ra)
	{
		if (Overlap(a, rb))return true;
	}
	return false;
}

/*2つの長方形配列が重なっているか調べる
@retval true 重なっている
@retval false 重なっていない
*/
inline bool Overlap(const Square& ra, const SquareList& rb)
{
	for (const auto& a : rb)
	{
		if (Overlap(ra, a))return true;
	}
	return false;
}

#endif // !RECT_H_INCLUDE