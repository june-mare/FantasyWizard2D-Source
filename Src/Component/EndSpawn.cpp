#include "EndSpawn.h"

void EndSpawn::Start()
{
	if (isStartUp) {
		std::vector<Texcoord> tcList;
		switch (startEffectType)
		{
		case 0:
			tcList = EffectTCList<Effect1>();
			break;
		case 1:
			tcList = EffectTCList<Effect2>();
			break;
		case 2:
			tcList = EffectTCList<Effect3>();
			break;
		case 3:
			tcList = EffectTCList<Effect4>();
			break;
		case 4:
			tcList = EffectTCList<Effect5>();
			break;
		case 5:
			tcList = EffectTCList<Effect6>();
			break;
		case 6:
			tcList = EffectTCList<Effect7>();
			break;
		case 7:
			tcList = EffectTCList<Effect8>();
			break;
		case 8:
			tcList = EffectTCList<Effect9>();
			break;
		case 9:
			tcList = EffectTCList<Effect10>();
			break;
		case 10:
			tcList = EffectTCList<Effect11>();
			break;
		case 11:
			tcList = EffectTCList<Effect12>();
			break;
		case 12:
			tcList = EffectTCList<Effect13>();
			break;
		case 13:
			tcList = EffectTCList<Effect14>();
			break;
		case 14:
			tcList = EffectTCList<Effect15>();
			break;
		case 15:
			tcList = EffectTCList<Effect16>();
			break;
		case 16:
			tcList = EffectTCList<Effect17>();
			break;
		case 17:
			tcList = EffectTCList<Effect18>();
			break;
		case 18:
			tcList = EffectTCList<Effect19>();
			break;

		default:break;
		}

		if (startEffectType < 15) {
			gameObject->AddSpriteTexture(tcList[0],
				gameObject->engine->FindWithTexture("Img/objects.tga"));
		}
		else
		{
			gameObject->AddSpriteTexture(tcList[0],
				gameObject->engine->FindWithTexture("Img/Effects.tga"));
		}

		auto anim = gameObject->AddComponent<Animator2D>();
		anim->killWhenEnd = true;
		anim->loopTime = false;
		anim->interval = 0.05f;
		anim->clip.resize(tcList.size());
		for (int i = 0; i < tcList.size(); i++)
		{
			anim->AddSpriteTexture(i, tcList[i], size);
		}
	}
	else
	{

	}
}

void EndSpawn::OnDestroy()
{
	if (spawnType == SpawnType::enemyspawn) {
		auto enemy = gameObject->engine->Create<GameObject>
			("enemy", gameObject->position.x, gameObject->position.y);
		int a = rand() % 6;
		if (randomRecovery)
		{
			if (a == 0)
			{
				auto data = enemy->AddComponent<EndSpawn>();
				data->spawnType = recoveryspawn; data->isStartUp = false;
				data->size = 2;
			}
		}
		if (randomWeapon)
		{
			if (a == 1)
			{
				auto data = enemy->AddComponent<EndSpawn>();
				data->spawnType = weaponspawn; data->isStartUp = false;
				if (scrolldata != nullptr) {
					data->scrolldata = scrolldata->shared_from_this();
				}
			}
		}
		switch (type)
		{
		case 0:
			enemy->AddComponent<AttachEnemy1>();
			break;
		case 1:
			enemy->AddComponent<AttachEnemy2>();
			break;
		case 2:
			enemy->AddComponent<AttachEnemy3>();
			break;
		case 3:
			enemy->AddComponent<AttachEnemy4>();
			break;
		case 4:
			enemy->AddComponent<AttachEnemy5>();
			break;
		case 5:
			enemy->AddComponent<AttachBoss1>();
			break;
		case 6:
			enemy->AddComponent<AttachEnemy1>();
			break;
		case 7:
			enemy->AddComponent<AttachEnemy1>();
			break;
		case 8:
			enemy->AddComponent<AttachEnemy1>();
			break;
		case 9:
			enemy->AddComponent<AttachEnemy1>();
			break;
		case 10:
			enemy->AddComponent<AttachEnemy1>();
			break;
		case 11:
			enemy->AddComponent<AttachEnemy1>();
			break;
		case 12:
			enemy->AddComponent<AttachEnemy1>();
			break;
		case 13:
			enemy->AddComponent<AttachEnemy1>();
			break;
		case 14:
			enemy->AddComponent<AttachEnemy1>();
			break;
		case 15:
			enemy->AddComponent<AttachEnemy1>();
			break;
		}
		
	}
	if (spawnType == SpawnType::recoveryspawn)
	{
		auto object = gameObject->engine->Create<GameObject>("recovery",
			gameObject->position.x, gameObject->position.y);
		if (scrolldata != nullptr) {
			auto scroll = object->AddComponent<ScrollObject>();
			scroll->scrolldata = scrolldata->shared_from_this();
		}
		object->AddComponent<Recovery>();
	}
	if (spawnType == SpawnType::objectspawn)
	{
		auto object = gameObject->engine->Create<GameObject>("object",
			gameObject->position.x, gameObject->position.y);
		int a = rand() % 10;
		if (randomRecovery)
		{
			if (a == 0)
			{
				auto data = object->AddComponent<EndSpawn>();
				data->spawnType = recoveryspawn;
				data->size = 2; data->isStartUp = false;
				if (scrolldata != nullptr) {
					data->scrolldata = scrolldata->shared_from_this();
				}
			}
		}
		if (randomWeapon)
		{
			if (a == 1)
			{
				auto data = object->AddComponent<EndSpawn>();
				data->spawnType = weaponspawn;	data->isStartUp = false;
				if (scrolldata != nullptr) {
					data->scrolldata = scrolldata->shared_from_this();
				}
			}
		}
		if (scrolldata != nullptr) {
			auto scroll = object->AddComponent<ScrollObject>();
			scroll->scrolldata = scrolldata->shared_from_this();
		}
		object->AddComponent<WollObject>()->life = life;
	}
	if (spawnType == SpawnType::bulletspawn)
	{
		auto object = gameObject->engine->Create<GameObject>("bullet",
			gameObject->position.x, gameObject->position.y);
		auto magic = object->AddComponent<BulletMagic>();
		magic->type = type; magic->activityTime = activityTime;
		magic->enemyMagic = enemyMagic; magic->shotInterval = shotInterval;
		magic->vec = vec; magic->move = move;
	}
	if (spawnType == SpawnType::weaponspawn)
	{
		auto object = gameObject->engine->Create<GameObject>("weapon",
			gameObject->position.x, gameObject->position.y);
		if (scrolldata != nullptr) {
			auto scroll = object->AddComponent<ScrollObject>();
			scroll->scrolldata = scrolldata->shared_from_this();
		}
		object->AddComponent<Weapon>();
	}
}
