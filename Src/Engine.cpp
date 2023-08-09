#include "Engine.h"
#include "SceneInclude.h"
#include "EasyAudio.h"
#include "ProgramPipeline.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Meth.h"
#include "Component/Camera.h"
#include "Component/Box2DCollision.h"
#include "Rect.h"
#include "Component/FadeController.h"
#include "GLTextFont.h"





#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_glfw.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#define MODEL2D
//#define MODEL3D
//#define MODELASSIMP


#ifdef MODELASSIMP
#include "Assimp/AssimpModel.h"
#include "Assimp/AssimpCamera.h"
#endif // MODELASSIMP


//ファイバーについても考えてみる

/*SSBOのサイズを計算する*/
size_t CalcSsboSize(size_t n)
{
	/*
	n異常かつ256の倍数を返す
	OpenGLではSSBOのアライメントの最大値は256と決められている
	*/
	const size_t alignment = 256;	/*アライメント（データの境界線）*/
	return ((n + alignment - 1) / alignment) * alignment;
}

/*メッセージを処理するコールバック関数
* @param source         メッセージの発信者
* @param type           メッセージの種類
* @param id             メッセージを一位に識別する値
* @param severity       メッセージの重要度
* @param length         メッセージの文字数
* @param message        メッセージ本体
* @param userParam      コールバック設定時に指定したポインタ
*/
void GLAPIENTRY DebugCallBuck(GLenum source, GLenum type, GLuint id,
	GLenum serverity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string s;
	if (length < 0)
	{
		s = message;
	}
	else
	{
		s.assign(message, message + length);
	}
	s += '^n';
	OutputDebugString(s.c_str());
}

/*ファイルを読み込みオブジェクトがある位置などを返す*/
TileMap Engine::LoadTileMap(const char* filename, TileMapScroll scrolldata)
{
	/*ファイルを開く*/
	std::ifstream file;
	file.open(filename);
	if (!file)
	{
		return {};
	}

	TileMap tileMap;
	tileMap.data.reserve(scrolldata.sizeX * scrolldata.sizeY);/*適当な容量を予約*/

	/*ファイルを読み込む*/
	while (!file.eof())
	{
		/*1行読み込む*/
		std::string line;
		std::getline(file, line);
		if (line.size() <= 0)
		{
			continue;/*データのない行を無視*/
		}
		line.push_back(',');/*終端を追加*/

		/*カンマ区切り文字列を数値に変換*/
		tileMap.sizex = 0;
		const char* start = line.data();
		const char* end = start + line.size();
		for (const char* p = start; p != end; ++p)
		{
			if (*p == ',') {
				const int n = atoi(start);
				tileMap.data.push_back(n);
				start = p + 1;/*次のカンマ区切りを指すように更新*/
				++tileMap.sizex;
			}
		}
		++tileMap.sizey;
	}

	return tileMap;
}

/*タイルマップに登録されているデータをもとに背景を作成する*/
void Engine::SetSpriteList(const TileMap& tileMap, TexturePtr tex, TileMapScroll scrolldata)
{
	/*背景スプライトを削除*/
	spriteListBg.clear();

	scrolldata.baseX = 16;
	scrolldata.baseY = 16;
	spriteListBg.push_back(tex);
	/*配置データを参照して背景スプライトを設定*/
	/*画面サイズ = 512x512,タイルサイズ = 32x32*/
	for (size_t y = tileMap.sizey; y > 0;)
	{
		--y;/*マイナスにならないようにここで減算*/
		for (size_t x = 0; x < tileMap.sizex; ++x)
		{
			/*タイル番号を取得*/
			const int tileNo = tileMap.data[y * tileMap.sizex + x];

			/*タイル番号に対応するテクスチャ座標を求める*/
			const float u = static_cast<float>(tileNo % 16) * 32;
			const float v = static_cast<float>(tileNo / 16) * 32;

			/*背景スプライトを設定*/
			spriteListBg.begin()->spriteList.push_back({
				static_cast<float>(x * 32 + scrolldata.baseX),
				static_cast<float>(y * 32 + scrolldata.baseY),
				0,0,MakeTexcoord(u,v,32,32)
				});
		}
	}
}

void Engine::SetBackGroundTexture(TexturePtr tex, TileMapScroll scrolldata)
{
	/*背景スプライトを削除*/
	spriteListBg.clear();
	spriteListBg.push_back(tex);
	spriteListBg.begin()->spriteList.push_back({
		scrolldata.baseX, scrolldata.baseY,
		0, 0, { 0,0,1,1 } });
}

/*ゲームエンジンを実行する*/
int Engine::Run()
{
	int ret = Initialize();

	if (!ret)
	{
		MainLoop();
	}
	Finalize();
	return ret;
}

