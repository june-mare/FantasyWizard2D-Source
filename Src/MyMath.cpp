#include "MyMath.h"
#include <cmath>
#include <iostream>

namespace MyMath {

#pragma region vec
#pragma region Radian
	float Radian(float x1, float y1, float x2, float y2)
	{
		return std::atan2(y2 - y1, x2 - x1);
	}
	float Radian(vec2 point, vec2 target)
	{
		return std::atan2(target.y - point.y, target.x - point.x);
	}
	float Radian(vec2 distance)
	{
		return std::atan2(distance.y, distance.x);
	}
	float Radian(float x, float y)
	{
		return std::atan2(y, x);
	}
#pragma endregion
	//距離
#pragma region Distance
	vec2 Distance(float x1, float y1, float x2, float y2)
	{
		return vec2(
			x2 - x1,
			y2 - y1
		);
	}
	vec2 Distance(vec2 point, vec2 target)
	{
		return vec2(
			target.x - point.x,
			target.y - point.y
		);
	}

	inline float vec2::Distance(const vec2& other)const {
		return (other - *this).Magnitude();
	}
	inline float vec3::Distance(const vec3& other)const {
		return (other - *this).Magnitude();
	}
	inline float vec4::Distance(const vec4& other)const {
		return (other - *this).Magnitude();
	}
#pragma endregion
	//大きさ
#pragma region Magnitude
	//長さ
	float Magnitude(float x, float y)
	{
		return sqrt(x * x + y * y);
	}
	float Magnitude(float x, float y, float z)
	{
		return sqrt(x * x + y * y + z * z);
	}

