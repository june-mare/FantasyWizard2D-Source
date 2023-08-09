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


//�t�@�C�o�[�ɂ��Ă��l���Ă݂�

/*SSBO�̃T�C�Y���v�Z����*/
size_t CalcSsboSize(size_t n)
{
	/*
	n�ُ킩��256�̔{����Ԃ�
	OpenGL�ł�SSBO�̃A���C�����g�̍ő�l��256�ƌ��߂��Ă���
	*/
	const size_t alignment = 256;	/*�A���C�����g�i�f�[�^�̋��E���j*/
	return ((n + alignment - 1) / alignment) * alignment;
}

/*���b�Z�[�W����������R�[���o�b�N�֐�
* @param source         ���b�Z�[�W�̔��M��
* @param type           ���b�Z�[�W�̎��
* @param id             ���b�Z�[�W����ʂɎ��ʂ���l
* @param severity       ���b�Z�[�W�̏d�v�x
* @param length         ���b�Z�[�W�̕�����
* @param message        ���b�Z�[�W�{��
* @param userParam      �R�[���o�b�N�ݒ莞�Ɏw�肵���|�C���^
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

/*�t�@�C����ǂݍ��݃I�u�W�F�N�g������ʒu�Ȃǂ�Ԃ�*/
TileMap Engine::LoadTileMap(const char* filename, TileMapScroll scrolldata)
{
	/*�t�@�C�����J��*/
	std::ifstream file;
	file.open(filename);
	if (!file)
	{
		return {};
	}

	TileMap tileMap;
	tileMap.data.reserve(scrolldata.sizeX * scrolldata.sizeY);/*�K���ȗe�ʂ�\��*/

	/*�t�@�C����ǂݍ���*/
	while (!file.eof())
	{
		/*1�s�ǂݍ���*/
		std::string line;
		std::getline(file, line);
		if (line.size() <= 0)
		{
			continue;/*�f�[�^�̂Ȃ��s�𖳎�*/
		}
		line.push_back(',');/*�I�[��ǉ�*/

		/*�J���}��؂蕶����𐔒l�ɕϊ�*/
		tileMap.sizex = 0;
		const char* start = line.data();
		const char* end = start + line.size();
		for (const char* p = start; p != end; ++p)
		{
			if (*p == ',') {
				const int n = atoi(start);
				tileMap.data.push_back(n);
				start = p + 1;/*���̃J���}��؂���w���悤�ɍX�V*/
				++tileMap.sizex;
			}
		}
		++tileMap.sizey;
	}

	return tileMap;
}

