#pragma once
#ifndef AUDIOEVENT_H_INCLUDE
#define AUDIOEVENT_H_INCLUDE
#include "../Component.h"
#include "../EasyAudio.h"
#include <string>

/*音声再生コンポーネント*/
class AudioEvent : public Component
{
public:
	struct {
		std::string onCollision;
		std::string onTakeDamage;
		std::string onDestroy;
	}files;
public:
	AudioEvent(GameObjectPtr gameObject)
		:Component(gameObject) { name = "AudioEvent"; }

	virtual ~AudioEvent() = default;

	virtual void OnCollision( GameObject&)override {
		if (!files.onCollision.empty()) {
			Audio::PlayOneShot(files.onCollision.c_str());
		}
	}

	virtual void OnTakeDamage(GameObject& other,
		const Damage& damage)override {
		if (!files.onTakeDamage.empty())
			Audio::PlayOneShot(files.onTakeDamage.c_str());
	}

	virtual void OnDestroy()override {
		if (!files.onDestroy.empty()) {
			Audio::PlayOneShot(files.onDestroy.c_str());
		}
	}
};

#endif