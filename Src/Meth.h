#pragma once
#ifndef METH_H_INCLUDE
#define METH_H_INCLUDE

#include "glad/glad.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Texture.h"

//��s�錾
class VertexArray;
using VertexArrayPtr = std::shared_ptr<VertexArray>;


namespace Mesh
{
#include "MyMath.h"
	
	//��s�錾
	class PrimitiveBuffer;
	using PrimitiveBufferPtr = std::shared_ptr<PrimitiveBuffer>;
	class MeshBuffer;
	using MeshBufferPtr = std::shared_ptr<MeshBuffer>;

	
	/*���_�f�[�^�`��*/
	struct Vertex
	{
		MyMath::vec3 position;
		MyMath::vec2 texcoord;
		MyMath::vec3 normal;
	};

	struct Material
	{
		std::string name = "<Default>";	//material��
		MyMath::vec4 baseColor = MyMath::vec4(1);		//��{�F+�A���t�@
		float specularPower = 16;		//���ʔ��ˎw��
		float normalizeFactor = 24.0f / 25.13274f; //���K���W��
		TexturePtr texBaseColor;		//��{�e�N�X�`��
	};
	using MaterialPtr = std::shared_ptr<Material>;

	struct Primitive
	{
		GLenum mode = GL_TRIANGLES;	//�v���~�e�B�u�̎��
		GLsizei count = 0;			//�`�悷��C���f�b�N�X��
		const void* indices = 0;	//�`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g
		GLint baseVertex = 0;		//�C���f�b�N�X0�ƂȂ钸�_�z����̈ʒu
		int materialNo = -1;		//material
	};

	void Draw(const Primitive& prim);

	/*�v���~�e�B�u���Ǘ�����N���X*/
	class PrimitiveBuffer
	{
	private:
		std::vector<Primitive> primitives;
		VertexArrayPtr vao;

		GLuint vbo = 0;				//VBO�Ǘ��ԍ�
		uint8_t* pVbo = nullptr;	//VBO�̏������ݐ�A�h���X
		size_t vboCapacity;			//VBO�̑S�e��(�o�C�g)
		size_t vboSize = 0;			//VBO�̎g�p�ςݗe��(�o�C�g)

		GLuint ibo = 0;				//IBO�Ǘ��ԍ�
		uint8_t* pIbo = nullptr;	//IBO�̏������ݐ�f�o�C�X
		size_t iboCapacity = 0;		//IBO�̑S�e��
		size_t iboSize = 0;			//IBO�̎g�p�ςݗe��(�o�C�g)

	public:

		//�v���~�e�B�u�o�b�t�@���쐬����
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

	/*3D���f��*/
	struct StaticMesh
	{
		std::string name;//���b�V����
		std::vector<Primitive> primitive;
		std::vector<MaterialPtr> materials;
	};

	using StaticMethPtr = std::shared_ptr<StaticMesh>;
	void Draw(const StaticMesh& meth,GLuint program = 0);

	/*���b�V�����Ǘ�����N���X*/
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
