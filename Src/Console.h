#pragma once
#ifndef CONSOLE_H_INCLUDE
#define CONSOLE_H_INCLUDE
#include <fstream>

class MyConsole
{
public:
	MyConsole()noexcept:fp(){}
	virtual ~MyConsole() = default;

	void Reopen(const char* path, const char* mode, FILE* std)
	{
		Close();
		::freopen_s(&fp, path, mode, std);
	}

	void Close()
	{
		if (fp)
		{
			::fclose(fp);
			fp = NULL;
		}
	}
protected:
	FILE* fp;
};


#endif // !CONSOLE_H_INCLUDE