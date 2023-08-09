#pragma once
#include <unordered_map>
#include <memory>
using namespace std;

enum class StatusPramID
{
	HP,			//Hit Point ‘Ì—Í
	MP,			//Magic Point –‚—Í
	ATK,		//Attack UŒ‚—Í
	STR,		//Strength •¨—UŒ‚—Í@—Í‚Ì‹­‚³
	MAT,		//Magic Attack –‚–@UŒ‚—Í
	DEF,		//Defense •¨—–hŒä—Í
	MDF,		//Magic Defense –‚–@–hŒä—Í
	VIT,		//Vitality ¶–½—Í
	INT,		//Intelligence ’m—Í@–‚–@UŒ‚—Í‚É‰e‹¿
	RES,		//Resist ’ïR—Í
	DEX,		//Dexterity Ší—p‚³B–½’†—¦‚É‰e‹¿‚ğ—^‚¦‚é
	AGI,		//Agility ‘f‘‚³
	LUK,		//LUK ‰^
	SP,			//Skill Point ƒXƒLƒ‹ƒ|ƒCƒ“ƒg
};
class Status
{
	//ƒpƒ‰ƒ[ƒ^
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