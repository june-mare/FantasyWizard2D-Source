#pragma once
#include <unordered_map>
#include <memory>
using namespace std;

enum class StatusPramID
{
	HP,			//Hit Point �̗�
	MP,			//Magic Point ����
	ATK,		//Attack �U����
	STR,		//Strength �����U���́@�͂̋���
	MAT,		//Magic Attack ���@�U����
	DEF,		//Defense �����h���
	MDF,		//Magic Defense ���@�h���
	VIT,		//Vitality ������
	INT,		//Intelligence �m�́@���@�U���͂ɉe��
	RES,		//Resist ��R��
	DEX,		//Dexterity ��p���B�������ɉe����^����
	AGI,		//Agility �f����
	LUK,		//LUK �^
	SP,			//Skill Point �X�L���|�C���g
};
class Status
{
	//�p�����[�^
	unordered_map<StatusPramID, int>	intpram;
	unordered_map<StatusPramID, float>	floatpram;
public:
	Status(){}
	virtual ~Status() { intpram.clear(); floatpram.clear(); }
	
	void Set(StatusPramID id,const int pram)
	{
		intpram[id] = pram;
	}
	void Set(StatusPramID id,const float pram)
	{
		floatpram[id] = pram;
	}
	

	float Get(StatusPramID id)
	{
		if (intpram.find(id) != intpram.end())
		{
			return (int)intpram[id];
		}
		if (floatpram.find(id) != floatpram.end())
			return floatpram[id];
		return -999.0f;
	}
};
using StatusPtr = shared_ptr<Status>;