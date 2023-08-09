#pragma once
#ifndef ANIMATOR2D_H_INCLUDE
#define ANIMATOR2D_H_INCLUDE
#include "../Component.h"
#include "../GameObject.h"
#include "../Sprite.h"
#include <vector>
#include <algorithm>



/*2D�A�j���[�V�����R���|�[�l���g*/
class Animator2D :public Component
{
public:
	bool loopTime = true;	//���[�v�Đ��̗L��
	bool killWhenEnd = false;//�A�j���I�����ɃQ�[���I�u�W�F�N�g���폜
	int startCount = 0;
	int endCount = 0;
	float timer = 0;//�o�ߎ���
	float interval = 0.1f;//�t���[����i�߂�Ԋu
	std::vector<SpriteList> clip;//�A�j���[�V�����N���b�v
public:
	Animator2D(GameObjectPtr gameObject)
		:Component(gameObject) { name = "Animator2D"; };
	virtual ~Animator2D() = default;


	virtual void Update(float deltaTime)override
	{
		if (clip.empty()) {
			return;
		}

		//�o�ߎ��Ԃ��X�V
		timer += deltaTime;
		if (endCount != 0)
		{
			const int size = static_cast<int>(endCount - startCount);
			int i = static_cast<int>(timer / interval);
			if (loopTime)
			{
				i %= size;//���[�v�Đ�
				/*end()�̂Ƃ���܂Ő}��̂ŃA�j���[�V��������+1�̃T�C�Y��p�ӂł���*/
			}
			else
			{
				/*�A�j���I����killWhenEnd�t���O��true�Ȃ�Q�[���I�u�W�F�N�g���폜*/
				if (i >= size && killWhenEnd)
				{
					gameObject->isDead = true;
				}
				i = std::clamp(i, 0, size - 1);
			}
			i += startCount;
			/*�X�v���C�g�f�[�^���Đ��ʒu�̃f�[�^�ɍX�V*/
			gameObject->spriteList.begin()->spriteList = clip[i];
		}
		else 
		{
			//�Đ��ʒu���v�Z
			const int size = static_cast<int>(clip.size());
			int i = static_cast<int>(timer / interval);
			if (loopTime)
			{
				i %= size;//���[�v�Đ�
				/*end()�̂Ƃ���܂Ő}��̂ŃA�j���[�V��������+1�̃T�C�Y��p�ӂł���*/
			}
			else
			{
				/*�A�j���I����killWhenEnd�t���O��true�Ȃ�Q�[���I�u�W�F�N�g���폜*/
				if (i >= size && killWhenEnd)
				{
					gameObject->isDead = true;
				}
				i = std::clamp(i, 0, size - 1);
			}
			/*�X�v���C�g�f�[�^���Đ��ʒu�̃f�[�^�ɍX�V*/
			gameObject->spriteList.begin()->spriteList = clip[i];
		}

		if (timer > 888)timer = 0;
	}

	void AddSpriteTexture(size_t flameNo, const Texcoord& tc,
		float scale = 1, float x = 0, float y = 0, float rotation = 0) {
		if (flameNo >= clip.size()) {
			clip.resize(flameNo);
		}
		clip[flameNo].push_back({ x,y,0,0,tc });
		clip[flameNo].back().scale = scale;
		clip[flameNo].back().angle = rotation;
	}
	
	void AddSpriteTexture(size_t flameNo, const Texcoord& tc,void* a,
		 float x = 0, float y = 0,float scalex = 1,float scaley = 1, float rotation = 0) {
		if (flameNo >= clip.size()) {
			clip.resize(flameNo);
		}
		clip[flameNo].push_back({ x,y,0,0,tc });
		clip[flameNo].back().scaleX = scalex;
		clip[flameNo].back().scaleY = scaley;
		clip[flameNo].back().angle = rotation;
	}

	void Debug()override
	{
		
		if (ImGui::CollapsingHeader(name.c_str()))
		{
			ImGui::BulletText("loopTime", &loopTime);
			ImGui::BulletText("killWhenEnd", &killWhenEnd);

			ImGui::DragFloat("timer", &timer);
			ImGui::DragFloat("interval", &interval);
			int a = 0; char* c;
		}
		
	}
};

#endif // !ANIMATOR2D_H_INCLUDE