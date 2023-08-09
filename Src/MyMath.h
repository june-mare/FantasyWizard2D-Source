#pragma once
#ifndef MYMATH_H_INCLUDE
#define MYMATH_H_INCLUDE

namespace MyMath
{
#pragma region define 

#define FPS (60.0f)
	//時間
#define SECOND(_sec) (_sec * FPS)
#define MINUTE(_min) (SECOND(_min * 60))
#define HOUR(_hour)  (MINUTE(_hour * 60))
	//距離　（デフォルト = M）
//#define M (_m)		 (_m)
//#define CM(_cm)		 (M(0.01f * _cm))
//#define MM(_mm)		 (CM(0.1f * (_mm)))
//#define KM(_km)		 (M(1000.0f*(_km)))
	//速度
#define M_S(_speed)  (M(_speed)/SECOND(1.0f))
#define KM_H(_speed) (KM(_speed)/HOUR(1.0f))
	//加速度の計算
#define CM_S2(_acc)  (CM(_acc)/SECOND(1.0f) * SECOND(1.0f))
#define M_S2(_acc)	 (M(_acc)/(SECOND(1.0f) * SECOND(1.0f)))
#define K_S2(_acc)   (KM(_acc)/(SECOND(1.0f) * SECOND(1.0f)))

#define M_PI 3.14159265359
#define M_REPI 0.31830988618

#pragma endregion

#pragma region vec
	//構造体
#pragma region struct

	struct vec2
	{
		float x = 0, y = 0;
#pragma region Constructor
		vec2()noexcept = default;
		constexpr explicit vec2(float value)noexcept
			:x(value), y(value) {}
		constexpr vec2(float _x, float _y)noexcept
			:x(_x), y(_y) {}
#pragma endregion
#pragma region Calc
		vec2 operator=(const vec2& other)noexcept
		{
			this->x = other.x; this->y = other.y;
			return *this;
		}
		vec2& operator+=(float other)noexcept
		{
			this->x += other;
			this->y += other;
			return *this;
		}
		vec2& operator+=(const vec2& other)noexcept
		{
			this->x += other.x;
			this->y += other.y;
			return *this;
		}
		vec2& operator-=(float other)noexcept
		{
			this->x -= other;
			this->y -= other;
			return *this;
		}
		vec2& operator-=(const vec2& other)noexcept
		{
			this->x -= other.x;
			this->y -= other.y;
			return *this;
		}
		vec2& operator*=(float other)noexcept
		{
			this->x *= other;
			this->y *= other;
			return *this;
		}
		vec2& operator/=(float other)noexcept
		{
			return *this *= (1.0f / other);
		}
		bool operator==(const vec2& other)
		{
			return this->x == other.x && this->y == other.y;
		}
		bool operator!=(const vec2& other)
		{
			return this->x != other.x && this->y != other.y;
		}
		float& operator[](size_t n)noexcept { return *(&x + n); }
		const float& operator[](size_t n)const { return *(&x + n); }
		vec2 Add(float x, float y)
		{
			this->x += x;
			this->y += y;
			return *this;
		}
		vec2 Sub(float x, float y)
		{
			this->x -= x;
			this->y -= y;
			return *this;
		}
#pragma endregion
		//thisポインタの実数を正規化して返す
		inline const vec2 Normalize()noexcept;
		//thisポインタの正規化を返す
		inline const vec2 normal() const;
		//大きさの二乗　※他のベクトルとどちらが長いかを調べるときに処理が軽い
		constexpr float SqMagnitude()const noexcept;
		//大きさ
		float Magnitude()const;
		//内積
		constexpr float Dot(const vec2& other)const noexcept;
		//正規化した内積
		float NormalizeDot(const vec2& other)noexcept;
		//外積
		constexpr const float Cross(const vec2& other)const noexcept;
		//射影
		inline constexpr vec2 Projection(const vec2& other)const;
		//プロジェクションで出てきた垂線を表す
		inline constexpr vec2 Rejection(const vec2& other)const;
		//反射
		inline constexpr vec2 Reflection(const vec2& other)noexcept;
		//距離
		inline float Distance(const vec2& other)const;
	};

