#pragma once
#ifndef TIME_H_INCLUDE
#define TIME_H_INCLUDE
#include <iostream>
#include <sstream>
#include <windows.h>
#include "GLFW/glfw3.h"

//����
//inline float SECOND(float _sec, float _idealTime) { return _sec * _idealTime; }

struct GameTime
{
	float totalTime = 0;
	int second = 0, minute = 0, hour = 0;
	void Calc()
	{
		//�����_�ȉ��̐؂�̂�
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
	//���݂�fps���i�[����
	float fps = 0;
	//�t���[���񐔂̃J�E���g
	int frameCount = 0;
	//�w��fps��
	float idealCount = 60;
	//�v���O�����J�n����
	float startTime = 0;
	//�������Ԋ��Z���i�[
	GameTime time;
	//�t���[���̊J�n����
	float frameStartTime = 0;
	//�t���[������
	float frameTime = 0;
	//���z�̃t���[������
	int idealTime = 0;
	//�O��̃t���[������
	float frameTimeOld = 0;
	//�O��ƍ���̃t���[���̍�
	float progressTime = 0;

private:	
	//�ŏ��ɂ����s������
	void Initialize() {
		startTime = glfwGetTime() * 1000;
		frameTime = glfwGetTime() * 1000;
		frameTimeOld = glfwGetTime() * 1000;
	}

public:
	//fps�ɂ�鏈���̕ω���}���邽�߂̕�
	float deltaTime = 0;

public:

	Time() { Initialize(); };
	virtual ~Time() = default;

	//�X�V����
	void Execute() {
		//1000�@���@1�b
		//�����������Ԃ�����
		frameTime = glfwGetTime() * 1000;
		//�J�n���Ă��獡�܂ł̎��Ԃ��i�[
		time.totalTime = frameTime - startTime;
		//���������b�����������ԂŌv�Z
		time.Calc();
		
		//���̃t���[���ł����������Ԃ�����
		progressTime = frameTime - frameStartTime;
		//�w���fps�񐔂ōs����悤�Ɏ��̃t���[���ł̗��z���Ԃ��擾
		idealTime = frameCount * (1000.0f / idealCount);

		//�w���fps�񐔂ō��܂ł�������fps��E�o���A
		//���̃t���[���Ɉڂ邽�ߌ��݂܂ł̎��Ԃ����̃t���[���J�n�ɓ����
		frameCount++;
		if (frameCount == idealCount)
		{
			fps = 1000.0f / (progressTime / idealCount);
			frameCount = 0;
			frameStartTime = frameTime;
		}

		//�O��ƍ���̃t���[���̍���m�b�ł���delta���i�[
		deltaTime = static_cast<float>(frameTime - frameTimeOld) / 1000.0f;	
		frameTimeOld = frameTime;		
	}

	//fps���w��̕��ɂ��邽�ߎ~�܂点�鏈��
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
	* @return �v���O�����J�n�����獡�܂ł����������Ԃ��������Z�ŕԂ�
	*/
	std::string GetTotalTimeString()
	{
		return time.GetTimeString();
	}

#pragma endregion

};

#endif // !TIME_H_INCLUDE
