#include "ProgramPipeline.h"
#include <fstream>
#include <filesystem>
#include <vector>
#include "Debug.h"

void ReadTextFile(GLuint& value, GLuint type, const char* name)
{
	std::ifstream file;
	value = glCreateShader(type);
	file.open(name, std::ios::binary);//ファイルを開く
	if (file)
	{
		/*メインメモリ領域を確保*/
		std::vector<char> s(std::filesystem::file_size(name));

		/*ファイルを読み込む*/
		file.read(s.data(), s.size());
		s.push_back('\0');//NULL終端を追加

		/*ソースを設定してコンパイル*/
		const char* a[] = { s.data() };
		glShaderSource(value, 1, a, nullptr);
		glCompileShader(value);

	}
	else
	{
		LOG_ERROR("%sを開けません", name);
	}
	file.close();
}

/*
@return 作成したプログラムパイプラインのポインタ
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

	/*フラグメントシェーダーを読み込んでコンパイルする*/
	ReadTextFile(fs, GL_FRAGMENT_SHADER, filenameFS.c_str());

	/*2つのシェーダをリンクする*/
	if (vs && fs) {
		pp = glCreateProgram();
		glAttachShader(pp, vs);
		glAttachShader(pp, fs);
		glLinkProgram(pp);
	
		//リンクの成否を調べる
		GLint result;
		glGetProgramiv(pp, GL_LINK_STATUS, &result);
		if (result != GL_TRUE)
		{
			LOG_ERROR("シェーダのリンクに失敗(vs = %s,fs = %s)",
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