	struct vec3 {
		float x = 0, y = 0, z = 0;
#pragma region Constructor
		vec3()noexcept = default;
		explicit constexpr vec3(float value)noexcept
			:x(value), y(value), z(value) {}
		constexpr vec3(vec2 xy, float _z = 0)noexcept
			:x(xy.x), y(xy.y), z(_z) {}
		constexpr vec3(float _x, float _y, float _z)noexcept
			:x(_x), y(_y), z(_z) {}
#pragma endregion
#pragma region Calc
		vec3 operator=(const vec3& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}
		vec3 operator=(const vec2& other)
		{
			x = other.x;
			y = other.y;
			return *this;
		}
		vec3 operator+=(const vec3& other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;
		}
		vec3 operator+=(float other)
		{
			this->x += other;
			this->y += other;
			this->z += other;
			return *this;
		}
		vec3 operator-=(const vec3& other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;
		}
		vec3 operator-=(float other)
		{
			this->x -= other;
			this->y -= other;
			this->z -= other;
			return *this;
		}
		vec3 operator*=(float other)
		{
			this->x *= other;
			this->y *= other;
			this->z *= other;
			return *this;
		}
		vec3 operator/=(float other)
		{
			return *this *= (1.0f / other);
		}
		bool operator==(const vec3& other)
		{
			return this->x == other.x &&
				this->y == other.y &&
				this->z == other.z;
		}
		bool operator!=(const vec3& other)
		{
			return this->x != other.x &&
				this->y != other.y &&
				this->z != other.z;
		}
		float& operator[](size_t n)noexcept { return *(&x + n); }
		const float& operator[](size_t n)const { return *(&x + n); }
		vec3 Add(vec2 xy, float z)
		{
			this->x += xy.x;
			this->y += xy.y;
			this->z += z;
			return *this;
		}
		vec3 Add(float x, float y, float z)
		{
			this->x += x;
			this->y += y;
			this->z += z;
			return *this;
		}
		vec3 Sub(vec2 xy, float z)
		{
			this->x -= xy.x;
			this->y -= xy.y;
			this->z -= z;
			return *this;
		}
		vec3 Sub(float x, float y, float z)
		{
			this->x -= x;
			this->y -= y;
			this->z -= z;
			return *this;
		}
#pragma endregion
		//thisポインタの実数を正規化して返す
		inline const vec3 Normalize()noexcept;
		//thisポインタの正規化を返す
		inline const vec3 normal() const;
		//大きさの二乗　※他のベクトルとどちらが長いかを調べるときに処理が軽い
		constexpr float SqMagnitude()const noexcept;
		//大きさ
		float Magnitude()const;
		//内積
		constexpr float Dot(const vec3& other)const noexcept;
		//正規化した内積
		float NormalizeDot(const vec3& other)noexcept;
		//外積
		constexpr const vec3 Cross(const vec3& other)const noexcept;
		//射影
		inline constexpr vec3 Projection(const vec3& other)const;
		//プロジェクションで出てきた垂線を表す
		inline constexpr vec3 Rejection(const vec3& other)const;
		//反射
		inline constexpr vec3 Reflection(const vec3& other)noexcept;
		//距離
		inline float Distance(const vec3& other)const;
	};

