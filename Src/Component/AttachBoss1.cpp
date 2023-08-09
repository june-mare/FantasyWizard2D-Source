#include "AttachBoss1.h"

void AttachBoss1::Start()
{
	//Šî–{ˆ—
	{
		target = gameObject->engine->FindWithGameObjectName("player");
		bullet = gameObject->AddComponent<BulletWays>();
		gameObject->layer = prioEnemy;
		gameObject->tag = GameObjectTag::enemy;
		auto vx = target->position.x - gameObject->position.x;
		auto vy = target->position.y - gameObject->position.y;
		if (vx > 0)vec = side::right;
		else vec = side::left;
		if (vy > 0)vec = side::down;
		else vec = side::up;
		bullet->move.moveVector = MyMath::vec3(vx, vy, 0);
	}

	auto tcList = CharacterTCList<Boss1>();
	//‰æ‘œˆ—
	{
		gameObject->AddSpriteTexture(tcList[0],
			gameObject->engine->FindWithTexture("Img/objects.tga"));
		auto anim = gameObject->AddComponent<Animator2D>();
		anim->clip.resize(tcList.size());
		for (int i = 0; i < tcList.size(); i++)
		{
			anim->AddSpriteTexture(i, tcList[i], 1.5);
		}
		anim->interval = 0.2f;
		anim->loopTime = true;
	}

	float collisionSize = (IMG_OBJECTS_SIZE * 0.4f);
	auto collition = gameObject->AddComponent<Box2DCollision>();
	collition->AddCollision({ -tcList[0].sx * collisionSize, -tcList[0].sy * collisionSize,
		tcList[0].sx * collisionSize, tcList[0].sy * collisionSize
		});

	health = gameObject->AddComponent<Health>();
	health->health = 250;

	auto explosion = gameObject->AddComponent<Explosion>();
	explosion->count = 2;

	auto audio = gameObject->AddComponent<AudioEvent>();
	audio->files.onTakeDamage = AudioPlayer::SE::enemyExplosionS;
	
	auto damage = gameObject->AddComponent<DamageSource>();
	damage->targetName.push_back("player");
	damage->isOnce = false; damage->interval = 1.0f; damage->damageInfo.amount = 3;

	move.speed = 100.0f;
	move.acc = 50.0f;
	move.accIncrease = 0.5f;
	move.moveVector.x = -1;
	Reload();
}

