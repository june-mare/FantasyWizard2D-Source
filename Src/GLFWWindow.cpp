#include"GLFWWindow.h"
#include <Windows.h>
#include <iomanip>
#include <iostream>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"


namespace GLFW_SPACE {
	namespace
		/* unnamed */
	{
		/*GLFWからのエラー報告を処理する*/
		void ErrorCallback(int error, const char* desc)
		{
			::std::cerr << "ERROR(0x" << ::std::hex << ::std::setfill('0') << ::std::setw(8) << error << "): " << desc << ::std::endl;
		}
	} // unnamed namespace

	Window::Window() : isGLFWInitialized(false), isInitialized(false), window(nullptr)
	{

	}
	Window::~Window()
	{
		if (isGLFWInitialized) {
			glfwTerminate();
		}
	}

	/*GLFWの初期化*/
	bool Window::Init(int w, int h, const char* title)
	{
		/*すでに作られているかの確認*/
		if (isInitialized) {
		    ::std::cerr << "ERROR: GLFWEWは既に初期化されています." << ::std::endl;
			return false;
		}
		if (!isGLFWInitialized) {
			glfwSetErrorCallback(ErrorCallback);
			if (glfwInit() != GL_TRUE) {
				return false;
			}
			isGLFWInitialized = true;
		}
		/*
		GLFW_OPENGL_PROFILE
		コアプロファイルで古いものと新しいものの互換性を指定できる
	 　　*/
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		/*
		GLFW_OPENGL_FORWARD_COMPAT
		前方互換性の有無を指定
		「前方互換性」というのは「近い未来のバージョンでも動作する保証がある」という意味
		*/
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		/*
		GLFW_CONTEXT_VERSION_MAJOR
		利用するOpenGLのメジャーバージョン(バージョン番号のドットの前側)を指定する
		今回は4.5を使うので 4 を指定
		*/
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		/*GLFW_CONTEXT_VERSION_MINOR
		利用するOpenGLのマイナーバージョン(バージョン番号のドットの後ろ側)を指定する
		今回は4.5を使うので 5 を指定
		*/
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		/*
		GLFW_OPENGL_DEBUG_CONTEXT
		GLFW_TRUE を指定すると、OpenGLのデバッグ支援機能が有効になる
		この機能はバージョン4.3で追加された。
		*/
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);


		/*描画ウィンドウの作成*/
		if (!window) {
			window = glfwCreateWindow(w, h, title, nullptr, nullptr);
			if (!window) {
				return false;
			}
			/*コンテキストの生成*/
			glfwMakeContextCurrent(window);
		}


		/*バイト数の変数　使用しているレンダーとバージョンを書き出している*/
		//const GLubyte* renderer = glGetString(GL_RENDERER);
		//std::cout << "Renderer: " << renderer << std::endl;
		//const GLubyte* version = glGetString(GL_VERSION);
		//std::cout << "Version: " << version << std::endl;
		
		isInitialized = true;
		return true;
	}

	/*GLFWの終了のbool*/
	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(window);
	}

	/* フロントバッファとバックバッファを切り替える*/
	void Window::SwapBuffers() const
	{
		glfwSwapBuffers(window);
	}

	void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}


	bool  isFirstMouse = true;
	float lastx = window_W / 2.0f;
	float lasty = window_H / 2.0f;
	AssimpCamera camera(MyMath::vec3(0.0f, 0.0f, 3.0f));

	void MouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		float xposon = static_cast<float>(xpos);
		float yposon = static_cast<float>(ypos);
		if (isFirstMouse)
		{
			lastx = xposon;
			lasty = yposon;
			isFirstMouse = false;
		}

		float xoffset = xposon - lastx;
		float yoffset = lasty - yposon;

		lastx = xposon;
		lasty = yposon;
		camera.ProcessMouseMovement(xoffset, yoffset);
	}

	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera.ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void Window::AssimpSetup()
	{
		glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
		glfwSetCursorPosCallback(window, MouseCallback);
		glfwSetScrollCallback(window, ScrollCallback);
		glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);		
	}

	MyMath::mat4 GetViewMatrix()
	{
		return camera.GetViewMatrix();
	}

	AssimpCamera* GetViewAssimpCamera()
	{
		return &camera;
	}

} // namespace GLFWEW
