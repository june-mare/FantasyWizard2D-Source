#ifndef SCENE_H_INCLUDE
#define SCENE_H_INCLUDE

#include <memory>
#include "TileMapScroll.h"

/*��s�錾*/
class Engine;
class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;

/*�V�[���̊��N���X*/
class Scene
{
public:
	//std::vector<TexturePtr> textureList;

	Scene() = default;
	virtual ~Scene() = default;
	virtual bool Initialize(Engine& engine) { return true; }
	virtual void Execute(Engine& engine, float deltaTime) = 0;
	virtual void Finalize(Engine& engine) {}

};

using ScenePtr = std::shared_ptr<Scene>;

#endif // !SCENE_H_INCLUDE
