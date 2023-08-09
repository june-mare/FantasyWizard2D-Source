#include "AssimpShader.h"


std::string& AssimpShader::AssimpReadShader(const char* path)
{
	std::string code;
	std::ifstream file;

	file.exceptions(std::fstream::failbit | std::ifstream::badbit);

	try
	{
		file.open(path);
		std::stringstream stream;
		stream << file.rdbuf();
		file.close();
		code = stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return code;
}

void AssimpShader::AssimpCheckCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type;"
				<< type << std::endl << infoLog
				<< "\n---------------------------------" << std::endl;
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKNG_ERROR of type;" << type << "\n"
					<< infoLog << "\n--------------------------" << std::endl;
			}
		}
	}

}


AssimpShader::AssimpShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	unsigned int vertex, fragment, geometry = 0;
	const char* vShaderCode = AssimpReadShader(vertexPath).c_str();
	const char* fShaderCode = AssimpReadShader(fragmentPath).c_str();
	if (geometryPath != nullptr)
	{
		const char* gShaderCode = AssimpReadShader(geometryPath).c_str();
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		AssimpCheckCompileErrors(geometry, "GEOMETRY");
	}


	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	AssimpCheckCompileErrors(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	AssimpCheckCompileErrors(fragment, "FRAGMENT");

	//シェーダ側のプログラム
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	if (geometryPath != nullptr)
		glAttachShader(id, geometry);
	glLinkProgram(id);
	AssimpCheckCompileErrors(id, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
}



void AssimpShader::Use()
{
	glUseProgram(id);
}

void AssimpShader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

void AssimpShader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void AssimpShader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void AssimpShader::SetFloat(const std::string& name, float value1, float value2)const
{
	glUniform2f(glGetUniformLocation(id, name.c_str()), value1, value2);
}

void AssimpShader::SetFloat(const std::string& name, MyMath::vec2& value) const
{
	glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
}

void AssimpShader::SetFloat(const std::string& name, float value1, float value2, float value3) const
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), value1, value2,value3);
}

void AssimpShader::SetFloat(const std::string& name, MyMath::vec3& value) const
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y,value.z);
}

void AssimpShader::SetFloat(const std::string& name, float value1, float value2, float value3, float value4) const
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), value1, value2,value3,value4);
}

void AssimpShader::SetFloat(const std::string& name, MyMath::vec4& value) const
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), value.x, value.y,value.z,value.w);
}

void AssimpShader::SetMat2(const std::string& name, const MyMath::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat.m[0][0]);
}

void AssimpShader::SetMat3(const std::string& name, const MyMath::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat.m[0][0]);
}

void AssimpShader::SetMat4(const std::string& name, const MyMath::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat.m[0][0]);
}

void AssimpShader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
