#pragma once
#ifndef TIME_H_INCLUDE
#define TIME_H_INCLUDE
#include <iostream>
#include <sstream>
#include <windows.h>
#include "GLFW/glfw3.h"

//時間
//inline float SECOND(float _sec, float _idealTime) { return _sec * _idealTime; }

struct GameTime
{
	float totalTime = 0;
	int second = 0, minute = 0, hour = 0;
	void Calc()
	{
		//小数点以下の切り捨て
		int nowSecond = std::floor(totalTime / 1000);
		second = nowSecond % 60;
		minute = (nowSecond / 60) % 60;		
		hour = (nowSecond / 3600);
	}
	std::string GetTimeString()
	{
		std::string r;
		std::to_string(hour);
		if (hour < 10)
		{
			r += std::to_string(0);
		}
		r += std::to_string(hour); r += ":";
		if (minute < 10)
		{
			r += std::to_string(0);
		}
		r += std::to_string(minute); r += ":";
		if (second < 10)
		{
			r += std::to_string(0);
		}
		r += std::to_string(second);
		return r;
	}
};

class Time
{
	//現在のfpsを格納する
	float fps = 0;
	//フレーム回数のカウント
	int frameCount = 0;
	//指定fps回数
	float idealCount = 60;
	//プログラム開始時間
	float startTime = 0;
	//現実時間換算を格納
	GameTime time;
	//フレームの開始時間
	float frameStartTime = 0;
	//フレーム時間
	float frameTime = 0;
	//理想のフレーム時間
	int idealTime = 0;
	//前回のフレーム時間
	float frameTimeOld = 0;
	//前回と今回のフレームの差
	float progressTime = 0;

private:	
	//最初にだけ行う処理
	void Initialize() {
		startTime = glfwGetTime() * 1000;
		frameTime = glfwGetTime() * 1000;
		frameTimeOld = glfwGetTime() * 1000;
	}

public:
	//fpsによる処理の変化を抑えるための物
	float deltaTime = 0;

public:

	Time() { Initialize(); };
	virtual ~Time() = default;

	//更新処理
	void Execute() {
		//1000　＝　1秒
		//かかった時間を持つ
		frameTime = glfwGetTime() * 1000;
		//開始してから今までの時間を格納
		time.totalTime = frameTime - startTime;
		//かかった秒数を現実時間で計算
		time.Calc();
		
		//今のフレームでかかった時間を入れる
		progressTime = frameTime - frameStartTime;
		//指定のfps回数で行けるように次のフレームでの理想時間を取得
		idealTime = frameCount * (1000.0f / idealCount);

		//指定のfps回数で今までかかったfpsを摘出し、
		//次のフレームに移るため現在までの時間を次のフレーム開始に入れる
		frameCount++;
		if (frameCount == idealCount)
		{
			fps = 1000.0f / (progressTime / idealCount);
			frameCount = 0;
			frameStartTime = frameTime;
		}

		//前回と今回のフレームの差をm秒でいれdeltaを格納
		deltaTime = static_cast<float>(frameTime - frameTimeOld) / 1000.0f;	
		frameTimeOld = frameTime;		
	}

	//fpsを指定の物にするため止まらせる処理
	void Wait()
	{
		if (idealTime > progressTime)
		{
			Sleep(idealTime - progressTime);
		}
	}

#pragma region GetterSetter
	
	void SetFPS(float value)
	{
		idealCount = value;
		frameCount = 0;
		frameTime = 1 / idealCount;
	}
	float GetFPS() const
	{
		return fps;
	}

	float GetTotalSecond()
	{
		return std::floor(time.totalTime / 1000);
	}

	int GetSecond()
	{
		return time.second;
	}
	int GetMinute()
	{
		return time.minute;
	}
	int GetHour() {
		return time.hour;
	}
	
	/*
	* @return プログラム開始時から今までかかった時間を現実換算で返す
	*/
	std::string GetTotalTimeString()
	{
		return time.GetTimeString();
	}

#pragma endregion

};

#endif // !TIME_H_INCLUDE
