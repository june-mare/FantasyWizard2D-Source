#pragma once
#ifndef METH_H_INCLUDE
#define METH_H_INCLUDE

#include "glad/glad.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Texture.h"

//先行宣言
class VertexArray;
using VertexArrayPtr = std::shared_ptr<VertexArray>;


namespace Mesh
{
#include "MyMath.h"
	
	//先行宣言
	class PrimitiveBuffer;
	using PrimitiveBufferPtr = std::shared_ptr<PrimitiveBuffer>;
	class MeshBuffer;
	using MeshBufferPtr = std::shared_ptr<MeshBuffer>;

	
	/*頂点データ形式*/
	struct Vertex
	{
		MyMath::vec3 position;
		MyMath::vec2 texcoord;
		MyMath::vec3 normal;
	};

	struct Material
	{
		std::string name = "<Default>";	//material名
		MyMath::vec4 baseColor = MyMath::vec4(1);		//基本色+アルファ
		float specularPower = 16;		//鏡面反射指数
		float normalizeFactor = 24.0f / 25.13274f; //正規化係数
		TexturePtr texBaseColor;		//基本テクスチャ
	};
	using MaterialPtr = std::shared_ptr<Material>;

	struct Primitive
	{
		GLenum mode = GL_TRIANGLES;	//プリミティブの種類
		GLsizei count = 0;			//描画するインデックス数
		const void* indices = 0;	//描画開始インデックスのバイトオフセット
		GLint baseVertex = 0;		//インデックス0となる頂点配列内の位置
		int materialNo = -1;		//material
	};

	void Draw(const Primitive& prim);

	/*プリミティブを管理するクラス*/
	class PrimitiveBuffer
	{
	private:
		std::vector<Primitive> primitives;
		VertexArrayPtr vao;

		GLuint vbo = 0;				//VBO管理番号
		uint8_t* pVbo = nullptr;	//VBOの書き込み先アドレス
		size_t vboCapacity;			//VBOの全容量(バイト)
		size_t vboSize = 0;			//VBOの使用済み容量(バイト)

		GLuint ibo = 0;				//IBO管理番号
		uint8_t* pIbo = nullptr;	//IBOの書き込み先デバイス
		size_t iboCapacity = 0;		//IBOの全容量
		size_t iboSize = 0;			//IBOの使用済み容量(バイト)

	public:

		//プリミティブバッファを作成する
		static PrimitiveBufferPtr Create(size_t vboCapacity, size_t iboCapacity)
		{
			return std::make_shared<PrimitiveBuffer>(vboCapacity, iboCapacity);
		}

		PrimitiveBuffer(size_t vboCapacity, size_t iboCapacity);
		virtual ~PrimitiveBuffer();

		PrimitiveBuffer(const PrimitiveBuffer&) = delete;
		PrimitiveBuffer& operator=(const PrimitiveBuffer&) = delete;

		void AddPrimitive(const Vertex* vertices, size_t VertexCount,
			const uint16_t* indices, size_t indexCount, GLenum mode = GL_TRIANGLES);

		const Primitive& GetPrimitive(size_t index)const { return primitives[index]; }

		VertexArrayPtr GetVAO()const { return vao; }

		void Clear();

		GLuint AddVertices(const Vertex* vertices, size_t count);

		const void* AddIndices(const uint16_t* indices, size_t count);
	};

	/*3Dモデル*/
	struct StaticMesh
	{
		std::string name;//メッシュ名
		std::vector<Primitive> primitive;
		std::vector<MaterialPtr> materials;
	};

	using StaticMethPtr = std::shared_ptr<StaticMesh>;
	void Draw(const StaticMesh& meth,GLuint program = 0);

	/*メッシュを管理するクラス*/
	class MeshBuffer
	{
	private:
		PrimitiveBuffer primitiveBuffer;
		std::unordered_map<std::string, StaticMethPtr> meshs;
	public:
		static MeshBufferPtr Create(size_t vboCapacity, size_t iboCapacity);
		
		MeshBuffer(size_t vboCapacity, size_t iboCapacity);
		virtual ~MeshBuffer() = default;

		StaticMethPtr LoadOBJ(const char* filename);
		VertexArrayPtr GetVAO()const;
		void Clear();
	};
};

#endif // !METH_H_INCLUDE