/*�^�C���}�b�v�ɓo�^����Ă���f�[�^�����Ƃɔw�i���쐬����*/
void Engine::SetSpriteList(const TileMap& tileMap, TexturePtr tex, TileMapScroll scrolldata)
{
	/*�w�i�X�v���C�g���폜*/
	spriteListBg.clear();

	scrolldata.baseX = 16;
	scrolldata.baseY = 16;
	spriteListBg.push_back(tex);
	/*�z�u�f�[�^���Q�Ƃ��Ĕw�i�X�v���C�g��ݒ�*/
	/*��ʃT�C�Y = 512x512,�^�C���T�C�Y = 32x32*/
	for (size_t y = tileMap.sizey; y > 0;)
	{
		--y;/*�}�C�i�X�ɂȂ�Ȃ��悤�ɂ����Ō��Z*/
		for (size_t x = 0; x < tileMap.sizex; ++x)
		{
			/*�^�C���ԍ����擾*/
			const int tileNo = tileMap.data[y * tileMap.sizex + x];

			/*�^�C���ԍ��ɑΉ�����e�N�X�`�����W�����߂�*/
			const float u = static_cast<float>(tileNo % 16) * 32;
			const float v = static_cast<float>(tileNo / 16) * 32;

			/*�w�i�X�v���C�g��ݒ�*/
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
	/*�w�i�X�v���C�g���폜*/
	spriteListBg.clear();
	spriteListBg.push_back(tex);
	spriteListBg.begin()->spriteList.push_back({
		scrolldata.baseX, scrolldata.baseY,
		0, 0, { 0,0,1,1 } });
}

/*�Q�[���G���W�������s����*/
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

/*�J������ImGui���o��(3D�̂�)*/
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

/*UI�p�̃e�N�X�`���摜�𐶐�����*/
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

/*UI�p�̃e�N�X�`���摜����|����*/
void Engine::ClearUILayers()
{
	uiLayerList.clear();
}

/*�����̃X�N���[���f�[�^�����Ƃɔw�i���ړ�������*/
void Engine::BgScroll(TileMapScroll& scrolldata) noexcept
{
	/*�w�i���X�N���[��������*/
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

/*�X�N���[���f�[�^��base�ɔw�i�̈ʒu�����킹��(Initialize����)*/
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
���� GLFWglproc glfwGetProcAddress(�A�h���X��m�肽���֐��̖��O)
gladLoadGllLoader(�֐��A�h���X���擾����֐�)
*/
/*OpenGL�̊֐��̃A�h���X���擾����*/
int Engine::Initialize() {
	GLFW_SPACE::Window& window = GLFW_SPACE::Window::Instance();

	window.Init(GLFW_SPACE::window_W, GLFW_SPACE::window_H, title.c_str());

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) return 1;

	glDebugMessageCallback(DebugCallBuck, nullptr);

	progSprite = ProgramPipeline::Create("Res/standard_2D.vert", "Res/standard_2D.frag");

	progStandard3D = ProgramPipeline::Create("Res/standard_3D.vert", "Res/standard_3D.frag");




	/*���_�f�[�^��GPU�������ɃR�s�[*/
	struct Vertex
	{
		MyMath::vec3 pos;/*���_���W*/
		MyMath::vec2 uv;   /*�e�N�X�`��*/
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

	/*�C���f�b�N�X�f�[�^��GPU�������ɃR�s�[*/

	/*�^�̓V���[�g*/
	const GLshort indexdata[] =
	{
		0,1,2,2,3,0,
	};
	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, sizeof(indexdata), indexdata, 0);

	/*���_�f�[�^�̍\������ݒ�*/
	{
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		/*�L���ɂ��钸�_�����̔z��C���f�b�N�X*/
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

	/*tex�e�N�X�`���̊Ǘ��ԍ�*/
	for (auto i = 0; i < _countof(filename); i++)
	{
		auto texture = Texture::Create(filename[i], GL_NEAREST);
		tex.push_back(texture);
	}

	/*�X�v���C�g�̔z����쐬*/
	spriteListObj.reserve(10'000);

	if (!Audio::Initialize()) {
		return 1;
	}

	/*�Q�[���I�u�W�F�N�g�z��̗e�ʂ�\��*/
	gameObjectList.reserve(1000);

	spriteSsboSize = CalcSsboSize(maxSpriteCount * sizeof(Sprite));
	glCreateBuffers(1, &ssboSprite);
	glNamedBufferStorage(ssboSprite,
		spriteSsboSize * 2,			//�_�u���o�b�t�@�̂��߂ɃT�C�Y���{�ɂ���
		nullptr,
		/*�R�s�[��A�h���X���擾���邽�߂̃t���O���w��*/
		GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_WRITE_BIT);
	/*
	vbo��ibo�Ɠ��l�̊֐����g�p���Ă��邪
	����̓t���O�𗘗p���Ă���
	*/
	/*�t���O�ꗗ*/
	/*
	GL_MAP_PRESISTENT_BIT		�}�b�s���O���������Ȃ��o�b�t�@�����悤�Ɏw������
	GL_MAP_COHERENT_BIT			GPU���ǂݏo������CPU����������ł��AGPU�̕`��ɉe�����Ȃ��o�b�t�@�����悤�Ɏw������
	GL_MAP_WRITE_BIT			CPU���珑�����߂�o�b�t�@�����悤�Ɏw������
	*/

	/*�X�v���C�g�f�[�^�̃R�s�[��A�h���X���}�b�s���O*/
	pSpriteSsbo = static_cast<uint8_t*>(glMapNamedBuffer(ssboSprite, GL_WRITE_ONLY));

#ifdef MODEL3D

	meshBuffer = Mesh::MeshBuffer::Create(
		sizeof(Mesh::Vertex) * 100'000, sizeof(uint16_t) * 300'000);


	/*3D���f���p�̃o�b�t�@���쐬*/
	primitiveBuffer = Mesh::PrimitiveBuffer::Create(
		sizeof(Mesh::Vertex) * 100'000, sizeof(uint16_t) * 300'000);

	cameraObject = Create<GameObject>("MainCamera");
	cameraObject->position.y = 10;
	cameraObject->rotation.rotation.x =
		-45.0f * acos(-1.0f) / 180.0f; //�J�������΂߉��Ɍ�����
	cameraObject->AddComponent<Camera>();


	/*�v���~�e�B�u�o�b�t�@�̓���e�X�g�p�f�[�^��ǉ�*/
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
	/*FPS�v���@�p�̕ϐ�*/

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

		/*�R�s�[��o�b�t�@��؂�ւ���*/
		spriteSsboIndex = (spriteSsboIndex + 1) % 2;

#ifdef MODEL2D


		/*�V�[���̐؂�ւ�*/
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

		/*�V�[�����X�V*/

		if (scene && fadedata->trriger == false)
		{
			scene->Execute(*this, time.deltaTime);
		}

		/*�Q�[���I�u�W�F�N�g���X�V*/
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

		/*GPU���o�b�t�@���g���I���̂�҂�*/
		for (GLsync& sync = syncSpriteSsbo[spriteSsboIndex]; sync;)
		{
			const GLuint64 timeout = 16'700'000;	//16.7�~���b(��1/60�b)
			const GLenum status = glClientWaitSync(sync, 0, timeout);
			// GLenum glClientWaitSync(�t�F���X�̊Ǘ��ԍ�, ����t���O, �ҋ@����);
			if (status != GL_TIMEOUT_EXPIRED)
			{
				//�g���I����������I�u�W�F�N�g�͏����Ȃ��ł�������
				glDeleteSync(sync);
				break;
			}
		}

		/*�X�v���C�g�f�[�^��SSBO���R�s�[*/
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

		/*UI���C���[�̃X�v���C�g�f�[�^��SSBO�ɃR�s�[*/
		for (const auto& layer : uiLayerList) {
			memcpy(p, layer.spriteList.data(), layer.spriteList.size() * sizeof(Sprite));
			p += CalcSsboSize(layer.spriteList.size() * sizeof(Sprite));
		}

		/*�o�b�N�o�b�t�@���N���A*/
		glViewport(0, 0, GLFW_SPACE::window_W, GLFW_SPACE::window_H);
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

		/*�o�b�t�@�̎��*
		�J���[�o�b�t�@ GL_COLOR_BUFFER_BIT
		�[�x�o�b�t�@ GL_DEPTH_BUFFER_BIT
		�X�e���V���o�b�t�@ GL_STENCIL_BUFFER_BIT
		*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*�`��Ɏg���V�F�[�_���w��*/
		glUseProgram(*progSprite);

#ifdef MODEL3D
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
#endif // 0

		/*�A���t�@�u�����h��L���ɂ���*/
		/*
		�\�[�X�W���@== �t���O�����g�V�F�[�_����o�͂��ꂽ�F
		�f�B�X�e�B�l�[�V�����W���@== �`���̉�ʂ̐F
		*/
		glEnable(GL_BLEND);
		/*�u�����h���̎��*/
		/*
		GL_FUNK_ADD						�\�[�X�W�� + �f�B�X�e�B�l�[�V�����W��
		GL_FUNK_SUBTRACT				�\�[�X�W�� - �f�B�X�e�B�l�[�V�����W��
		GL_FUNK_REVERSE_SUBTRACT		�f�B�X�e�B�l�[�V�����W�� - �\�[�X�W��
		GL_FUNK_MIN						min(�\�[�X�W�� * �f�B�X�e�B�l�[�V�����W��)
		GL_FUNK_MAX						max(�\�[�X�W�� * �f�B�X�e�B�l�[�V�����W��)
		*/
		glBlendEquation(GL_FUNC_ADD);
		/*�W���Ɏw��ł��閼�O*/
		/*
		���O					�J���[�̒l			�����x�̒l
		GL_ZERO					(0,0,0)				0
		GL_ONE					(1,1,1)				1
		GL_SRC_COLOR			(S.r,S.g,S.b)		S.a
		GL_SRC_ALPHA			(S.a,S.a,S.a)		S.a
		GL_ONE_MINUS_SRC_ALPHA	(1-S.a,1-S.a,1-S.a) 1-S.a
		GL_DST_COLOR			(D.r,D.g,D.b)		D.a
		GL_DST_ALPHA			(D.a,D.a,D.a)		D.a
		*/
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		/*�悭�g����g�ݍ��킹*/
		/*
		* �u�����h����
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ZERO);
		*������
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		*���Z
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		*���Z
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		*/

#if 0
		glProgramUniform1f(*progSprite, 0, time.GetTotalSecond());
#endif

		/*�X�v���C�g�̕`��͈͂�ݒ�*/
		glViewport(0, 100, GLFW_SPACE::window_W, GLFW_SPACE::window_H);

		///*���j�t�H�[���ϐ��ɁA���W�ϊ��p�p�����[�^���R�s�[����*/
		glProgramUniform4f(*progSprite, 2,
			2.0f / static_cast<float>(GLFW_SPACE::window_W),
			2.0f / static_cast<float>(GLFW_SPACE::window_H), -1, -1);

		glBindVertexArray(vao);

		/*�X�v���C�g�z���`��*/
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
		/*UI���C���[�̃X�v���C�g�z���`��*/
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

		/*�X�v���C�g�p��SSBO�̊��蓖�Ă���������*/
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 0, 0, 0);

		/*�X�v���C�g�`��̒���Ƀt�F���X���쐬*/
		/*�������Ƀt�F���X��ʉ߂����Ƃ݂Ȃ�����������*/
		syncSpriteSsbo[spriteSsboIndex] =
			glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

#ifdef MODEL3D
		/*3D���f���̕\���e�X�g*/
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
				MyMath::vec3 color;//�F
				float intensity;//���邳
				MyMath::vec3 direction;//����
			};
			const float tanx = 0.57735f;
			DirectionalLight directionalLight = {
				MyMath::vec3(1.0f,0.9f,0.8f),
				3,
				MyMath::vec3(tanx,-tanx,-tanx)
			};

			const vec3 color = directionalLight.color * directionalLight.intensity;
			//���s�����̏���ݒ�
			glProgramUniform3f(*progStandard3D, 100, color.x, color.y, color.z);
			glProgramUniform3f(*progStandard3D, 101,
				directionalLight.direction.x,
				directionalLight.direction.y,
				directionalLight.direction.z);

			static TexturePtr texBox = Texture::Create("Res/enemy_s_01.tga");
			static float rotate = 0;
			rotate += time.deltaTime;

			//��T�����ŉ��s�̐ݒ�����������P�[�V����
			glProgramUniform3f(*progStandard3D, 0, 0, 0, -2);//���s�ړ�
			glProgramUniform2f(*progStandard3D, 1, sin(rotate), cos(rotate));//Y����]
			glProgramUniform3f(*progStandard3D, 2, 1, 1, 1);//�g��k��

			//3D�\���p�̃J��������ݒ�
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


		/*�������C�u�������X�V*/
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
	/*�������ɔz�񂪕ω����Ȃ��悤�ɃR�s�[�𐶐�*/
	std::vector<GameObject*> list(gameObjectList.size());
	std::transform(gameObjectList.begin(), gameObjectList.end(),
		list.begin(), [](const GameObjectPtr& e) {return e.get(); });

	/*UI���C���[�̃Q�[���I�u�W�F�N�g�̃R�s�[���쐬*/
	for (auto& layer : uiLayerList) {
		/*previous(�v���r�A�X�A[���O�́E�ЂƂO��])*/
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
/*�Q�[���I�u�W�F�N�g�̏Փ˔���*/
void Engine::CollideGameObject()
{
	struct WorldCollider
	{
		GameObject* gameObject = {};
		Square aabb = { 0,0,0,0 };
		std::vector<Square> collision = {};
	};

	/*���[���h���W�̃R���C�_�[���v�Z*/
	std::vector<WorldCollider> tmp;
	tmp.reserve(collitionList.size());

	for (auto& e : collitionList)
	{
		WorldCollider a;
		/*Start���Ă��Ȃ�������ł���Q�[���I�u�W�F�N�g�͏Փ˂��Ȃ�*/
		if (!e.gameObject->IsStarted() || e.gameObject->isDead)
		{
			continue;
		}
		/*���[���h���W�@=�@���[�J�����W + �Q�[���I�u�W�F�N�g�̍��W*/
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

	/*�R���C�_�[�̏Փ˔���*/
	for (auto pa = tmp.begin(); pa != tmp.end(); ++pa)
	{
		for (auto pb = pa + 1; pb != tmp.end(); ++pb)
		{
			if (pb->gameObject->isDead)
			{
				continue;	/*�Փ˂��ĂȂ�*/
			}
			
			auto cpList = pa->DetectCollision(*pb);
			if (!cpList.empty()) {
				/*�Փ˂��Ă���̂ŁA����ƂC�x���g�֐������s*/
				pa->gameObject->OnCollision(*pb->gameObject);
				pb->gameObject->OnCollision(*pa->gameObject);

				if (pa->gameObject->isDead)
				{
					break;		//pa������ł���Ȃ烋�[�v��ł��؂�
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

	/*���[���h���W�̃R���C�_�[���v�Z*/
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
		/*Start���Ă��Ȃ�������ł���Q�[���I�u�W�F�N�g�͏Փ˂��Ȃ�*/
		if (!e.gameObject->IsStarted() || e.gameObject->isDead)
		{
			continue;
		}
		/*���[���h���W�@=�@���[�J�����W + �Q�[���I�u�W�F�N�g�̍��W*/
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
			continue;	/*�Փ˂��ĂȂ�*/
		}
		if (!Overlap(main.aabb, pa->aabb))
		{
			continue;	/*�Փ˂��Ă��Ȃ�*/
		}
		return true;
	}
	return false;
}

void Engine::RemoveGameObject(GameObjectList& gameObjectList)
{
	/*�����Ă���I�u�W�F�N�g�Ǝ���ł���I�u�W�F�N�g�ɕ�����*/
	const auto i = std::partition(
		gameObjectList.begin(), gameObjectList.end(),
		[](const GameObjectPtr& e) {return !e->isDead; }
	);

	if (i == gameObjectList.end())
		return;

	/*����ł���I�u�W�F�N�g��ʂ̔z��Ɉړ�*/
	GameObjectList deadList(
		std::make_move_iterator(i),
		std::make_move_iterator(gameObjectList.end()));

	/*�z�񂩂�ړ��ς݂̃I�u�W�F�N�g���폜*/
	gameObjectList.erase(i, gameObjectList.end());

	/*����ł���I�u�W�F�N�g�ɑ΂���Destroy�C�x���g�����s*/
	for (auto& e : deadList)
	{
		e->OnDestroy();
	}
	/*�����Ŏ���ł���I�u�W�F�N�g�ɑ΂����폜�����*/
}

void Engine::RemoveCollider()
{
	collitionList.erase(
		std::remove_if(collitionList.begin(), collitionList.end(), [](const Collider& e) {return e.gameObject->isDead; })
		, collitionList.end());
}

void Engine::MakeSpriteListObj(GameObjectList& gameObjectList, std::vector<ObjectSprite>& spriteList)
{
	/*�����̓������̗p�̃X�v���C�g�z�����ɂ���*/
	spriteList.clear();

	/*�D�揇�ʂ��傫�����Ԃɕ��ёւ���*/
	std::stable_sort(
		gameObjectList.begin(), gameObjectList.end(),
		[](const GameObjectPtr& a, const GameObjectPtr& b) {
			return a->layer > b->layer;
		}
	);

	/*�Q�[���I�u�W�F�N�g�̃X�v���C�g�f�[�^���X�v���C�g�z��ɒǉ�*/
	/*�����Ƀ��[���h���W�֕ϊ�����*/
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
	/*�����̓������̗p�̃X�v���C�g�z�����ɂ���*/
	spriteList.clear();

	/*�D�揇�ʂ��傫�����Ԃɕ��ёւ���*/
	std::stable_sort(
		gameObjectList.begin(), gameObjectList.end(),
		[](const GameObjectPtr& a, const GameObjectPtr& b) {
			return a->layer > b->layer;
		}
	);

	/*�Q�[���I�u�W�F�N�g�̃X�v���C�g�f�[�^���X�v���C�g�z��ɒǉ�*/
	/*�����Ƀ��[���h���W�֕ϊ�����*/
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
	/*�`��f�[�^���Ȃ���΃X�L�b�v*/
	const size_t size = CalcSsboSize(spriteList.size() * sizeof(Sprite));
	if (size == 0)	return;
	this->tex;
	/*�p�b�`���w�肷��*/
	glBindTextures(0, 1, &tex);

	/*�X�v���C�g�pSSBO�����蓖�Ă�*/
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, ssboSprite,
		spriteSsboOffset, static_cast<GLintptr>(size));
	spriteSsboOffset += size;

	/*vec3�^�̃��jf�O���ϐ��Ƀf�[�^���R�s�[���܂�*/
	/*
	�v���~�e�B�u
	���				����
	GL_POINTS			�_�̏W�܂�
	GL_LINES			�����̏W�܂�
	GL_LINE_STRIP		�A����������
	GL_LINE_LOOP        STRIP�̍ŏ��ƍŌ�ɂ�������
	GL_TRIANGLES		�O�p�`�̏W�܂�
	GL_TRIANGLE_STRIP	�ӂ̌q�������O�p�`�̏W�܂�
	GL_TRIANGLE_FAN		�ӂ̌q�������A�ŏ��̒��_�����L����O�p�`�̂��܂�
	*/
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0,
		static_cast<GLsizei>(spriteList.size()));
}