/*カメラのImGuiを出す(3Dのみ)*/
void Engine::CameraDebug()
{
	ImGui::Begin(cameraObject->name.c_str());

	if (ImGui::CollapsingHeader("position")) {
		ImGui::InputFloat("x", &cameraObject->position.x);
		ImGui::InputFloat("y", &cameraObject->position.y);
		ImGui::InputFloat("z", &cameraObject->position.z);
	}
	if (ImGui::CollapsingHeader("rotation")) {
		ImGui::InputFloat("x", &cameraObject->rotation.rotation.x);
		ImGui::InputFloat("y", &cameraObject->rotation.rotation.y);
		ImGui::InputFloat("z", &cameraObject->rotation.rotation.z);
	}
	for (auto itr : cameraObject->componentList)
	{
		itr->Debug();
	}
	ImGui::End();
	ImGui::ShowStyleEditor(&ImGui::GetStyle());
}

/*UI用のテクスチャ画像を生成する*/
size_t Engine::AddUILayer(const char* image, GLenum filferMode, size_t reserve)
{
	TexturePtr tmptex = nullptr;
	for (auto itr : this->tex) {
		if (image == itr->GetName().c_str()) {
			tmptex = itr;
		}
	}
	if (tmptex == nullptr) {
		tmptex = Texture::Create(image, filferMode);
		this->tex.push_back(tmptex);
	}
	uiLayerList.push_back({ tmptex });
	uiLayerList.back().gameObjectList.reserve(reserve);
	uiLayerList.back().spriteList.reserve(reserve);
	return uiLayerList.size() - 1;
}

/*UI用のテクスチャ画像を一掃する*/
void Engine::ClearUILayers()
{
	uiLayerList.clear();
}

/*引数のスクロールデータをもとに背景を移動させる*/
void Engine::BgScroll(TileMapScroll& scrolldata) noexcept
{
	/*背景をスクロールさせる*/
	if (spriteListBg.size() != 0) {
		switch (scrolldata.type)
		{
		case nonScroll:

			break;
		case upScroll:
			if (spriteListBg.begin()->spriteList.size() >=
				scrolldata.sizeX * scrolldata.sizeY) {
				scrolldata.baseY -= scrolldata.speed * time.deltaTime;
				if (scrolldata.baseY < scrolldata.endPosisionY)
				{
					scrolldata.baseY = scrolldata.endPosisionY;
				}
				for (size_t y = 0; y < scrolldata.sizeY; ++y)
				{
					const float sy = y * 32 + scrolldata.baseY;
					for (size_t x = 0; x < scrolldata.sizeX; ++x)
					{
						spriteListBg.begin()->spriteList[y * scrolldata.sizeX + x].y = sy;
					}
				}
			}
			break;
		case downScroll:
			if (spriteListBg.begin()->spriteList.size() >=
				scrolldata.sizeX * scrolldata.sizeY) {
				scrolldata.baseY += scrolldata.speed * time.deltaTime;
				if (scrolldata.baseY > scrolldata.endPosisionY)
				{
					scrolldata.baseY = scrolldata.endPosisionY;
				}
				for (size_t y = 0; y < scrolldata.sizeY; ++y)
				{
					const float sy = y * 32 + scrolldata.baseY;
					for (size_t x = 0; x < scrolldata.sizeX; ++x)
					{
						spriteListBg.begin()->spriteList[y * scrolldata.sizeX + x].y = sy;
					}
				}
			}
			break;
		case rightScroll:
			if (spriteListBg.begin()->spriteList.size() >=
				scrolldata.sizeX * scrolldata.sizeY)
			{
				scrolldata.baseX -= scrolldata.speed * time.deltaTime;
				if (scrolldata.baseX < scrolldata.endPosisionX)
				{
					scrolldata.baseX = scrolldata.endPosisionX;
				}
				for (size_t y = 0; y < scrolldata.sizeY; ++y)
				{
					for (size_t x = 0; x < scrolldata.sizeX; ++x)
					{
						const float sx = x * 32 + scrolldata.baseX;
						spriteListBg.begin()->spriteList[y * scrolldata.sizeX + x].x = sx;
					}
				}
			}
			break;
		case leftScroll:
			if (spriteListBg.begin()->spriteList.size() >=
				scrolldata.sizeX * scrolldata.sizeY) {
				scrolldata.baseX += scrolldata.speed * time.deltaTime;
				if (scrolldata.baseX > scrolldata.endPosisionX)
				{
					scrolldata.baseX = scrolldata.endPosisionX;
				}
				for (size_t y = 0; y < scrolldata.sizeY; ++y)
				{
					for (size_t x = 0; x < scrolldata.sizeX; ++x)
					{
						const float sx = x * 32 + scrolldata.baseX;
						spriteListBg.begin()->spriteList[y * scrolldata.sizeX + x].x = sx;
					}
				}
			}

			break;
		}
	}
}

/*スクロールデータのbaseに背景の位置を合わせる(Initialize推奨)*/
void Engine::CalcStartUpBgLocation(TileMapScroll& scrolldata) noexcept
{
	if (spriteListBg.begin()->spriteList.size() >=
		scrolldata.sizeX * scrolldata.sizeY) {

		for (size_t y = 0; y < scrolldata.sizeY; ++y)
		{
			for (size_t x = 0; x < scrolldata.sizeX; ++x)
			{
				const float sx = x * 32 + scrolldata.baseX;
				const float sy = y * 32 + scrolldata.baseY;
				spriteListBg.begin()->spriteList[y * scrolldata.sizeX + x].x = sx;
				spriteListBg.begin()->spriteList[y * scrolldata.sizeX + x].y = sy;
			}
		}
	}
}

