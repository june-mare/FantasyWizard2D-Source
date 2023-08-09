#include "ProgramPipeline.h"
#include <fstream>
#include <filesystem>
#include <vector>
#include "Debug.h"

void ReadTextFile(GLuint& value, GLuint type, const char* name)
{
	std::ifstream file;
	value = glCreateShader(type);
	file.open(name, std::ios::binary);//�t�@�C�����J��
	if (file)
	{
		/*���C���������̈���m��*/
		std::vector<char> s(std::filesystem::file_size(name));

		/*�t�@�C����ǂݍ���*/
		file.read(s.data(), s.size());
		s.push_back('\0');//NULL�I�[��ǉ�

		/*�\�[�X��ݒ肵�ăR���p�C��*/
		const char* a[] = { s.data() };
		glShaderSource(value, 1, a, nullptr);
		glCompileShader(value);

	}
	else
	{
		LOG_ERROR("%s���J���܂���", name);
	}
	file.close();
}

/*
@return �쐬�����v���O�����p�C�v���C���̃|�C���^
*/
ProgramPipelinePtr ProgramPipeline::Create(const std::string& filenameVS
	, const std::string& filenameFS)
{
	return std::make_shared<ProgramPipeline>(filenameVS,filenameFS);
}

ProgramPipeline::ProgramPipeline(const std::string& filenameVS,const std::string& filenameFS)
	:filenameVS(filenameVS),filenameFS(filenameFS)
{
	ReadTextFile(vs, GL_VERTEX_SHADER, filenameVS.c_str());

	/*�t���O�����g�V�F�[�_�[��ǂݍ���ŃR���p�C������*/
	ReadTextFile(fs, GL_FRAGMENT_SHADER, filenameFS.c_str());

	/*2�̃V�F�[�_�������N����*/
	if (vs && fs) {
		pp = glCreateProgram();
		glAttachShader(pp, vs);
		glAttachShader(pp, fs);
		glLinkProgram(pp);
	
		//�����N�̐��ۂ𒲂ׂ�
		GLint result;
		glGetProgramiv(pp, GL_LINK_STATUS, &result);
		if (result != GL_TRUE)
		{
			LOG_ERROR("�V�F�[�_�̃����N�Ɏ��s(vs = %s,fs = %s)",
				filenameVS.c_str(), filenameFS.c_str());
		}
	}
}

ProgramPipeline::~ProgramPipeline()
{
	glDeleteProgram(pp);
	glDeleteShader(vs);
	glDeleteShader(fs);
}