	struct vec4
	{
		float x = 0, y = 0, z = 0, w = 0;
#pragma region Constructor
		vec4()noexcept = default;
		constexpr vec4(float value)noexcept
			:x(value), y(value), z(value), w(value) {}
		constexpr vec4(vec2 xy, float _z = 0, float _w = 0)noexcept
			:x(xy.x), y(xy.y), z(_z), w(_w) {}
		constexpr vec4(vec3 xyz, float _w = 0)noexcept
			:x(xyz.x), y(xyz.y), z(xyz.z), w(_w) {}
		constexpr vec4(float _x, float _y, float _z, float _w)noexcept
			:x(_x), y(_y), z(_z), w(_w) {}
#pragma endregion
#pragma region Calc
		vec4 operator=(const vec2& other) {
			x = other.x;
			y = other.y;
			return *this;
		}
		vec4 operator=(const vec3& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}
		vec4 operator=(const vec4& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
			return *this;
		}
		vec4 operator+=(const vec4& other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;

			return *this;
		}
		vec4 operator+=(float other)
		{
			this->x += other;
			this->y += other;
			this->z += other;

			return *this;
		}
		vec4 operator-=(const vec4& other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;

			return *this;
		}
		vec4 operator-=(float other)
		{
			this->x -= other;
			this->y -= other;
			this->z -= other;

			return *this;
		}
		vec4 operator*=(float other)
		{
			this->x *= other;
			this->y *= other;
			this->z *= other;

			return *this;
		}
		vec4 operator/=(float other)
		{
			return *this *= (1.0f / other);
		}
		bool operator==(const vec4& other)
		{
			return this->x == other.x &&
				this->y == other.y &&
				this->z == other.z;
		}
		bool operator!=(const vec4& other)
		{
			return this->x != other.x &&
				this->y != other.y &&
				this->z != other.z;
		}
		float& operator[](size_t n)noexcept { return *(&x + n); }
		const float& operator[](size_t n)const { return *(&x + n); }
		vec4 Add(vec2 xy, float z)
		{
			this->x += xy.x;
			this->y += xy.y;
			this->z += z;
			return *this;
		}
		vec4 Add(vec3 xyz)
		{
			this->x += xyz.x;
			this->y += xyz.y;
			this->z += xyz.z;
			return *this;
		}
		vec4 Add(float x, float y, float z)
		{
			this->x += x;
			this->y += y;
			this->z += z;
			return *this;
		}
		vec4 Sub(vec2 xy, float z)
		{
			this->x -= xy.x;
			this->y -= xy.y;
			this->z -= z;
			return *this;
		}
		vec4 Sub(vec3 xyz)
		{
			this->x -= xyz.x;
			this->y -= xyz.y;
			this->z -= xyz.z;
			return *this;
		}
		vec4 Sub(float x, float y, float z)
		{
			this->x -= x;
			this->y -= y;
			this->z -= z;
			return *this;
		}
#pragma endregion
		//thisポインタの実数を正規化して返す
		inline const vec4 Normalize()noexcept;
		//thisポインタの正規化を返す
		inline const vec4 normal() const;
		//大きさの二乗　※他のベクトルとどちらが長いかを調べるときに処理が軽い
		constexpr float SqMagnitude()const noexcept;
		//大きさ
		float Magnitude()const;
		//内積
		constexpr float Dot(const vec4& other)const noexcept;
		//正規化した内積
		float NormalizeDot(const vec4& other)noexcept;
		//外積
		constexpr const vec4 Cross(const vec4& other)const noexcept;
		//射影
		inline constexpr vec4 Projection(const vec4& other)const;
		//プロジェクションで出てきた垂線を表す
		inline constexpr vec4 Rejection(const vec4& other)const;
		//反射
		inline constexpr vec4 Reflection(const vec4& other)noexcept;
		//距離
		inline float Distance(const vec4& other)const;
		//ポイントとしてこの構造体を使う		
		inline constexpr void ToPoint()noexcept
		{
			this->w = 1.0f;
		}
		//ベクトルとしてこの構造体を使う
		inline constexpr void ToVector()noexcept
		{
			this->w = 0.0f;
		}
	};

#pragma endregion
	//逆ベクトル用operator 
#pragma region avec
	inline constexpr const vec2 operator-(const vec2& v) {
		return vec2(-v.x, -v.y);
	}
	inline constexpr const vec3 operator-(const vec3& v)
	{
		return vec3(-v.x, -v.y, -v.z);
	}
	inline constexpr const vec4 operator-(const vec4& v)
	{
		return vec4(-v.x, -v.y, -v.z, v.w);
	}
#pragma endregion
	//vecのoperator
#pragma region vecoperator
	inline constexpr const vec2 operator+(const vec2& v, const vec2& other)
	{
		return vec2(v.x + other.x, v.y + other.y);
	}
	inline constexpr const vec2 operator+(const vec2& v, float other)
	{
		return vec2(v.x + other, v.y + other);
	}
	inline constexpr const vec3 operator+(const vec3& v, const vec3& other)
	{
		return vec3(v.x + other.x, v.y + other.y, v.z + other.z);
	}
	inline constexpr const vec3 operator+(const vec3& v, float other)
	{
		return vec3(v.x + other, v.y + other, v.z + other);
	}
	inline constexpr const vec4 operator+(const vec4& v, const vec4& other)
	{
		return vec4(v.x + other.x, v.y + other.y, v.z + other.z, 0);
	}
	inline constexpr const vec4 operator+(const vec4& v, float other)
	{
		return vec4(vec3(v.x + other, v.y + other, v.z + other));
	}
	inline constexpr const vec2 operator-(const vec2& v, const vec2& other)
	{
		return vec2(v.x + other.x, v.y + other.y);
	}
	inline constexpr const vec3 operator-(const vec3& v, const vec3& other)
	{
		return vec3(v.x + other.x, v.y + other.y, v.z + other.z);
	}
	inline constexpr const vec4 operator-(const vec4& v, const vec4& other)
	{
		return vec4(v.x + other.x, v.y + other.y, v.z + other.z, 0);
	}
	inline constexpr const vec2 operator*(const vec2& v, float s)noexcept
	{
		return vec2(v.x * s, v.y * s);
	}
	inline constexpr const vec2 operator*(float s, const vec2& v)noexcept
	{
		return vec2(s * v.x, s * v.y);
	}
	inline constexpr const vec3 operator*(const vec3& v, float s)noexcept
	{
		return vec3(v.x * s, v.y * s, v.z * s);
	}
	inline constexpr const vec3 operator*(float s, const vec3& v)noexcept
	{
		return vec3(s * v.x, s * v.y, s * v.z);
	}
	inline constexpr const vec4 operator*(const vec4& v, float s)noexcept
	{
		return vec4(v.x * s, v.y * s, v.z * s, v.w);
	}
	inline constexpr const vec4 operator*(float s, const vec4& v)noexcept
	{
		return vec4(s * v.x, s * v.y, s * v.z, v.w);
	}
	inline constexpr const vec2 operator/(const vec2& v, float other)noexcept {
		return other != 0 ? (v * (1.0f / other)) : v;
	}
	inline constexpr const vec3 operator/(const vec3& v, float other)noexcept {
		return other != 0 ? (v * (1.0f / other)) : v;
	}
	inline constexpr const vec4 operator/(const vec4& v, float other)noexcept {
		return other != 0 ? (v * (1.0f / other)) : v;
	}

#pragma endregion

#pragma endregion

#pragma region mat