/*
書式 GLFWglproc glfwGetProcAddress(アドレスを知りたい関数の名前)
gladLoadGllLoader(関数アドレスを取得する関数)
*/
/*OpenGLの関数のアドレスを取得する*/
int Engine::Initialize() {
	GLFW_SPACE::Window& window = GLFW_SPACE::Window::Instance();

	window.Init(GLFW_SPACE::window_W, GLFW_SPACE::window_H, title.c_str());

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) return 1;

	glDebugMessageCallback(DebugCallBuck, nullptr);

	progSprite = ProgramPipeline::Create("Res/standard_2D.vert", "Res/standard_2D.frag");

	progStandard3D = ProgramPipeline::Create("Res/standard_3D.vert", "Res/standard_3D.frag");




	/*頂点データをGPUメモリにコピー*/
	struct Vertex
	{
		MyMath::vec3 pos;/*頂点座標*/
		MyMath::vec2 uv;   /*テクスチャ*/
	};
	const Vertex vertexdata[] =
	{
		{ MyMath::vec3(-0.5f,-0.5f, 0.0f), MyMath::vec2(0, 1)},
		{ MyMath::vec3(0.5f,-0.5f, 0.0f),  MyMath::vec2(1, 1)},
		{ MyMath::vec3(0.5f, 0.5f, 0.0f),  MyMath::vec2(1, 0)},
		{ MyMath::vec3(-0.5f, 0.5f, 0.0f), MyMath::vec2(0, 0)},
	};
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, sizeof(vertexdata), vertexdata, 0);

	/*インデックスデータをGPUメモリにコピー*/

	/*型はショート*/
	const GLshort indexdata[] =
	{
		0,1,2,2,3,0,
	};
	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, sizeof(indexdata), indexdata, 0);

	/*頂点データの構成情報を設定*/
	{
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		/*有効にする頂点属性の配列インデックス*/
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(const void*)offsetof(Vertex, uv.x));
	}
	//GLContext

	const char* filename[] = {
		"Img/bg0.tga" , "Img/objects.tga" ,
	};

	/*texテクスチャの管理番号*/
	for (auto i = 0; i < _countof(filename); i++)
	{
		auto texture = Texture::Create(filename[i], GL_NEAREST);
		tex.push_back(texture);
	}

	/*スプライトの配列を作成*/
	spriteListObj.reserve(10'000);

	if (!Audio::Initialize()) {
		return 1;
	}

	/*ゲームオブジェクト配列の容量を予約*/
	gameObjectList.reserve(1000);

	spriteSsboSize = CalcSsboSize(maxSpriteCount * sizeof(Sprite));
	glCreateBuffers(1, &ssboSprite);
	glNamedBufferStorage(ssboSprite,
		spriteSsboSize * 2,			//ダブルバッファのためにサイズを二倍にする
		nullptr,
		/*コピー先アドレスを取得するためのフラグを指定*/
		GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_WRITE_BIT);
	/*
	vboやiboと同様の関数を使用しているが
	今回はフラグを利用している
	*/
	/*フラグ一覧*/
	/*
	GL_MAP_PRESISTENT_BIT		マッピングを解除しないバッファを作るように指示する
	GL_MAP_COHERENT_BIT			GPUが読み出し中にCPUが書き込んでも、GPUの描画に影響しないバッファを作るように指示する
	GL_MAP_WRITE_BIT			CPUから書き込めるバッファを作るように指示する
	*/

	/*スプライトデータのコピー先アドレスをマッピング*/
	pSpriteSsbo = static_cast<uint8_t*>(glMapNamedBuffer(ssboSprite, GL_WRITE_ONLY));

#ifdef MODEL3D

	meshBuffer = Mesh::MeshBuffer::Create(
		sizeof(Mesh::Vertex) * 100'000, sizeof(uint16_t) * 300'000);


	/*3Dモデル用のバッファを作成*/
	primitiveBuffer = Mesh::PrimitiveBuffer::Create(
		sizeof(Mesh::Vertex) * 100'000, sizeof(uint16_t) * 300'000);

	cameraObject = Create<GameObject>("MainCamera");
	cameraObject->position.y = 10;
	cameraObject->rotation.rotation.x =
		-45.0f * acos(-1.0f) / 180.0f; //カメラを斜め下に向ける
	cameraObject->AddComponent<Camera>();


	/*プリミティブバッファの動作テスト用データを追加*/
	{
		const Mesh::Vertex vertices[] = {
			{{ -1, -1,  1 },{ 0, 0 }},
			{{  1, -1,  1 },{ 1, 0 }},
			{{  1,  1,  1 },{ 1, 1 }},
			{{ -1,  1,  1 },{ 0, 1 }},

			{{  1, -1, -1 },{ 0, 0 }},
			{{ -1, -1, -1 },{ 1, 0 }},
			{{ -1,  1, -1 },{ 1, 1 }},
			{{  1,  1, -1 },{ 0, 1 }},

			{{  1, -1,  1 },{ 0, 0 }},
			{{  1, -1, -1 },{ 1, 0 }},
			{{  1,  1, -1 },{ 1, 1 }},
			{{  1,  1,  1 },{ 0, 1 }},

			{{ -1, -1, -1 },{ 0, 0 }},
			{{ -1, -1,  1 },{ 1, 0 }},
			{{ -1,  1,  1 },{ 1, 1 }},
			{{ -1,  1, -1 },{ 0, 1 }},

			{{  1,  1,  1 },{ 0, 0 }},
			{{  1,  1, -1 },{ 1, 0 }},
			{{ -1,  1, -1 },{ 1, 1 }},
			{{ -1,  1,  1 },{ 0, 1 }},

			{{ -1,  -1, -1 },{ 0, 0 }},
			{{  1,  -1, -1 },{ 1, 0 }},
			{{  1,  -1,  1 },{ 1, 1 }},
			{{ -1,  -1,  1 },{ 0, 1 }},
		};
		const uint16_t indices[] = {
			0,1,2,2,3,0,
			4,5,6,6,7,4,
			8,9,10,10,11,8,
			12,13,14,14,15,12,
			16,17,18,18,19,16,
			20,21,22,22,23,20,
		};
		primitiveBuffer->AddPrimitive(
			vertices, std::size(vertices), indices, std::size(indices));
	}

#endif

#ifdef MODEL2D


	scene = std::make_shared<TitleScene>();
	if (scene)
	{
		scene->Initialize(*this);
	}

#endif // MODEL2D

#ifdef MODELASSIMP
	window.AssimpSetup();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


#endif

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style


	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.GetGLWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 450");

	io.Fonts->AddFontFromFileTTF("Res/Font/x12y16pxMaruMonica.ttf", 20, nullptr, io.Fonts->GetGlyphRangesJapanese());

	::AttachConsole(ATTACH_PARENT_PROCESS);



	return 0;
}


