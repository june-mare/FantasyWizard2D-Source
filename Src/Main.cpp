#include "Engine.h"

INT WINAPI WinMain(
	_In_ HINSTANCE hInstance,			/*Windows������������C���X�^���X�ɂ���*/
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR IpCmdLine,
	_In_ INT nShowCmd)
{
	Engine engine;
	return engine.Run();
}