	//構造体
#pragma region struct

	struct mat2
	{
#pragma region Constructor
		mat2()noexcept = default;
		constexpr mat2(float m00, float m01,
			float m10, float m11)noexcept :
			m{ m00, m01,
			  m10, m11 } {}
#pragma endregion
#pragma region operator
		mat2& operator+=(const mat2& m)noexcept
		{
			this->m[0][0] += m.m[0][0];
			this->m[0][1] += m.m[0][1];

			this->m[1][0] += m.m[1][0];
			this->m[1][1] += m.m[1][1];
			return *this;
		}
		constexpr const mat2 operator+(const mat2& m2)const noexcept
		{
			return mat2(
				m[0][0] + m2.m[0][0],
				m[0][1] + m2.m[0][1],

				m[1][0] + m2.m[1][0],
				m[1][1] + m2.m[1][1]
			);
		}
		mat2& operator-=(const mat2& m)noexcept
		{
			this->m[0][0] -= m.m[0][0];
			this->m[0][1] -= m.m[0][1];

			this->m[1][0] -= m.m[1][0];
			this->m[1][1] -= m.m[1][1];
			return *this;
		}
		constexpr const mat2 operator-(const mat2& m2)const noexcept
		{
			return mat2(
				m[0][0] - m2.m[0][0],
				m[0][1] - m2.m[0][1],

				m[1][0] - m2.m[1][0],
				m[1][1] - m2.m[1][1]
			);
		}
		mat2& operator*=(const float value)noexcept
		{
			this->m[0][0] *= value;
			this->m[0][1] *= value;

			this->m[1][0] *= value;
			this->m[1][1] *= value;

			return *this;
		}
		constexpr const mat2 operator*(const float value)const noexcept
		{
			return mat2(
				m[0][0] * value,
				m[0][1] * value,

				m[1][0] * value,
				m[1][1] * value
			);
		}
		constexpr const mat2 operator*(const mat2& other)const noexcept
		{
			return mat2(
				m[0][0] * other.m[0][0] +
				m[0][1] * other.m[1][0],

				m[0][0] * other.m[0][1] +
				m[0][1] * other.m[1][1],


				m[1][0] * other.m[0][0] +
				m[1][1] * other.m[1][0],

				m[1][0] * other.m[0][1] +
				m[1][1] * other.m[1][1]
			);
		}
		mat2& operator*=(const mat2& other)noexcept
		{
			*this = *this * other;
			return *this;
		}
#pragma endregion
	public:
		static const int row = 2, col = 2;
		float m[row][col] = { 0 };
	public:
		//転置
		constexpr const mat2 Transpose()const noexcept;
		//単位マトリクス
		constexpr const mat2 Identity()const noexcept;
		//行列式
		constexpr float Determinant()const noexcept;
		//逆行列
		constexpr const mat2 Inverse()const noexcept;
		//回転マトリクス
		const mat2 Rotate(float degree)const noexcept;
		//拡大縮小マトリクス
		constexpr const mat2 Scale(const vec2& scale)const noexcept;
		constexpr const mat2 Scale(float scale)const noexcept;
	};

	struct mat3
	{
#pragma region Constructor
		constexpr mat3(
			float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22
		)noexcept :
			m{
			m00, m01, m02,
			m10, m11, m12,
			m20, m21, m22
		} {}