int Engine::MainLoop()
{
	GLFW_SPACE::Window& window = GLFW_SPACE::Window::Instance();
	/*FPS計測機用の変数*/

	//textObject->AddSpriteTexture();
	float setFps = 60.0f;
	bool start = false;

#ifdef MODEL2D
	size_t fadeLayer = AddUILayer("Img/BlackBack.tga", GL_NEAREST, 10);
	GameObjectPtr fade = CreateUI<GameObject>(fadeLayer, "fade", 0, 0);
	fade->AddSpriteTexture({ 0,0,1,1 }, FindWithTexture("Img/BlackBack.tga"), 0, 0, 3, 0, 0.0f);
	std::shared_ptr<FadeController> fadedata = fade->AddComponent<FadeController>();
	fadedata->fadeSpeed = 1; fadedata->fadeObject = fade;

#endif // MODEL2D

#ifdef MODELASSIMP

	glEnable(GL_DEPTH_TEST);
	AssimpShader ourShader("AssimpModel.vert", "AssimpModel.frag");
	AssimpModel ourModel((char*)"Res/player_fighter_01.obj");
#endif


	while (!window.ShouldClose())
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		time.Execute();

#ifdef DEBUG

		float oldFps = setFps;
		ImGui::Begin("Times");
		ImGui::Text(time.GetTotalTimeString().c_str());
		float fps = time.GetFPS();
		ImGui::DragFloat("FPS", &fps);
		ImGui::DragFloat("deltaTime", &time.deltaTime);
		ImGui::SliderFloat("IdealFPS", &setFps, 3, 500);
		ImGui::End();
		if (oldFps != setFps)
		{
			setFps = floor(setFps);
			time.SetFPS(setFps);
		}

#endif // !DEBUG

		if (GetKey(GLFW_KEY_ESCAPE))exit(0);

		/*コピー先バッファを切り替える*/
		spriteSsboIndex = (spriteSsboIndex + 1) % 2;

#ifdef MODEL2D


		/*シーンの切り替え*/
		if (nextScene)
		{
			if (start) { fadedata->trriger = true; }
			else  start = true;
			if (fadedata->fadeWhich == false) {
				if (scene)
				{
					scene->Finalize(*this);
				}

				nextScene->Initialize(*this);
				fadeLayer = AddUILayer("Img/BlackBack.tga", GL_NEAREST, 10);
				RegisterUIObject(fadeLayer, fade);
				scene = std::move(nextScene);
			}
		}
		fadedata->CustomEvent(time.deltaTime);

		/*シーンを更新*/

		if (scene && fadedata->trriger == false)
		{
			scene->Execute(*this, time.deltaTime);
		}

		/*ゲームオブジェクトを更新*/
		UpdateGameObject(time.deltaTime);
		CollideGameObject();
		RemoveGameObject(gameObjectList);
		RemoveCollider();
		for (auto& layer : uiLayerList) {
			RemoveGameObject(layer.gameObjectList);
		}
		MakeSpriteListObj(gameObjectList, spriteListObj);
		for (auto& layer : uiLayerList) {
			MakeSpriteListObj(layer.gameObjectList, layer.spriteList);
		}


#endif // DEBUG

		/*GPUがバッファを使い終わるのを待つ*/
		for (GLsync& sync = syncSpriteSsbo[spriteSsboIndex]; sync;)
		{
			const GLuint64 timeout = 16'700'000;	//16.7ミリ秒(約1/60秒)
			const GLenum status = glClientWaitSync(sync, 0, timeout);
			// GLenum glClientWaitSync(フェンスの管理番号, 制御フラグ, 待機時間);
			if (status != GL_TIMEOUT_EXPIRED)
			{
				//使い終わった同期オブジェクトは消さないでください
				glDeleteSync(sync);
				break;
			}
		}

		/*スプライトデータにSSBOをコピー*/
		//const std::vector<Sprite>* drawList[] = {&spriteListBg ,&spriteListObj };

		uint8_t* p = pSpriteSsbo + spriteSsboIndex * spriteSsboSize;

		for (const auto& sprite : spriteListBg)
		{
			memcpy(p, sprite.spriteList.data(), sprite.spriteList.size() * sizeof(Sprite));
			p += CalcSsboSize(sprite.spriteList.size() * sizeof(Sprite));
		}

		for (const auto& sprite : spriteListObj)
		{
			memcpy(p, sprite.spriteList.data(), sprite.spriteList.size() * sizeof(Sprite));
			p += CalcSsboSize(sprite.spriteList.size() * sizeof(Sprite));
		}

		/*UIレイヤーのスプライトデータをSSBOにコピー*/
		for (const auto& layer : uiLayerList) {
			memcpy(p, layer.spriteList.data(), layer.spriteList.size() * sizeof(Sprite));
			p += CalcSsboSize(layer.spriteList.size() * sizeof(Sprite));
		}

		/*バックバッファをクリア*/
		glViewport(0, 0, GLFW_SPACE::window_W, GLFW_SPACE::window_H);
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

		/*バッファの種類*
		カラーバッファ GL_COLOR_BUFFER_BIT
		深度バッファ GL_DEPTH_BUFFER_BIT
		ステンシルバッファ GL_STENCIL_BUFFER_BIT
		*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*描画に使うシェーダを指定*/
		glUseProgram(*progSprite);

#ifdef MODEL3D
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
#endif // 0

		/*アルファブレンドを有効にする*/
		/*
		ソース係数　== フラグメントシェーダから出力された色
		ディスティネーション係数　== 描画先の画面の色
		*/
		glEnable(GL_BLEND);
		/*ブレンド式の種類*/
		/*
		GL_FUNK_ADD						ソース係数 + ディスティネーション係数
		GL_FUNK_SUBTRACT				ソース係数 - ディスティネーション係数
		GL_FUNK_REVERSE_SUBTRACT		ディスティネーション係数 - ソース係数
		GL_FUNK_MIN						min(ソース係数 * ディスティネーション係数)
		GL_FUNK_MAX						max(ソース係数 * ディスティネーション係数)
		*/
		glBlendEquation(GL_FUNC_ADD);
		/*係数に指定できる名前*/
		/*
		名前					カラーの値			透明度の値
		GL_ZERO					(0,0,0)				0
		GL_ONE					(1,1,1)				1
		GL_SRC_COLOR			(S.r,S.g,S.b)		S.a
		GL_SRC_ALPHA			(S.a,S.a,S.a)		S.a
		GL_ONE_MINUS_SRC_ALPHA	(1-S.a,1-S.a,1-S.a) 1-S.a
		GL_DST_COLOR			(D.r,D.g,D.b)		D.a
		GL_DST_ALPHA			(D.a,D.a,D.a)		D.a
		*/
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		/*よく使われる組み合わせ*/
		/*
		* ブレンド無し
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ZERO);
		*半透明
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		*加算
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		*減算
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		*/

#if 0
		glProgramUniform1f(*progSprite, 0, time.GetTotalSecond());
#endif

		/*スプライトの描画範囲を設定*/
		glViewport(0, 100, GLFW_SPACE::window_W, GLFW_SPACE::window_H);

		///*ユニフォーム変数に、座標変換用パラメータをコピーする*/
		glProgramUniform4f(*progSprite, 2,
			2.0f / static_cast<float>(GLFW_SPACE::window_W),
			2.0f / static_cast<float>(GLFW_SPACE::window_H), -1, -1);

		glBindVertexArray(vao);

		/*スプライト配列を描画*/
		size_t spriteSsboOffset = spriteSsboIndex * spriteSsboSize;

#ifdef MODEL2D
		for (const auto& bg : spriteListBg)
		{
			DrawSpriteList(bg.spriteList, *bg.tex, spriteSsboOffset);
		}
		for (const auto& obj : spriteListObj)
		{
			DrawSpriteList(obj.spriteList, *obj.tex, spriteSsboOffset);
		}

#endif // MODEL2D
		/*UIレイヤーのスプライト配列を描画*/
		glViewport(0, 0, GLFW_SPACE::window_W, GLFW_SPACE::window_H);
		glProgramUniform4f(*progSprite, 2,
			2.0f / GLFW_SPACE::window_W,
			2.0f / GLFW_SPACE::window_H,
			-1, -1);

#ifdef MODEL2D
		for (const auto& layer : uiLayerList) {
			DrawSpriteList(layer.spriteList, *layer.tex, spriteSsboOffset);
		}

#endif // MODEL2D

		/*スプライト用のSSBOの割り当てを解除する*/
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 0, 0, 0);

		/*スプライト描画の直後にフェンスを作成*/
		/*第一引数にフェンスを通過したとみなす条件が入る*/
		syncSpriteSsbo[spriteSsboIndex] =
			glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

