#pragma once
#ifndef ASSIMPSHADER_H_INCLUDE
#define ASSIMPSHADER_H_INCLUDE

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../GLContext.h"
#include "../MyMath.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class AssimpShader
{
private:
	void AssimpCheckCompileErrors(GLuint shader, std::string type);
	std::string& AssimpReadShader(const char* path);
public:
	//Žg—pID
	unsigned int id;

	AssimpShader(const char* vertexPath, const char* fragmentPath,const char* geometryPath = nullptr);
	virtual ~AssimpShader() = default;

	void Use();
	void SetBool(const std::string& name, bool value)const;
	void SetInt(const std::string& name, int value)const;	
	void SetFloat(const std::string& name, float value)const;
	void SetFloat(const std::string& name, float value1, float value2)const;
	void SetFloat(const std::string& name, MyMath::vec2& value)const;
	void SetFloat(const std::string& name, float value1, float value2,float value3)const;
	void SetFloat(const std::string& name, MyMath::vec3& value)const;
	void SetFloat(const std::string& name, float value1, float value2,float value3,float value4)const;
	void SetFloat(const std::string& name, MyMath::vec4& value)const;
	void SetMat2(const std::string& name, const MyMath::mat2& mat)const;
	void SetMat3(const std::string& name, const MyMath::mat3& mat)const;
	void SetMat4(const std::string& name, const MyMath::mat4& mat)const;
	void SetMat4(const std::string& name, const glm::mat4& mat)const;

};

#endif // !ASSIMPSHADER_H_INCLUDE