		mat3()noexcept = default;
#pragma endregion
#pragma region operator
		mat3& operator+=(const mat3& m)noexcept
		{
			this->m[0][0] += m.m[0][0];
			this->m[0][1] += m.m[0][1];
			this->m[0][2] += m.m[0][2];

			this->m[1][0] += m.m[1][0];
			this->m[1][1] += m.m[1][1];
			this->m[1][2] += m.m[1][2];

			this->m[2][0] += m.m[2][0];
			this->m[2][1] += m.m[2][1];
			this->m[2][2] += m.m[2][2];

			return *this;
		}
		constexpr const mat3 operator+(const mat3& m2)const noexcept
		{
			return mat3(
				m[0][0] + m2.m[0][0],
				m[0][1] + m2.m[0][1],
				m[0][2] + m2.m[0][2],

				m[1][0] + m2.m[1][0],
				m[1][1] + m2.m[1][1],
				m[1][2] + m2.m[1][2],

				m[2][0] + m2.m[2][0],
				m[2][1] + m2.m[2][1],
				m[2][2] + m2.m[2][2]
			);
		}
		mat3& operator-=(const mat3& m)noexcept
		{
			this->m[0][0] -= m.m[0][0];
			this->m[0][1] -= m.m[0][1];
			this->m[0][2] -= m.m[0][2];

			this->m[1][0] -= m.m[1][0];
			this->m[1][1] -= m.m[1][1];
			this->m[1][2] -= m.m[1][2];

			this->m[2][0] -= m.m[2][0];
			this->m[2][1] -= m.m[2][1];
			this->m[2][2] -= m.m[2][2];

			return *this;
		}
		constexpr const mat3 operator-(const mat3& m2)const noexcept
		{
			return mat3(
				m[0][0] - m2.m[0][0],
				m[0][1] - m2.m[0][1],
				m[0][2] - m2.m[0][2],

				m[1][0] - m2.m[1][0],
				m[1][1] - m2.m[1][1],
				m[1][2] - m2.m[1][2],

				m[2][0] - m2.m[2][0],
				m[2][1] - m2.m[2][1],
				m[2][2] - m2.m[2][2]
			);
		}
		mat3& operator*=(const float value)noexcept
		{
			this->m[0][0] *= value;
			this->m[0][1] *= value;
			this->m[0][2] *= value;

			this->m[1][0] *= value;
			this->m[1][1] *= value;
			this->m[1][2] *= value;

			this->m[2][0] *= value;
			this->m[2][1] *= value;
			this->m[2][2] *= value;
			return *this;
		}
		constexpr const mat3 operator*(const float value)const noexcept
		{
			return mat3(
				m[0][0] * value,
				m[0][1] * value,
				m[0][2] * value,

				m[1][0] * value,
				m[1][1] * value,
				m[1][2] * value,

				m[2][0] * value,
				m[2][1] * value,
				m[2][2] * value
			);
		}
		constexpr const mat3 operator*(const mat3& other)const noexcept
		{
			return mat3(
				m[0][0] * other.m[0][0] +
				m[0][1] * other.m[1][0] +
				m[0][2] * other.m[2][0],

				m[0][0] * other.m[0][1] +
				m[0][1] * other.m[1][1] +
				m[0][2] * other.m[2][1],

				m[0][0] * other.m[0][2] +
				m[0][1] * other.m[1][2] +
				m[0][2] * other.m[2][2],


				m[1][0] * other.m[0][0] +
				m[1][1] * other.m[1][0] +
				m[1][2] * other.m[2][0],

				m[1][0] * other.m[0][1] +
				m[1][1] * other.m[1][1] +
				m[1][2] * other.m[2][1],

				m[1][0] * other.m[0][2] +
				m[1][1] * other.m[1][2] +
				m[1][2] * other.m[2][2],


				m[2][0] * other.m[0][0] +
				m[2][1] * other.m[1][0] +
				m[2][2] * other.m[2][0],

				m[2][0] * other.m[0][1] +
				m[2][1] * other.m[1][1] +
				m[2][2] * other.m[2][1],

				m[2][0] * other.m[0][2] +
				m[2][1] * other.m[1][2] +
				m[2][2] * other.m[2][2]
			);
		}
		mat3& operator*=(const mat3& other)noexcept
		{
			*this = *this * other;
			return *this;
		}
#pragma endregion
	public:
		static const int row = 3, col = 3;
		float m[row][col] = { 0 };
	public:
		//単位マトリクス
		constexpr const mat3 Identity()const noexcept;
		//転置
		constexpr const mat3 Transpose()const noexcept;
		//行列式
		constexpr float Determinant()const noexcept;
		//逆行列
		constexpr const mat3 Inverse()const noexcept;
		//回転マトリクス
		constexpr const mat3 RotateX(float degree)const noexcept;
		constexpr const mat3 RotateY(float degree)const noexcept;
		constexpr const mat3 RotateZ(float degree)const noexcept;
		//拡大縮小マトリクス
		constexpr const mat3 Scale(const vec3& scale)const noexcept;
		constexpr const mat3 Scale(float scale)const noexcept;
	};