#ifdef MODEL3D
		/*3Dモデルの表示テスト*/
		{
#ifdef DEBUG
			CameraDebug();
#endif
			glUseProgram(*progStandard3D);
			glBindVertexArray(*meshBuffer->GetVAO());

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);

			struct DirectionalLight
			{
				MyMath::vec3 color;//色
				float intensity;//明るさ
				MyMath::vec3 direction;//向き
			};
			const float tanx = 0.57735f;
			DirectionalLight directionalLight = {
				MyMath::vec3(1.0f,0.9f,0.8f),
				3,
				MyMath::vec3(tanx,-tanx,-tanx)
			};

			const vec3 color = directionalLight.color * directionalLight.intensity;
			//平行光源の情報を設定
			glProgramUniform3f(*progStandard3D, 100, color.x, color.y, color.z);
			glProgramUniform3f(*progStandard3D, 101,
				directionalLight.direction.x,
				directionalLight.direction.y,
				directionalLight.direction.z);

			static TexturePtr texBox = Texture::Create("Res/enemy_s_01.tga");
			static float rotate = 0;
			rotate += time.deltaTime;

			//第５引数で奥行の設定第二引数がロケーション
			glProgramUniform3f(*progStandard3D, 0, 0, 0, -2);//平行移動
			glProgramUniform2f(*progStandard3D, 1, sin(rotate), cos(rotate));//Y軸回転
			glProgramUniform3f(*progStandard3D, 2, 1, 1, 1);//拡大縮小

			//3D表示用のカメラ情報を設定
			auto camera = cameraObject->GetConponent<Camera>();
			glProgramUniform4f(*progStandard3D, 3,
				camera->fovScaleX, camera->fovScaleY, camera->A, camera->B);
			glProgramUniform3f(*progStandard3D, 4,
				cameraObject->position.x, cameraObject->position.y, cameraObject->position.z);
			glProgramUniform2f(*progStandard3D, 5,
				sin(cameraObject->rotation.rotation.x), cos(cameraObject->rotation.rotation.x)
			);


			const GLuint tex[] = { *texBox };
			glBindTextures(0, 1, tex);
			const auto mesh = meshBuffer->LoadOBJ("Res/player_fighter_01.obj");
			for (float x = 0; x < 3; ++x)
			{
				for (float y = 0; y < 3; ++y)
				{
					for (float z = 0; z < 3; ++z)
					{
						glProgramUniform3f(*progStandard3D, 0,
							x * 4 - 4, y * 4 - 4, z * 4 - 16);
						Mesh::Draw(*mesh, *progStandard3D);
					}
				}
			}

			glBindTextures(0, 1, nullptr);
			glBindVertexArray(0);
			glUseProgram(0);
		}

