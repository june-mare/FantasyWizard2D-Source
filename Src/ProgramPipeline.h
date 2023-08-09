#pragma once
#ifndef PROGRAMPIPELINE_H_INCLUDE
#define PROGRAMPIPELINE_H_INCLUDE

#include "glad/glad.h"
#include <memory>
#include <string>


//先行宣言
class ProgramPipeline;
using ProgramPipelinePtr = std::shared_ptr<ProgramPipeline>;

/*プログラムパイプラインを管理するクラス*/
class ProgramPipeline 
{
	GLuint vs = 0;	//頂点シェーダ
	GLuint fs = 0;	//フラグメントシェーダ
	GLuint pp = 0;	//プログラムパイプライン
	std::string filenameVS;//頂点シェーダファイル名
	std::string filenameFS;//フラグメントシェーダファイル名
public:
	static ProgramPipelinePtr Create(
		const std::string& filenameVS, const std::string& filenameFS);

	ProgramPipeline(const std::string& filenameVS,const std::string& filenameFS);
	~ProgramPipeline();

	ProgramPipeline(const ProgramPipeline&) = delete;
	ProgramPipeline& operator=(const ProgramPipeline&) = delete;

	//管理番号を取得
	operator GLuint() const{ return pp; }
	GLuint GetVS()const { return vs; }
	GLuint GetFS()const { return fs; }

};

#endif // !PROGRAMPIPELINE_H_INCLUDE
