#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifndef ENGINE_H_INCLUDE
#define ENGINE_H_INCLUDE


//#define DEBUG

#include <Windows.h>
#include <string>
#include <random>
#include <memory>
#include "ObjectSprite.h"
#include "GLFWWindow.h"
#include "GLContext.h"
#include "Scene.h"
#include "GameObject.h"
#include "TileMap.h"
#include "Time.h"
#include "Collision.h"
#include "GLTextFont.h"

class ProgramPipeline;
using ProgramPipelinePtr = std::shared_ptr<ProgramPipeline>;
class Texture;
using TexturePtr = std::shared_ptr<Texture>;

namespace Mesh
{
	class PrimitiveBuffer;
	using PrimitiveBufferPtr = std::shared_ptr<PrimitiveBuffer>;
	class MeshBuffer;
	using MeshBufferPtr = std::shared_ptr<MeshBuffer>;
}

/*ゲームエンジン*/
class Engine
{
public:
	int Run();

	std::mt19937& GetRandomGenerator() { return rg; }
	
	/*背景スプライトの位置情報*/
	
	template<typename T>
	void SetNextScene() { nextScene = std::make_shared<T>(); }
	bool GetKey(int key)const {
		GLFW_SPACE::Window& windows = GLFW_SPACE::Window::Instance();
		return glfwGetKey(windows.GetGLWindow(), key);
	}
	TileMap LoadTileMap(const char* filename, TileMapScroll scrolldata);
	void SetSpriteList(const TileMap& tileMap,TexturePtr tex, TileMapScroll scrolldata);
	void SetBackGroundTexture(TexturePtr tex, TileMapScroll scrolldata);
	/*ゲームオブジェクトを作成する*/
	template<typename T>
	std::shared_ptr<T> Create(const std::string& name, float x = 0, float y = 0)
	{
		std::shared_ptr<T> p = std::make_shared<T>();
		p->engine = this;
		p->name = name;
		p->position.x = x;
		p->position.y = y;
		gameObjectList.push_back(p);
		return p;
	}
	void ClearGameObjectList() { gameObjectList.clear();  collitionList.clear(); }
	void CameraDebug();

	void RegisterCollitionObject(GameObjectPtr gameObject, std::shared_ptr<Box2DCollision> collision)noexcept;
	
	bool CheckCollideObject(GameObjectPtr gameObject, MyMath::vec3 vector);

	size_t AddUILayer(const char* image, GLenum filferMode, size_t reserve);

	void ClearUILayers();
	/*UI用ゲームオブジェクトを作成する
	@param layerNo 追加先のレイヤー番号
	@param name	   ゲームオブジェクトの名前
	@param x	　 ゲームオブジェクトを配置するx座標
	@param y	   ゲームオブジェクトを配置するy座標

	@return 作成したUI用ゲームオブジェト
	*/
	template<typename T>
	std::shared_ptr<T> CreateUI(size_t layerNo, const std::string& name,
		float x = 0, float y = 0) {
		if (layerNo >= uiLayerList.size()) {
			return nullptr;
		}
		std::shared_ptr<T> p = std::make_shared<T>();
		p->engine = this;
		p->name = name;
		p->position.x = x;
		p->position.y = y;
		uiLayerList[layerNo].gameObjectList.push_back(p);
		return p;
	}
	void RegisterUIObject(size_t layerNo, GameObjectPtr uiObject)
	{
		uiLayerList[layerNo].gameObjectList.push_back(uiObject);
	}

	TexturePtr FindWithTexture(std::string filename)noexcept;
	
	GameObjectPtr FindWithTagGameObject(GameObjectTag tag)noexcept;
	GameObjectList FindWithTagGameObjects(GameObjectTag tag)noexcept;
	GameObjectPtr FindWithGameObjectName(std::string name)noexcept;
	GameObjectList FindWithGameObjectsName(std::string name)noexcept;

	void BgScroll(TileMapScroll& scrolldata)noexcept;
	void CalcStartUpBgLocation(TileMapScroll& scrolldata)noexcept;
private:

	int Initialize();
	int MainLoop();
	int Finalize();

	void UpdateGameObject(float deltaTime);
	void CollideGameObject();
	void RemoveGameObject(GameObjectList& gameObjectList);
	void RemoveCollider();
	void MakeSpriteListObj(GameObjectList& gameObjectList, SpriteList& spriteList);
	void MakeSpriteListObj(GameObjectList& gameObjectList, std::vector<ObjectSprite>& spriteList);
	void DrawSpriteList(const SpriteList& spriteList,
		GLuint tex, size_t& spriteSsboOffset);

	std::string title = "FantasyWizard";
	//シェーダー
	ProgramPipelinePtr progSprite;

	//頂点バッファ
	GLuint vbo = 0; /*頂点データをバーテックスバッファにコピー*/
	GLuint ibo = 0; /*インデックスバッファの管理番号*/
	GLuint vao = 0; /*構成情報の管理番号*/
	//テキスチャ
	std::vector<TexturePtr> tex;
	//スプライト
	const size_t maxSpriteCount = 100'000;/*スプライトの最大数*/
	
	ObjectSpriteList spriteListBg;
	ObjectSpriteList spriteListObj;
	
	//SSBO
	size_t spriteSsboSize = 0;		//スプライト用SSBOのバイトサイズ
	GLuint ssboSprite = 0;			//スプライト用の管理番号
	size_t spriteSsboIndex = 0;	//コピー先バッファのインデックス
	uint8_t* pSpriteSsbo = nullptr;//スプライトデータの子イー先アドレス

	Time time;
	/*スプライト用SSBOの同期オブジェクト*/
	GLsync syncSpriteSsbo[2] = { 0,0 };/*GLsync == 同期オブジェクトの管理番号*/

	/*3Dモデル用の変数*/
	ProgramPipelinePtr progStandard3D;
	Mesh::PrimitiveBufferPtr primitiveBuffer;
	Mesh::MeshBufferPtr meshBuffer;

	ProgramPipelinePtr progAssimp3D;



	GameObjectPtr cameraObject;
	//乱数
	std::mt19937 rg;		/*疑似乱数を生成するオブジェクト*/

	ScenePtr scene;//実行中のシーン
	ScenePtr nextScene;//次のシーン

	GameObjectList gameObjectList;/*ゲームオブジェクト配列*/
	ColliderList collitionList;
private:
	/*UI用ゲームオブジェクトのレイヤー構造*/
	struct UILayer {
		TexturePtr tex;/*レイヤーの画面に使用するテクスチャ*/
		GameObjectList gameObjectList;
		std::vector<Sprite> spriteList;
	};
	std::vector<UILayer> uiLayerList;/*UIレイヤー配列*/	
};

#endif // !ENGINE_H_INCLUDE