#endif // 0


#ifdef MODELASSIMP

		ourShader.Use();

		auto assimpCamera = GLFW_SPACE::GetViewAssimpCamera();
		glm::mat4 projection = glm::perspective(glm::radians(assimpCamera->Zoom),
			1280.0f / 820.0f, 0.1f, 100.0f);
		MyMath::mat4 view = GLFW_SPACE::GetViewMatrix();

		ourShader.SetMat4("projection", projection);
		ourShader.SetMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		ourShader.SetMat4("model", model);
		
		
		ourModel.Draw(ourShader);

#endif // MODELASSIMP


		/*音声ライブラリを更新*/
		Audio::Update();

		// Rendering
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.SwapBuffers();
		time.Wait();
	}


	return 0;
}

int Engine::Finalize()
{
	spriteListBg.clear();
	ClearGameObjectList();
	ClearUILayers();
	tex.clear();
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	Audio::Finalize();

	return 0;
}

void Engine::UpdateGameObject(float deltaTime)
{
	/*処理中に配列が変化しないようにコピーを生成*/
	std::vector<GameObject*> list(gameObjectList.size());
	std::transform(gameObjectList.begin(), gameObjectList.end(),
		list.begin(), [](const GameObjectPtr& e) {return e.get(); });

	/*UIレイヤーのゲームオブジェクトのコピーを作成*/
	for (auto& layer : uiLayerList) {
		/*previous(プレビアス、[直前の・ひとつ前の])*/
		const size_t prevListSize = list.size();
		list.resize(prevListSize + layer.gameObjectList.size());
		std::transform(layer.gameObjectList.begin(), layer.gameObjectList.end(),
			list.begin() + prevListSize,
			[](const GameObjectPtr& e) { return e.get(); });
	}

	for (auto& itr : list) {
		itr->Start();
	}

	for (auto& itr : list) {
		if (!itr->isDead)
			itr->Update(deltaTime);
	}
}

