#pragma once
#ifndef ASSIMPFILESYSTEM_H_INCLUDE

#include <string>
#include <cstdlib>

//�g�p����CMake�ׂ̈̃f�B���N�g���[
const char* logl_root = "${CMAKE_SOURCE_DIR}";

class AssimpFileSystem
{
private:
	typedef std::string(*Bulider)(const std::string& path);

public:
	static std::string getPath(const std::string path)
	{
		static std::string(*pathBulider)(std::string const&) = getPathBuilder();
		return (*pathBulider)(path);
	}

private:
	static std::string const& getRoot()
	{
		static char const* envRoot = getenv("LOGL_ROOT_PATH");
		static char const* giveRoot = (envRoot != nullptr ? envRoot  : logl_root);
		static std::string root = (giveRoot != nullptr ? giveRoot : "");
		return root;
	}



	static Bulider getPathBuilder()
	{
		//CMake�̓o�^�����邩�ǂ���
		if (getRoot() != "")
			return &AssimpFileSystem::getPathRelativeRoot;
		else
			return &AssimpFileSystem::getPathRelaticeBinary;
	}

	static std::string getPathRelativeRoot(const std::string& path)
	{
		return getRoot() + std::string("/") + path;
	}

	static std::string getPathRelaticeBinary(const std::string& path)
	{
		//�p�X��ʂ���{�̈ʒu
		return "../../../" + path;
	}
};



#endif // !ASSIMPFILESYSTEM_H_INCLUDE