	struct mat4
	{
#pragma region Constructor
		mat4()noexcept = default;
		constexpr mat4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		) noexcept :
			m{
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33
		}
		{}
#pragma endregion
#pragma region operator
		mat4& operator+=(const mat4& m)noexcept
		{
			this->m[0][0] += m.m[0][0];
			this->m[0][1] += m.m[0][1];
			this->m[0][2] += m.m[0][2];
			this->m[0][3] += m.m[0][3];

			this->m[1][0] += m.m[1][0];
			this->m[1][1] += m.m[1][1];
			this->m[1][2] += m.m[1][2];
			this->m[1][3] += m.m[1][3];

			this->m[2][0] += m.m[2][0];
			this->m[2][1] += m.m[2][1];
			this->m[2][2] += m.m[2][2];
			this->m[2][3] += m.m[2][3];

			this->m[3][0] += m.m[3][0];
			this->m[3][1] += m.m[3][1];
			this->m[3][2] += m.m[3][2];
			this->m[3][3] += m.m[3][3];
			return *this;
		}
		constexpr const mat4 operator+(const mat4& m2)const noexcept
		{
			return mat4(
				m[0][0] + m2.m[0][0],
				m[0][1] + m2.m[0][1],
				m[0][2] + m2.m[0][2],
				m[0][3] + m2.m[0][3],

				m[1][0] + m2.m[1][0],
				m[1][1] + m2.m[1][1],
				m[1][2] + m2.m[1][2],
				m[1][3] + m2.m[1][3],

				m[2][0] + m2.m[2][0],
				m[2][1] + m2.m[2][1],
				m[2][2] + m2.m[2][2],
				m[2][3] + m2.m[2][3],

				m[3][0] + m2.m[3][0],
				m[3][1] + m2.m[3][1],
				m[3][2] + m2.m[3][2],
				m[3][3] + m2.m[3][3]);
		}
		mat4& operator-=(const mat4& m)noexcept
		{
			this->m[0][0] -= m.m[0][0];
			this->m[0][1] -= m.m[0][1];
			this->m[0][2] -= m.m[0][2];
			this->m[0][3] -= m.m[0][3];

			this->m[1][0] -= m.m[1][0];
			this->m[1][1] -= m.m[1][1];
			this->m[1][2] -= m.m[1][2];
			this->m[1][3] -= m.m[1][3];

			this->m[2][0] -= m.m[2][0];
			this->m[2][1] -= m.m[2][1];
			this->m[2][2] -= m.m[2][2];
			this->m[2][3] -= m.m[2][3];

			this->m[3][0] -= m.m[3][0];
			this->m[3][1] -= m.m[3][1];
			this->m[3][2] -= m.m[3][2];
			this->m[3][3] -= m.m[3][3];
			return *this;
		}
		constexpr const mat4 operator-(const mat4& m2)noexcept
		{
			return mat4(
				m[0][0] - m2.m[0][0],
				m[0][1] - m2.m[0][1],
				m[0][2] - m2.m[0][2],
				m[0][3] - m2.m[0][3],

				m[1][0] - m2.m[1][0],
				m[1][1] - m2.m[1][1],
				m[1][2] - m2.m[1][2],
				m[1][3] - m2.m[1][3],

				m[2][0] - m2.m[2][0],
				m[2][1] - m2.m[2][1],
				m[2][2] - m2.m[2][2],
				m[2][3] - m2.m[2][3],

				m[3][0] - m2.m[3][0],
				m[3][1] - m2.m[3][1],
				m[3][2] - m2.m[3][2],
				m[3][3] - m2.m[3][3]);
		}
		mat4& operator*=(const float value)noexcept
		{
			this->m[0][0] *= value;
			this->m[0][1] *= value;
			this->m[0][2] *= value;
			this->m[0][3] *= value;

			this->m[1][0] *= value;
			this->m[1][1] *= value;
			this->m[1][2] *= value;
			this->m[1][3] *= value;

			this->m[2][0] *= value;
			this->m[2][1] *= value;
			this->m[2][2] *= value;
			this->m[2][3] *= value;

			this->m[3][0] *= value;
			this->m[3][1] *= value;
			this->m[3][2] *= value;
			this->m[3][3] *= value;
			return *this;
		}
		constexpr const mat4 operator*(const float value)const noexcept
		{
			return mat4(
				m[0][0] * value,
				m[0][1] * value,
				m[0][2] * value,
				m[0][3] * value,

				m[1][0] * value,
				m[1][1] * value,
				m[1][2] * value,
				m[1][3] * value,

				m[2][0] * value,
				m[2][1] * value,
				m[2][2] * value,
				m[2][3] * value,

				m[3][0] * value,
				m[3][1] * value,
				m[3][2] * value,
				m[3][3] * value);
		}
		constexpr const mat4 operator*(const mat4& other)const noexcept
		{
			return mat4(
				m[0][0] * other.m[0][0] +
				m[0][1] * other.m[1][0] +
				m[0][2] * other.m[2][0] +
				m[0][3] * other.m[3][0],

				m[0][0] * other.m[0][1] +
				m[0][1] * other.m[1][1] +
				m[0][2] * other.m[2][1] +
				m[0][3] * other.m[3][1],

				m[0][0] * other.m[0][2] +
				m[0][1] * other.m[1][2] +
				m[0][2] * other.m[2][2] +
				m[0][3] * other.m[3][2],

				m[0][0] * other.m[0][3] +
				m[0][1] * other.m[1][3] +
				m[0][2] * other.m[2][3] +
				m[0][3] * other.m[3][3],


				m[1][0] * other.m[0][0] +
				m[1][1] * other.m[1][0] +
				m[1][2] * other.m[2][0] +
				m[1][3] * other.m[3][0],

				m[1][0] * other.m[0][1] +
				m[1][1] * other.m[1][1] +
				m[1][2] * other.m[2][1] +
				m[1][3] * other.m[3][1],

				m[1][0] * other.m[0][2] +
				m[1][1] * other.m[1][2] +
				m[1][2] * other.m[2][2] +
				m[1][3] * other.m[3][2],

				m[1][0] * other.m[0][3] +
				m[1][1] * other.m[1][3] +
				m[1][2] * other.m[2][3] +
				m[1][3] * other.m[3][3],


				m[2][0] * other.m[0][0] +
				m[2][1] * other.m[1][0] +
				m[2][2] * other.m[2][0] +
				m[2][3] * other.m[3][0],

				m[2][0] * other.m[0][1] +
				m[2][1] * other.m[1][1] +
				m[2][2] * other.m[2][1] +
				m[2][3] * other.m[3][1],

				m[2][0] * other.m[0][2] +
				m[2][1] * other.m[1][2] +
				m[2][2] * other.m[2][2] +
				m[2][3] * other.m[3][2],

				m[2][0] * other.m[0][3] +
				m[2][1] * other.m[1][3] +
				m[2][2] * other.m[2][3] +
				m[2][3] * other.m[3][3],


				m[3][0] * other.m[0][0] +
				m[3][1] * other.m[1][0] +
				m[3][2] * other.m[2][0] +
				m[3][3] * other.m[3][0],

				m[3][0] * other.m[0][1] +
				m[3][1] * other.m[1][1] +
				m[3][2] * other.m[2][1] +
				m[3][3] * other.m[3][1],

				m[3][0] * other.m[0][2] +
				m[3][1] * other.m[1][2] +
				m[3][2] * other.m[2][2] +
				m[3][3] * other.m[3][2],

				m[3][0] * other.m[0][3] +
				m[3][1] * other.m[1][3] +
				m[3][2] * other.m[2][3] +
				m[3][3] * other.m[3][3]
			);
		}
		mat4& operator*=(const mat4& other)noexcept
		{
			*this = *this * other;
			return *this;
		}
#pragma endregion
	public:
		static const int row = 4, col = 4;
		float m[row][col] = { 0 };
	public:
		//単位マトリクス
		constexpr const mat4 Identity()const noexcept;
		//転置
		constexpr const mat4 Transpose()const noexcept;
		//行列式
		constexpr float Determinant()const noexcept;
		//逆行列
		constexpr const mat4 Inverse()const noexcept;
		//回転マトリクス
		constexpr const mat4 RotateX(float degree)const noexcept;
		constexpr const mat4 RotateY(float degree)const noexcept;
		constexpr const mat4 RotateZ(float degree)const noexcept;
		//拡大縮小マトリクス
		constexpr const mat4 Scale(const vec4& scale)const noexcept;
		constexpr const mat4 Scale(float scale)const noexcept;
	};