#include "Component/ComponentInclude.h"
/*ゲームオブジェクトの衝突判定*/
void Engine::CollideGameObject()
{
	struct WorldCollider
	{
		GameObject* gameObject = {};
		Square aabb = { 0,0,0,0 };
		std::vector<Square> collision = {};
	};

	/*ワールド座標のコライダーを計算*/
	std::vector<WorldCollider> tmp;
	tmp.reserve(collitionList.size());

	for (auto& e : collitionList)
	{
		WorldCollider a;
		/*Startしていないか死んでいるゲームオブジェクトは衝突しない*/
		if (!e.gameObject->IsStarted() || e.gameObject->isDead)
		{
			continue;
		}
		/*ワールド座標　=　ローカル座標 + ゲームオブジェクトの座標*/
		auto collsion = e.collition;
		const SquareList& local = collsion->GetCollision();
		WorldCollider world;
		world.gameObject = e.gameObject.get();
		world.aabb = Add(collsion->GetAabb(), e.gameObject->position.x, e.gameObject->position.y);
		world.collision.resize(local.size());
		for (size_t i = 0; i < local.size(); ++i)
		{
			world.collision[i] = Add(local[i], e.gameObject->position.x, e.gameObject->position.y);
		}
		tmp.push_back(world);
	}

	/*コライダーの衝突判定*/
	for (auto pa = tmp.begin(); pa != tmp.end(); ++pa)
	{
		for (auto pb = pa + 1; pb != tmp.end(); ++pb)
		{
			if (pb->gameObject->isDead)
			{
				continue;	/*衝突してない*/
			}
			
			auto cpList = pa->DetectCollision(*pb);
			if (!cpList.empty()) {
				/*衝突しているので、しょとつイベント関数を実行*/
				pa->gameObject->OnCollision(*pb->gameObject);
				pb->gameObject->OnCollision(*pa->gameObject);

				if (pa->gameObject->isDead)
				{
					break;		//paが死んでいるならループを打ち切る
				}
			}
		}
	}
}

TexturePtr Engine::FindWithTexture(std::string filename) noexcept
{
	for (auto itr : tex)
	{
		if (itr->GetName() == filename)
		{
			return itr->shared_from_this();
		}
	}
	auto texture = Texture::Create(filename, GL_NEAREST);
	tex.push_back(texture);
	return texture;
}

GameObjectPtr Engine::FindWithTagGameObject(GameObjectTag tag) noexcept
{
	for (auto itr : gameObjectList)
	{
		if (itr->tag == tag)
		{
			return itr->shared_from_this();
		}
	}
	return nullptr;
}

GameObjectList Engine::FindWithTagGameObjects(GameObjectTag tag) noexcept
{
	GameObjectList tmp;
	tmp.reserve(gameObjectList.size());

	for (auto itr : gameObjectList)
	{
		if (itr->tag == tag)
		{
			tmp.push_back(itr->shared_from_this());
		}
	}
	return tmp;
}

GameObjectPtr Engine::FindWithGameObjectName(std::string name) noexcept
{
	for (auto itr : gameObjectList)
	{
		if (itr->name == name)
		{
			return itr->shared_from_this();
		}
	}
	return nullptr;
}

GameObjectList Engine::FindWithGameObjectsName(std::string name) noexcept
{
	GameObjectList tmp;
	tmp.reserve(gameObjectList.size());

	for (auto itr : gameObjectList)
	{
		if (itr->name == name)
		{
			tmp.push_back(itr->shared_from_this());
		}
	}
	return tmp;
}

void Engine::RegisterCollitionObject(GameObjectPtr gameObject, std::shared_ptr<Box2DCollision> collision) noexcept
{
	Collider collider;
	collider.gameObject = gameObject; collider.collition = collision;
	collitionList.push_back(collider);
}

