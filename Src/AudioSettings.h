#pragma once
#ifndef AUDIOSETTINGS_H_INCLUDE
#define AUDIOSETTINGS_H_INCLUDE

/*音声プレイヤー番号*/
namespace AudioPlayer {
	inline constexpr int bgm = 0;/*再生に使うプレイヤー番号*/

	namespace BGM {
		inline constexpr char title[] = "Res/Audio/title.wav";
		inline constexpr char stage1[] = "Res/Audio/stage1.wav";
		inline constexpr char stage1Boss[] = "Res/Audio/stage1Boss.wav";
		inline constexpr char clear[] = "Res/Audio/clear.wav";
		inline constexpr char gameOver[] = "Res/Audio/gammeover.wav";
	}

	namespace SE {
		inline constexpr char playerShot[]			= "Res/Audio/????";
		inline constexpr char playerExplosion[]		= "Res/Audio/Explosion1.wav";
		inline constexpr char enemyExplosionS[]		= "Res/Audio/Explosion2.wav";
		inline constexpr char enemyExplosionM[]		= "Res/Audio/Explosion2.wav";
		inline constexpr char enemyExplosionL[]		= "Res/Audio/Explosion2.wav";
		inline constexpr char gageUp[]				= "Res/Audio/gage.wav";
		inline constexpr char cure[]				= "Res/Audio/cure.wav";
		inline constexpr char select[]				= "Res/Audio/select.wav";
		inline constexpr char enter[]				= "Res/Audio/enter.wav";
		inline constexpr char laser[]				= "Res/Audio/laser.wav";
	}
}

#endif // !AUDIOSETTINGS_H_INCLUDE