#pragma endregion
	//外部オペレータ
#pragma region matoperator
	inline constexpr const mat2 operator*(float s, const mat2& m)noexcept
	{
		return m * s;
	}
	inline constexpr const mat3 operator*(float s, const mat3& m)noexcept
	{
		return m * s;
	}
	inline constexpr const mat4 operator*(float s, const mat4& m)noexcept
	{
		return m * s;
	}
#pragma endregion

#pragma endregion

#pragma region matvecCalc
	inline constexpr const vec2 operator*(const vec2& vec, const mat2& mat)
	{
		return vec2(
			vec.x * mat.m[0][0] + vec.y * mat.m[0][1],
			vec.x * mat.m[1][0] + vec.y * mat.m[1][1]
		);
	}
	inline constexpr const vec2 operator*=(vec2& vec, const mat2& mat)
	{
		vec.x = vec.x * mat.m[0][0] + vec.y * mat.m[0][1];
		vec.y = vec.x * mat.m[1][0] + vec.y * mat.m[1][1];
		return vec;
	}
	inline constexpr const vec3 operator*(const vec3& vec, const mat3& mat)
	{
		return vec3(
			vec.x * mat.m[0][0] + vec.y * mat.m[0][1] + vec.z * mat.m[0][2],
			vec.x * mat.m[1][0] + vec.y * mat.m[1][1] + vec.z * mat.m[1][2],
			vec.x * mat.m[2][0] + vec.y * mat.m[2][1] + vec.z * mat.m[2][2]
		);
	}
	inline constexpr const vec3 operator*=(vec3& vec, const mat3& mat)
	{
		vec.x = vec.x * mat.m[0][0] + vec.y * mat.m[0][1] + vec.z * mat.m[0][2];
		vec.y = vec.x * mat.m[1][0] + vec.y * mat.m[1][1] + vec.z * mat.m[1][2];
		vec.z = vec.x * mat.m[2][0] + vec.y * mat.m[2][1] + vec.z * mat.m[2][2];
		return vec;
	}
	inline constexpr const vec4 operator*(const vec4& vec, const mat4& mat)
	{
		return vec4(
			vec.x * mat.m[0][0] + vec.y * mat.m[0][1] + vec.z * mat.m[0][2] + vec.w * mat.m[0][3],
			vec.x * mat.m[1][0] + vec.y * mat.m[1][1] + vec.z * mat.m[1][2] + vec.w * mat.m[1][3],
			vec.x * mat.m[2][0] + vec.y * mat.m[2][1] + vec.z * mat.m[2][2] + vec.w * mat.m[2][3],
			vec.x * mat.m[3][0] + vec.y * mat.m[3][1] + vec.z * mat.m[3][2] + vec.w * mat.m[3][3]
		);
	}
	inline constexpr const vec4 operator*=(vec4& vec, const mat4& mat)
	{
		vec.x = vec.x * mat.m[0][0] + vec.y * mat.m[0][1] + vec.z * mat.m[0][2] + vec.w * mat.m[0][3];
		vec.y = vec.x * mat.m[1][0] + vec.y * mat.m[1][1] + vec.z * mat.m[1][2] + vec.w * mat.m[1][3];
		vec.z = vec.x * mat.m[2][0] + vec.y * mat.m[2][1] + vec.z * mat.m[2][2] + vec.w * mat.m[2][3];
		vec.w = vec.x * mat.m[3][0] + vec.y * mat.m[3][1] + vec.z * mat.m[3][2] + vec.w * mat.m[3][3];
		return vec;
	}