bool Engine::CheckCollideObject(GameObjectPtr gameObject, MyMath::vec3 vector)
{
	struct WorldCollider
	{
		GameObject* gameObject = {};
		Square aabb = { 0,0,0,0 };
		std::vector<Square> collision = {};
	};

	/*ワールド座標のコライダーを計算*/
	WorldCollider main;
	for (auto e : collitionList)
	{
		if (e.gameObject.get() == gameObject.get())
		{
			main.gameObject = e.gameObject.get();
			main.aabb = Add(e.collition->GetAabb(),
				e.gameObject->position.x + vector.x, e.gameObject->position.y + vector.y);
		}
	}

	std::vector<WorldCollider> tmp;
	tmp.reserve(collitionList.size());

	for (auto& e : collitionList)
	{
		/*Startしていないか死んでいるゲームオブジェクトは衝突しない*/
		if (!e.gameObject->IsStarted() || e.gameObject->isDead)
		{
			continue;
		}
		/*ワールド座標　=　ローカル座標 + ゲームオブジェクトの座標*/
		auto collsion = e.collition;
		const SquareList& local = collsion->GetCollision();
		WorldCollider world;
		world.gameObject = e.gameObject.get();
		world.aabb = Add(collsion->GetAabb(), e.gameObject->position.x, e.gameObject->position.y);
		tmp.push_back(world);
	}

	for (auto pa = tmp.begin(); pa != tmp.end(); ++pa)
	{
		if (pa->gameObject->name != "object") {
			continue;
		}
		if (main.gameObject == pa->gameObject) {
			continue;
		}
		if (pa->gameObject->isDead)
		{
			continue;	/*衝突してない*/
		}
		if (!Overlap(main.aabb, pa->aabb))
		{
			continue;	/*衝突していない*/
		}
		return true;
	}
	return false;
}

void Engine::RemoveGameObject(GameObjectList& gameObjectList)
{
	/*生きているオブジェクトと死んでいるオブジェクトに分ける*/
	const auto i = std::partition(
		gameObjectList.begin(), gameObjectList.end(),
		[](const GameObjectPtr& e) {return !e->isDead; }
	);

	if (i == gameObjectList.end())
		return;

	/*死んでいるオブジェクトを別の配列に移動*/
	GameObjectList deadList(
		std::make_move_iterator(i),
		std::make_move_iterator(gameObjectList.end()));

	/*配列から移動済みのオブジェクトを削除*/
	gameObjectList.erase(i, gameObjectList.end());

	/*死んでいるオブジェクトに対してDestroyイベントを実行*/
	for (auto& e : deadList)
	{
		e->OnDestroy();
	}
	/*ここで死んでいるオブジェクトに対しが削除される*/
}

void Engine::RemoveCollider()
{
	collitionList.erase(
		std::remove_if(collitionList.begin(), collitionList.end(), [](const Collider& e) {return e.gameObject->isDead; })
		, collitionList.end());
}

void Engine::MakeSpriteListObj(GameObjectList& gameObjectList, std::vector<ObjectSprite>& spriteList)
{
	/*既存の動く物体用のスプライト配列を空にする*/
	spriteList.clear();

	/*優先順位が大きい順番に並び替える*/
	std::stable_sort(
		gameObjectList.begin(), gameObjectList.end(),
		[](const GameObjectPtr& a, const GameObjectPtr& b) {
			return a->layer > b->layer;
		}
	);

	/*ゲームオブジェクトのスプライトデータをスプライト配列に追加*/
	/*同時にワールド座標へ変換する*/
	for (const auto& e : gameObjectList)
	{
		for (const auto& sprite : e->spriteList)
		{
			spriteList.push_back(sprite);
			Sprite& s = spriteList.back().spriteList.back();
			s.x += e->position.x;
			s.y += e->position.y;
		}
	}

}

void Engine::MakeSpriteListObj(GameObjectList& gameObjectList, SpriteList& spriteList)
{
	/*既存の動く物体用のスプライト配列を空にする*/
	spriteList.clear();

	/*優先順位が大きい順番に並び替える*/
	std::stable_sort(
		gameObjectList.begin(), gameObjectList.end(),
		[](const GameObjectPtr& a, const GameObjectPtr& b) {
			return a->layer > b->layer;
		}
	);

	/*ゲームオブジェクトのスプライトデータをスプライト配列に追加*/
	/*同時にワールド座標へ変換する*/
	for (const auto& e : gameObjectList)
	{
		for (const auto& sprite : e->spriteList)
		{
			for (const auto& sd : sprite.spriteList) {
				spriteList.push_back(sd);
				Sprite& s = spriteList.back();
				s.x += e->position.x;
				s.y += e->position.y;
			}
		}
	}

}

void Engine::DrawSpriteList(const SpriteList& spriteList, GLuint tex, size_t& spriteSsboOffset)
{
	/*描画データがなければスキップ*/
	const size_t size = CalcSsboSize(spriteList.size() * sizeof(Sprite));
	if (size == 0)	return;
	this->tex;
	/*パッチを指定する*/
	glBindTextures(0, 1, &tex);

	/*スプライト用SSBOを割り当てる*/
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, ssboSprite,
		spriteSsboOffset, static_cast<GLintptr>(size));
	spriteSsboOffset += size;

	/*vec3型のユニf０ム変数にデータをコピーします*/
	/*
	プリミティブ
	種類				説明
	GL_POINTS			点の集まり
	GL_LINES			直線の集まり
	GL_LINE_STRIP		連続した直線
	GL_LINE_LOOP        STRIPの最初と最後にも線がつく
	GL_TRIANGLES		三角形の集まり
	GL_TRIANGLE_STRIP	辺の繋がった三角形の集まり
	GL_TRIANGLE_FAN		辺の繋がった、最初の頂点を共有する三角形のあつまり
	*/
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0,
		static_cast<GLsizei>(spriteList.size()));
}