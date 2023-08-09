#pragma once
#ifndef RECT_H_INCLUDE
#define RECT_H_INCLUDE

/*2D�̒����`*/
class Square
{
public:
	float min[2];
	float max[2];
	Square() = default;
};

using SquareList = std::vector<Square>;

/*�����`�̍��W���ړ�������*/
inline Square Add(const Square& r, float x, float y)
{
	return { r.min[0] + x,r.min[1] + y,r.max[0] + x ,r.max[1] + y };
}

/*2�̒����`���d�Ȃ��Ă��邩���ׂ�
@retval true �d�Ȃ��Ă���
@retval false �d�Ȃ��Ă��Ȃ�
*/
inline bool Overlap(const Square& a, const Square& b)
{
	return a.min[0] <= b.max[0] && b.min[0] <= a.max[0] &&
		a.min[1] <= b.max[1] && b.min[1] <= a.max[1];
}

/*2�̒����`�z�񂪏d�Ȃ��Ă��邩���ׂ�
@retval true �d�Ȃ��Ă���
@retval false �d�Ȃ��Ă��Ȃ�
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

/*2�̒����`�z�񂪏d�Ȃ��Ă��邩���ׂ�
@retval true �d�Ȃ��Ă���
@retval false �d�Ȃ��Ă��Ȃ�
*/
inline bool Overlap(const SquareList& ra, const Square& rb)
{
	for (const auto& a : ra)
	{
		if (Overlap(a, rb))return true;
	}
	return false;
}

/*2�̒����`�z�񂪏d�Ȃ��Ă��邩���ׂ�
@retval true �d�Ȃ��Ă���
@retval false �d�Ȃ��Ă��Ȃ�
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