void AttachBoss1::Update(float deltaTime)
{
	if (sequenceTiemmr < 0) {
		oldSequence = nowSequence;
		if (isHerf)nowSequence = rand() % 10;
		else nowSequence = rand() % 5;
				
		if (nowSequence >= 7)
		{
			shotInterval = 0.3;
		}
		sequenceTiemmr = sequenceInterval;
		shotTimer = shotInterval;
		count = 5;
	}

	if (shotTimer <= 0) {
		GameObjectPtr object;
		std::shared_ptr<EndSpawn> magic;

		auto vx = target->position.x - gameObject->position.x;
		auto vy = target->position.y - gameObject->position.y;
		bool verOrHori = abs(vx) < abs(vy) ? true : false;
		if (vx > 0 && !verOrHori)
		{
			vec = side::right;
		}
		else if (vx < 0 && !verOrHori)vec = side::left;
		if (vy > 0 && verOrHori)vec = side::up;
		else if (vy < 0 && verOrHori)vec = side::down;
		
		switch (nowSequence)
		{
		case 4:
			if (count == 5)
			{
				object = gameObject->engine->Create<GameObject>("bullet", 640, 365);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::left;
				magic->startEffectType = 16; magic->size = 1.5;
				magic->type = 4; magic->spawnType = bulletspawn;
				magic->shotInterval = 0.3;
				magic->move.accIncrease = 2; magic->move.speed = 90;
			}
			if (count == 1)
			{

				object = gameObject->engine->Create<GameObject>("bullet", 1240, 365);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::left;
				magic->startEffectType = 16; magic->size = 1.5;
				magic->type = 4; magic->spawnType = bulletspawn;
				magic->shotInterval = 0.3;
				magic->move.accIncrease = 2; magic->move.speed = 90;

			}
			if (count == 1)
			{
				object = gameObject->engine->Create<GameObject>("bullet", 40, 365);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::left;
				magic->startEffectType = 16; magic->size = 1.5;
				magic->type = 4; magic->spawnType = bulletspawn;
				magic->shotInterval = 0.3;
				magic->move.accIncrease = 2; magic->move.speed = 90;
			}
			break;
		case 5:
			for (int j = 0; j < 3; j++)
			{
				auto data = gameObject->engine->Create<GameObject>("object",
					304 + (j * 304),
					360)
					->AddComponent<EndSpawn>();
				data->startEffectType = 15; data->spawnType = enemyspawn;
				data->size = 1.5; data->type = 2; data->randomRecovery = true; data->randomWeapon = true;
			}
			nowSequence = 1;
			break;
		case 6:
			for (int j = 0; j < 18; j++)
			{
				auto data = gameObject->engine->Create<GameObject>("object",
					20 + (j * 80),
					360)
					->AddComponent<EndSpawn>();
				data->startEffectType = 15; data->spawnType = objectspawn;
				data->size = 1.5; data->life = 5; data->randomWeapon = true; data->randomRecovery = true;
			}
			nowSequence = 9;
			break;
		case 7:
		case 8:
		case 9:
		{
			if (count == 5) {
				object = gameObject->engine->Create<GameObject>("bullet", 1250, target->position.y);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::left;
				magic->startEffectType = 15; magic->size = 4;
				magic->type = 3; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", 1250, target->position.y);
				auto a = object->AddComponent<LaserOutLineBig>();
				a->vec = magic->vec;
			}
			if (count == 4) {
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 30);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::up;
				magic->startEffectType = 15; magic->size = 4;
				magic->type = 3; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 30);
				auto a = object->AddComponent<LaserOutLineBig>();
				a->vec = magic->vec;
			}
			if (count == 3) {
				object = gameObject->engine->Create<GameObject>("bullet", 30, target->position.y);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::right;
				magic->startEffectType = 15; magic->size = 4;
				magic->type = 3; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", 30, target->position.y);
				auto a = object->AddComponent<LaserOutLineBig>();
				a->vec = magic->vec;
			}
			if (count == 2) {
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 740);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::down;
				magic->startEffectType = 15; magic->size = 4;
				magic->type = 3; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 740);
				auto a = object->AddComponent<LaserOutLineBig>();
				a->vec = magic->vec;
			}
			if (count == 1) {
				object = gameObject->engine->Create<GameObject>("bullet", 30, target->position.y);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::right;
				magic->startEffectType = 15; magic->size = 4;
				magic->type = 3; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", 30, target->position.y);
				auto a = object->AddComponent<LaserOutLineBig>();
				a->vec = magic->vec;
			}
			if (count == 0) {
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 30);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::up;
				magic->startEffectType = 15; magic->size = 4;
				magic->type = 3; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 30);
				auto a = object->AddComponent<LaserOutLineBig>();
				a->vec = magic->vec;
			}
			if (count == -1) {
				object = gameObject->engine->Create<GameObject>("bullet", 30, target->position.y);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::right;
				magic->startEffectType = 15; magic->size = 4;
				magic->type = 3; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", 30, target->position.y);
				auto a = object->AddComponent<LaserOutLineBig>();
				a->vec = magic->vec;
			}
			if (count == -2) {
				object = gameObject->engine->Create<GameObject>("bullet", 1250, target->position.y);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::left;
				magic->startEffectType = 15; magic->size = 4;
				magic->type = 3; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", 1250, target->position.y);
				auto a = object->AddComponent<LaserOutLineBig>();
				a->vec = magic->vec;
			}
			if (count == -2) {
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 30);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::up;
				magic->startEffectType = 15; magic->size = 4;
				magic->type = 3; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 30);
				auto a = object->AddComponent<LaserOutLineBig>();
				a->vec = magic->vec;
			}
			if (count == -2) {
				object = gameObject->engine->Create<GameObject>("bullet", 30, target->position.y);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::right;
				magic->startEffectType = 15; magic->size = 4;
				magic->type = 3; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", 30, target->position.y);
				auto a = object->AddComponent<LaserOutLineBig>();
				a->vec = magic->vec;
			}
			if (count == -2) {
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 740);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::down;
				magic->startEffectType = 15; magic->size = 4;
				magic->type = 3; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 740);
				auto a = object->AddComponent<LaserOutLineBig>();
				a->vec = magic->vec;
			}
		}
		break;
		case 0:
			bullet->move.moveVector = MyMath::vec3(vx,vy,0);
			bullet->move.speed = 250;
			bullet->type = 0;
			bullet->count = 10;
			bullet->color = 5;
			bullet->vec = vec;
			bullet->EnemyBullet();
			bullet->AngleSet();
			bullet->Spawn();
			break;
		case 1:
		case 2:
		case 3:
		{
			if (count == 1) {
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 740);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::down;
				magic->startEffectType = 15; magic->size = 4;
				magic->type = 3; magic->spawnType = bulletspawn;				
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 740);
				auto a = object->AddComponent<LaserOutLineBig>();
				a->vec = magic->vec;
			}
			if (count == 2) {
				object = gameObject->engine->Create<GameObject>("bullet", 30, target->position.y);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::right;
				magic->startEffectType = 17; magic->size = 2;
				magic->type = 2; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", 30, target->position.y);
				auto a = object->AddComponent<LaserOutLineSmall>();
				a->vec = magic->vec;
			}
			if (count == 3) {
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 30);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::up;
				magic->startEffectType = 17; magic->size = 2;
				magic->type = 2; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", target->position.x, 30);
				auto a = object->AddComponent<LaserOutLineSmall>();
				a->vec = magic->vec;
			}
			if (count == 4) {
				object = gameObject->engine->Create<GameObject>("bullet", 30, target->position.y);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::right;
				magic->startEffectType = 17; magic->size = 2;
				magic->type = 2; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", 30, target->position.y);
				auto a = object->AddComponent<LaserOutLineSmall>();
				a->vec = magic->vec;
			}
			if (count == 5) {
				object = gameObject->engine->Create<GameObject>("bullet", 1250, target->position.y);
				magic = object->AddComponent<EndSpawn>();
				magic->vec = side::left;
				magic->startEffectType = 17; magic->size = 2;
				magic->type = 2; magic->spawnType = bulletspawn;
				object = gameObject->engine->Create<GameObject>("bullet", 1250, target->position.y);
				auto a = object->AddComponent<LaserOutLineSmall>();
				a->vec = magic->vec;
			}
		}
		break;
		}		
		count--;
		shotTimer = shotInterval;
	}
	sequenceTiemmr -= deltaTime;
	shotTimer -= deltaTime;
	if (gameObject->position.x > 1200)
	{
		move.moveVector.x = -1;
	}
	else if (gameObject->position.x < 80)
	{
		move.moveVector.x = 1;
	}

	move.Acceleration();
	gameObject->position += move.IncreasePos(deltaTime);

	if (health->health <= 125 && !isHerf)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				auto data = gameObject->engine->Create<GameObject>("object",
					426 + (j * 426),
					48 + (i * 96))
					->AddComponent<EndSpawn>();
				data->startEffectType = 15; data->spawnType = objectspawn;
				data->size = 1.5; data->life = 20; data->randomWeapon = true; data->randomRecovery = true;
			}
		}
		isHerf = true;
	}
}
