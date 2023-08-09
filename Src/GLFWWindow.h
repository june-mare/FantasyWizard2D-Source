#pragma once
#ifndef GLFWEW_INCLUDED
#define GLFWEW_INCLUDED
#define _CRT_SECURE_NO_WARNINGS
/*gladÇÕç≈èâÇ…ïKÇ∏IncludeÇ∑ÇÈ*/
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#pragma comment(lib,"opengl32.lib")
#include "MyMath.h"
#include "Assimp/AssimpCamera.h"
namespace GLFW_SPACE {

	const int window_W = 1280;
	const int window_H = 820;

	class Window {
	public:
		static Window& Instance()
		{
			static Window instance;
			return instance;
		}
		bool Init(int w, int h, const char* title);
		bool ShouldClose() const;
		void SwapBuffers() const;
		GLFWwindow* GetGLWindow() { return window; }
		//void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

		void AssimpSetup();



	private:
		Window();
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;


		bool isGLFWInitialized;
		bool isInitialized;

		GLFWwindow* window;
	};
	
	AssimpCamera* GetViewAssimpCamera();

	MyMath::mat4 GetViewMatrix();
}


#endif // GLFWEW_INCLUDED