	float vec2::Magnitude()const
	{
		return sqrt(Dot(*this));
	}
	constexpr float vec2::SqMagnitude()const noexcept
	{
		return Dot(*this);
	}
	float vec3::Magnitude()const
	{
		return sqrt(Dot(*this));
	}
	constexpr float vec3::SqMagnitude()const noexcept
	{
		return Dot(*this);
	}
	float vec4::Magnitude()const
	{
		return sqrt(Dot(*this));
	}
	constexpr float vec4::SqMagnitude()const noexcept
	{
		return Dot(*this);
	}
#pragma endregion
	//内積
#pragma region Dot
	constexpr float vec2::Dot(const vec2& other)const noexcept
	{
		return this->x * other.x + this->y * other.y;
	}
	float vec2::NormalizeDot(const vec2& other)noexcept
	{
		vec2 v1 = this->normal();
		vec2 v2 = other.normal();
		return v1.x * v2.x + v1.y * v2.y;
	}
	constexpr float vec3::Dot(const vec3& other)const noexcept
	{
		return this->x * other.x + this->y * other.y + this->z * other.z;
	}
	float vec3::NormalizeDot(const vec3& other)noexcept
	{
		vec3 v1 = this->normal();
		vec3 v2 = other.normal();
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	constexpr float vec4::Dot(const vec4& other)const noexcept
	{
		return this->x * other.x + this->y * other.y + this->z * other.z;
	}
	float vec4::NormalizeDot(const vec4& other)noexcept
	{
		vec4 v1 = this->normal();
		vec4 v2 = other.normal();
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

#pragma endregion
	//外積
#pragma region Cross
	constexpr const float vec2::Cross(const vec2& other)const noexcept
	{
		return this->x * other.y - this->y * other.x;
	}
	constexpr const vec3 vec3::Cross(const vec3& other)const noexcept
	{
		return vec3(
			this->y * other.z - this->z * other.y,
			this->z * other.x - this->x * other.z,
			this->x * other.y - this->y * other.x
		);
	}
	constexpr const vec4 vec4::Cross(const vec4& other)const noexcept
	{
		return vec4(
			vec3(this->y * other.z - this->z * other.y,
				this->z * other.x - this->x * other.z,
				this->x * other.y - this->y * other.x)
		);
	}
	inline constexpr const vec4 Cross3(const vec4& v1, const vec4& v2, const vec4& v3)noexcept
	{
		vec4 tmp1 = v2 - v1;
		vec4 tmp2 = v3 - v2;
		return tmp1.Cross(tmp2);
	}
#pragma endregion
	//正規化
#pragma region Normalize
	inline const vec2 vec2::Normalize()noexcept
	{
		return *this /= Magnitude();
	}
	inline const vec2 vec2::normal()const
	{
		return *this / Magnitude();
	}
	inline const vec3 vec3::Normalize()noexcept
	{
		return *this /= Magnitude();
	}
	inline const vec3 vec3::normal()const
	{
		return *this / Magnitude();
	}
	inline const vec4 vec4::Normalize()noexcept
	{
		return *this /= Magnitude();
	}
	inline const vec4 vec4::normal()const
	{
		return *this / Magnitude();
	}
	//単位ベクトル
	vec2 Normalize(float x, float y)
	{
		float l = Magnitude(x, y);
		vec2 v;
		v.x = x / l;
		v.y = y / l;
		return v;
	}
	vec3 Normalize(float x, float y, float z)
	{
		float l = Magnitude(x, y, z);
		vec3 v;
		v.x = x / l;
		v.y = y / l;
		v.z = z / l;
		return v;
	}
#pragma endregion
	//射影ベクトル
#pragma region Projection
	vec2 Projection(vec2 v, vec2 other)
	{
		return (v.Dot(other) / other.Dot(other)) * other;
	}
	vec3 Projection(vec3 v, vec3 other)
	{
		return (v.Dot(other) / other.Dot(other)) * other;
	}
	vec4 Projection(vec4 v, vec4 other)
	{
		return (v.Dot(other) / other.Dot(other)) * other;
	}
	inline constexpr vec2 vec2::Projection(const vec2& other)const
	{
		return (this->Dot(other) / other.Dot(other)) * other;
	}
	inline constexpr vec3 vec3::Projection(const vec3& other)const
	{
		return (this->Dot(other) / other.Dot(other)) * other;
	}
	inline constexpr vec4 vec4::Projection(const vec4& other)const
	{
		return (this->Dot(other) / other.Dot(other)) * other;
	}
#pragma endregion
	//プロジェクションで出てきた垂線を表す
#pragma region Rejection
	inline constexpr vec2 Rejection(vec2 v, vec2 other)
	{
		return v - v.Projection(other);
	}
	inline constexpr vec3 Rejection(vec3 v, vec3 other)
	{
		return v - v.Projection(other);
	}
	inline constexpr vec4 Rejection(vec4 v, vec4 other)
	{
		return v - v.Projection(other);
	}
	inline constexpr vec2 vec2::Rejection(const vec2& other)const
	{
		return *this - Projection(other);
	}
	inline constexpr vec3 vec3::Rejection(const vec3& other)const
	{
		return *this - Projection(other);
	}
	inline constexpr vec4 vec4::Rejection(const vec4& other)const
	{
		return *this - Projection(other);
	}
#pragma endregion
	//反射
#pragma region Reflection
	/*
	v 進行ベクトル
	other 反射する面
	*/
	inline constexpr vec2 Reflection(vec2 v, vec2 other)noexcept
	{
		return v + (-2.0f * v.Dot(other)) * other;
	}
	inline constexpr vec3 Reflection(vec3 v, vec3 other)noexcept
	{
		return v + (-2.0f * v.Dot(other)) * other;
	}
	inline constexpr vec4 Reflection(vec4 v, vec4 other)noexcept
	{
		return v + (-2.0f * v.Dot(other)) * other;
	}

	inline constexpr vec2 vec2::Reflection(const vec2& other)noexcept
	{
		return *this + (-2.0f * this->Dot(other)) * other;
	}
	inline constexpr vec3 vec3::Reflection(const vec3& other)noexcept
	{
		return *this + (-2.0f * this->Dot(other)) * other;
	}
	inline constexpr vec4 vec4::Reflection(const vec4& other)noexcept
	{
		return *this + (-2.0f * this->Dot(other)) * other;
	}
#pragma endregion

#pragma endregion

//#pragma region mat

	//転置
#pragma region Transpose
	constexpr const mat2 mat2::Transpose()const noexcept
	{
		return mat2(
			m[0][0], m[1][0],
			m[0][1], m[1][1]
		);
	}
	constexpr const mat3 mat3::Transpose()const noexcept
	{
		return mat3(
			m[0][0], m[1][0], m[2][0],
			m[0][1], m[1][1], m[2][1],
			m[0][2], m[1][2], m[2][2]
		);
	}
	constexpr const mat4 mat4::Transpose()const noexcept
	{
		return mat4(
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
		);
	}
#pragma endregion
	//単位マトリクス
#pragma region Identity
	constexpr const mat2 mat2::Identity()const noexcept
	{
		return mat2(
			1.0f, 0.0f,
			0.0f, 1.0f
		);
	}
	constexpr const mat3 mat3::Identity()const noexcept
	{
		return mat3(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		);
	}
	constexpr const mat4 mat4::Identity()const noexcept
	{
		return mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
#pragma endregion
	//行列式
#pragma region Determinant
	constexpr float mat2::Determinant()const noexcept
	{
		//小行列式
		return m[0][0] * m[1][1] - m[0][1] * m[1][0];
	}
	constexpr float mat3::Determinant()const noexcept
	{
		//余因子
		double c0 = mat2(
			m[1][1], m[1][2],
			m[2][1], m[2][2]
		).Determinant();
		double c1 = -(mat2(
			m[1][0], m[1][2],
			m[2][0], m[2][1]
		).Determinant());
		double c2 = mat2(
			m[1][0], m[1][1],
			m[2][0], m[2][1]
		).Determinant();

		//小行列式
		return m[0][0] * c0 + m[0][1] * c1 + m[0][2] * c2;
	}
	constexpr float mat4::Determinant()const noexcept
	{
		//余因子
		double c0 = mat3(
			m[1][1], m[1][2], m[1][3],
			m[2][1], m[2][2], m[2][3],
			m[3][1], m[3][2], m[3][3]
		).Determinant();
		double c1 = -(mat3(
			m[1][0], m[1][2], m[1][3],
			m[2][0], m[2][2], m[2][3],
			m[3][0], m[3][2], m[3][3]
		).Determinant());
		double c2 = mat3(
			m[1][0], m[1][1], m[1][3],
			m[2][0], m[2][1], m[2][3],
			m[3][0], m[3][1], m[3][3]
		).Determinant();
		double c3 = -(mat3(
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2],
			m[3][0], m[3][1], m[3][2]
		).Determinant());
		//小行列式
		return m[0][0] * c0 + m[0][1] * c1 + m[0][2] * c2 + m[0][3] * c3;
	}
#pragma endregion
	//逆引数  ※使用式余因子式
#pragma region Inverse
	constexpr const mat2 mat2::Inverse()const noexcept
	{
		double rd = 1.0f / this->Determinant();
		return mat2(
			m[1][1] * rd, -m[0][1] * rd,
			-m[1][0] * rd, m[0][0] * rd);
	}
	constexpr const mat3 mat3::Inverse()const noexcept
	{
		double c0 = mat2(
			m[1][1], m[1][2],
			m[2][1], m[2][2]
		).Determinant();
		double c1 = -(mat2(
			m[1][0], m[1][2],
			m[2][0], m[2][2]
		).Determinant());
		double c2 = mat2(
			m[1][0], m[1][1],
			m[2][0], m[2][1]
		).Determinant();

		double c10 = -(mat2(
			m[0][1], m[0][2],
			m[2][1], m[2][2]
		).Determinant());
		double c11 = mat2(
			m[0][0], m[0][2],
			m[2][0], m[2][2]
		).Determinant();
		double c12 = -(mat2(
			m[0][0], m[0][1],
			m[2][0], m[2][1]
		).Determinant());

		double c20 = mat2(
			m[0][1], m[0][2],
			m[1][1], m[1][2]
		).Determinant();
		double c21 = -(mat2(
			m[0][0], m[0][2],
			m[1][0], m[1][2]
		).Determinant());
		double c22 = mat2(
			m[0][0], m[0][1],
			m[1][0], m[1][1]
		).Determinant();

		double rd = 1.0f / (c0 + c1 + c2);

		return mat3(
			c0 * rd, c10 * rd, c20 * rd,
			c1 * rd, c11 * rd, c21 * rd,
			c2 * rd, c12 * rd, c22 * rd
		);
	}
	constexpr const mat4 mat4::Inverse()const noexcept
	{
		double c0 = mat3(
			m[1][1], m[1][2], m[1][3],
			m[2][1], m[2][2], m[2][3],
			m[3][1], m[3][2], m[3][3]
		).Determinant();
		double c1 = -(mat3(
			m[1][0], m[1][2], m[1][3],
			m[2][0], m[2][2], m[2][3],
			m[3][0], m[3][2], m[3][3]
		).Determinant());
		double c2 = mat3(
			m[1][0], m[1][1], m[1][3],
			m[2][0], m[2][1], m[2][3],
			m[3][0], m[3][1], m[3][3]
		).Determinant();
		double c3 = -(mat3(
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2],
			m[3][0], m[3][1], m[3][2]
		).Determinant());

		double c10 = -(mat3(
			m[0][1], m[1][2], m[1][3],
			m[2][1], m[2][2], m[2][3],
			m[3][1], m[3][2], m[3][3]
		).Determinant());
		double c11 = mat3(
			m[0][0], m[1][2], m[1][3],
			m[2][0], m[2][2], m[2][3],
			m[3][0], m[3][2], m[3][3]
		).Determinant();
		double c12 = -(mat3(
			m[0][0], m[1][1], m[1][3],
			m[2][0], m[2][1], m[2][3],
			m[3][0], m[3][1], m[3][3]
		).Determinant());
		double c13 = mat3(
			m[0][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2],
			m[3][0], m[3][1], m[3][2]
		).Determinant();

		double c20 = mat3(
			m[0][1], m[1][2], m[1][3],
			m[1][1], m[2][2], m[2][3],
			m[3][1], m[3][2], m[3][3]
		).Determinant();
		double c21 = -(mat3(
			m[0][0], m[1][2], m[1][3],
			m[1][0], m[2][2], m[2][3],
			m[3][0], m[3][2], m[3][3]
		).Determinant());
		double c22 = mat3(
			m[0][0], m[1][1], m[1][3],
			m[1][0], m[2][1], m[2][3],
			m[3][0], m[3][1], m[3][3]
		).Determinant();
		double c23 = -(mat3(
			m[0][0], m[1][1], m[1][2],
			m[1][0], m[2][1], m[2][2],
			m[3][0], m[3][1], m[3][2]
		).Determinant());

		double c30 = -(mat3(
			m[0][1], m[1][2], m[1][3],
			m[1][1], m[2][2], m[2][3],
			m[2][1], m[3][2], m[3][3]
		).Determinant());
		double c31 = mat3(
			m[0][0], m[1][2], m[1][3],
			m[1][0], m[2][2], m[2][3],
			m[2][0], m[3][2], m[3][3]
		).Determinant();
		double c32 = -(mat3(
			m[0][0], m[1][1], m[1][3],
			m[1][0], m[2][1], m[2][3],
			m[2][0], m[3][1], m[3][3]
		).Determinant());
		double c33 = mat3(
			m[0][0], m[1][1], m[1][2],
			m[1][0], m[2][1], m[2][2],
			m[2][0], m[3][1], m[3][2]
		).Determinant();

		double rd = 1.0f /
			(m[0][0] * c0 + m[0][1] * c1 + m[0][2] * c2 + m[0][3] * c3);

		return mat4(
			c0 * rd, c10 * rd, c20 * rd, c30 * rd,
			c1 * rd, c11 * rd, c21 * rd, c31 * rd,
			c2 * rd, c12 * rd, c22 * rd, c32 * rd,
			c3 * rd, c13 * rd, c23 * rd, c33 * rd
		);
	}
#pragma endregion
	//拡大縮小マトリクス
#pragma region Scale
	constexpr const mat2 mat2::Scale(const vec2& scale)const noexcept
	{
		return mat2(
			scale.x, 0.0f,
			0.0f, scale.y
		);
	}
	constexpr const mat2 mat2::Scale(float scale)const noexcept
	{
		return mat2(
			scale, 0.0f,
			0.0f, scale
		);
	}
	constexpr const mat3 mat3::Scale(const vec3& scale)const noexcept
	{
		return mat3(
			scale.x, 0.0f, 0.0f,
			0.0f, scale.y, 0.0f,
			0.0f, 0.0f, scale.z
		);
	}
	constexpr const mat3 mat3::Scale(float scale)const noexcept
	{
		return mat3(
			scale, 0.0f, 0.0f,
			0.0f, scale, 0.0f,
			0.0f, 0.0f, scale
		);
	}
	constexpr const mat4 mat4::Scale(const vec4& scale)const noexcept
	{
		return mat4(
			scale.x, 0.0f, 0.0f, 0.0f,
			0.0f, scale.y, 0.0f, 0.0f,
			0.0f, 0.0f, scale.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
	constexpr const mat4 mat4::Scale(float scale)const noexcept
	{
		return mat4(
			scale, 0.0f, 0.0f, 0.0f,
			0.0f, scale, 0.0f, 0.0f,
			0.0f, 0.0f, scale, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
#pragma endregion
	//回転マトリクス
#pragma region Rotate
	const mat2 mat2::Rotate(float degree)const noexcept
	{
		return mat2(
			cos(degree), -sin(degree),
			sin(degree), cos(degree)
		);
	}
	constexpr const mat3 mat3::RotateX(float degree)const noexcept
	{
		return mat3(
			1.0f, 0.0f, 0.0f,
			0.0f, cos(degree), -sin(degree),
			0.0f, sin(degree), cos(degree)
		);
	}
	constexpr const mat3 mat3::RotateY(float degree)const noexcept
	{
		return mat3(
			cos(degree), 0.0f, sin(degree),
			0.0f, 1.0f, 0.0f,
			-sin(degree), 0.0f, cos(degree)
		);
	}
	constexpr const mat3 mat3::RotateZ(float degree)const noexcept
	{
		return mat3(
			cos(degree), -sin(degree), 0.0f,
			sin(degree), cos(degree), 0.0f,
			0.0f, 0.0f, 1.0f
		);
	}
	constexpr const mat4 mat4::RotateX(float degree)const noexcept
	{
		return mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cos(degree), -sin(degree), 0.0f,
			0.0f, sin(degree), cos(degree), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
	constexpr const mat4 mat4::RotateY(float degree)const noexcept
	{
		return mat4(
			cos(degree), 0.0f, sin(degree), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sin(degree), 0.0f, cos(degree), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
	constexpr const mat4 mat4::RotateZ(float degree)const noexcept
	{
		return mat4(
			cos(degree), -sin(degree), 0.0f, 0.0f,
			sin(degree), cos(degree), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}
#pragma endregion


//#pragma endregion

#pragma region GameFunction

	mat4 LookAt(vec3 position, vec3 target, vec3 up)
	{
		vec3 forward = position - target;
		forward.Normalize();
		
		vec3 right = up.Cross(forward);
		right.Normalize();

		vec3 newUp = forward.Cross(right);

		return mat4(
			right.x,	right.y,	right.z,	0,
			newUp.x,	newUp.y,	newUp.z,	0,
			forward.x,	forward.y,	forward.z,	0,
			position.x,	position.y,	position.z,	1
		);
	}
	
	//角度数値変換
	float Radian2Degree(float value) { return value * M_PI / 180.0; }
	float Degree2Radian(float value) { return value * 180.0 / M_PI; }

	//現在地 position 回転度数　r 
	vec2 MovePoint(vec2 position, float r, float distance)
	{
		vec2 point;
		point.x = position.x + std::sin(r) * distance;
		point.y = position.y + std::cos(r) * distance;
		return point;
	}
	vec2 MovePoint(vec2 center, vec2 position, float r)
	{
		vec2 tmpv = Distance(position, center);

		vec2 point;
		point.x = tmpv.x * std::cos(r) - tmpv.y * std::sin(r);
		point.y = tmpv.x * std::sin(r) + tmpv.y * std::cos(r);

		return point + center;
	}

	float GetTheta(vec2 v1, vec2 v2)
	{
		return acos(v1.Dot(v2) / (v1.Magnitude() * v2.Magnitude()));
	}
	float GetTheta(vec3 v1, vec3 v2)
	{
		return acos(v1.Dot(v2) / (v1.Magnitude() * v2.Magnitude()));
	}
	float GetTheta(vec4 v1, vec4 v2)
	{
		return acos(v1.Dot(v2) / (v1.Magnitude() * v2.Magnitude()));
	}

	//角度
	float ToAngle(vec2 vector1, vec2 vector2)
	{
		return atan2(vector2.y - vector1.y, vector2.x - vector1.x);
	}
	float ToAngle(vec2 vector)
	{
		return atan2(vector.x, vector.y);
	}

	float Angle2Vector(vec4 vector1, vec4 vector2)
	{
		return
			acos(
				vector1.Dot(vector2) / (vector1.Magnitude() * vector2.Magnitude())
			);
	}
	vec2 RadToVector2(float rad) noexcept
	{
		return vec2(cos(rad * M_PI / 180), sin(rad * M_PI / 180));
	}

	float Lerp(float start, float target, float value)
	{
		return start + (target - start) * value;
	}
	vec2 Lerp(vec2 start, vec2 target, float value)
	{
		return start + (target - start) * value;
	}
	vec3 Lerp(vec3 start, vec3 target, float value)
	{
		return start + (target - start) * value;
	}
	vec4 Lerp(vec4 start, vec4 target, float value)
	{
		return start + (target - start) * value;
	}
	vec2 Slerp(vec2 start, vec2 target, float value)
	{
		vec2 p = start + (target - start) / 2;
		return MovePoint(p,start,value);

		float theta = M_PI * value;
		mat2 mt;
		return start + p * mt.Rotate(theta);	
	}

	vec3 Slerp(vec3 start, vec3 target, float value)
	{
		vec3 p = start + (target - start) / 2;

		float theta = M_PI * value;
		mat2 mt;
		//return start + p * mt.Rotate(theta);
		return vec3();
	}

	vec4 Slerp(vec4 start, vec4 target, float value)
	{
		return vec4();
	}

	float Easeing(float cave, float value, float x, float y)
	{
		return  cave * std::pow(value - x,2.0) + y;
	}

	mat4 Perspective(float fovY, float aspect, float zNear, float zFar)
	{
		float f = 1 / (zNear - zFar); 
		float i = 1 / tan(fovY / 2.0);
		

		return mat4(
			f/aspect,0,0,0,
			0,f,0,0,
			0,0,(zNear + zFar) * 1,-1,
			0,0,(2 * zFar * zNear) * 1,0
		);
	}




#pragma endregion

	
}