#pragma endregion

#pragma region GameFunction

	mat4 LookAt(vec3 position,vec3 target,vec3 up);
	vec2 Distance(float x1, float y1, float x2, float y2);
	vec2 Distance(vec2 point, vec2 target);
	float Radian2Degree(float value);
	float Degree2Radian(float value);
	vec2 MovePoint(vec2 position, float r, float distance);
	vec2 MovePoint(vec2 center, vec2 position, float r);
	float GetTheta(vec2 v1, vec2 v2);
	float GetTheta(vec3 v1, vec3 v2);
	float GetTheta(vec4 v1, vec4 v2);
	float ToAngle(vec2 vector1, vec2 vector2);
	float ToAngle(vec2 vector);
	float Angle2Vector(vec4 vector1, vec4 vector2);
	vec2 RadToVector2(float rad)noexcept;
	float Lerp(float start, float target, float value);
	vec2 Lerp(vec2 start, vec2 target, float value);
	vec3 Lerp(vec3 start, vec3 target, float value);
	vec4 Lerp(vec4 start, vec4 target, float value);
	vec2 Slerp(vec2 start, vec2 target, float value);
	vec3 Slerp(vec3 start, vec3 target, float value);
	vec4 Slerp(vec4 start, vec4 target, float value);
	float Easeing(float cave, float value ,float x = 0, float y = 0);
	mat4 Perspective(float fovY, float aspect, float zNear, float zFar);

#pragma endregion

} //!namespace MyMath

#endif 
