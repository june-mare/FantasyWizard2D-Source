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
		/*GLFW����̃G���[�񍐂���������*/
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

	/*GLFW�̏�����*/
	bool Window::Init(int w, int h, const char* title)
	{
		/*���łɍ���Ă��邩�̊m�F*/
		if (isInitialized) {
		    ::std::cerr << "ERROR: GLFWEW�͊��ɏ���������Ă��܂�." << ::std::endl;
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
		�R�A�v���t�@�C���ŌÂ����̂ƐV�������̂̌݊������w��ł���
	 �@�@*/
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		/*
		GLFW_OPENGL_FORWARD_COMPAT
		�O���݊����̗L�����w��
		�u�O���݊����v�Ƃ����̂́u�߂������̃o�[�W�����ł����삷��ۏ؂�����v�Ƃ����Ӗ�
		*/
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		/*
		GLFW_CONTEXT_VERSION_MAJOR
		���p����OpenGL�̃��W���[�o�[�W����(�o�[�W�����ԍ��̃h�b�g�̑O��)���w�肷��
		�����4.5���g���̂� 4 ���w��
		*/
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		/*GLFW_CONTEXT_VERSION_MINOR
		���p����OpenGL�̃}�C�i�[�o�[�W����(�o�[�W�����ԍ��̃h�b�g�̌�둤)���w�肷��
		�����4.5���g���̂� 5 ���w��
		*/
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		/*
		GLFW_OPENGL_DEBUG_CONTEXT
		GLFW_TRUE ���w�肷��ƁAOpenGL�̃f�o�b�O�x���@�\���L���ɂȂ�
		���̋@�\�̓o�[�W����4.3�Œǉ����ꂽ�B
		*/
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);


		/*�`��E�B���h�E�̍쐬*/
		if (!window) {
			window = glfwCreateWindow(w, h, title, nullptr, nullptr);
			if (!window) {
				return false;
			}
			/*�R���e�L�X�g�̐���*/
			glfwMakeContextCurrent(window);
		}


		/*�o�C�g���̕ϐ��@�g�p���Ă��郌���_�[�ƃo�[�W�����������o���Ă���*/
		//const GLubyte* renderer = glGetString(GL_RENDERER);
		//std::cout << "Renderer: " << renderer << std::endl;
		//const GLubyte* version = glGetString(GL_VERSION);
		//std::cout << "Version: " << version << std::endl;
		
		isInitialized = true;
		return true;
	}

	/*GLFW�̏I����bool*/
	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(window);
	}

	/* �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@��؂�ւ���*/
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
