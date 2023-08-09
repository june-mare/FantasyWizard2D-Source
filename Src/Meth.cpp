#define _CRT_SECURE_NO_WARNINGS
#include "Meth.h"
#include "VertexArray.h"
#include "Debug.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <stdio.h>

namespace Mesh {

	/*���Ă��邱��*/
	/*
	�X�v���C�g�p��VBO�AIBO�AVAO���쐬����v���O�����ƂقƂ�Ǔ����ňႢ��
	���Ƃ���f�[�^��ǉ��ł���悤��[�R�s�[��A�h���X]���擾���Ă���_�ł�
	*/
	PrimitiveBuffer::PrimitiveBuffer(size_t vboCapacity, size_t iboCapacity)
	{
		const GLenum flags =
			GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT;
		/*COHERENT �؂̒ʂ��� | PERSISTENT �����I | WRITE ����*/

	/*VBO���쐬*/
		glCreateBuffers(1, &vbo);
		glNamedBufferStorage(vbo, vboCapacity, nullptr, flags);
		pVbo = static_cast<uint8_t*>(glMapNamedBufferRange(vbo, 0, vboCapacity, flags));

		/*IBO���쐬*/
		glCreateBuffers(1, &ibo);
		glNamedBufferStorage(ibo, iboCapacity, nullptr, flags);
		pIbo = static_cast<uint8_t*>(glMapNamedBufferRange(ibo, 0, iboCapacity, flags));

		/*VAO���쐬���AOpenGL�R���e�L�X�g�Ɋ��蓖�Ă�*/
		vao = VertexArray::Create();
		glBindVertexArray(*vao);

		/*VAO�ɁAVBO��IBO�����蓖�Ă�*/
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		/*���_�A�g���r���[�g��ݒ�*/
		const auto setAttribute = [](GLuint index, GLint size, size_t offset)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				reinterpret_cast<void*>(offset));
		};
		setAttribute(0, 3, offsetof(Vertex, position));
		setAttribute(1, 2, offsetof(Vertex, texcoord));
		setAttribute(2, 3, offsetof(Vertex, normal));
		/*OPENGL�R���e�L�X�g�ւ̊��蓖�Ă�����*/
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	PrimitiveBuffer::~PrimitiveBuffer()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
		/*vao�̓N���X�Ǘ��̈ז����I�ɍ폜����K�v�Ȃ�*/
	}

	/*
	�v���~�e�B�u��ǉ�����
	@param vertices GPU�߂���ɃR�s�[���钸�_�f�[�^�z��
	@param veertexCount vertices�̗v�f��
	@param indices GPU�������ɃR�s�[����C���f�b�N�X�f�[�^�z��
	@param indexCount indices�̗v�f��
	@oaram mode �v���~�e�B�u�̎��
	*/
	void PrimitiveBuffer::AddPrimitive(
		const Vertex* vertices, size_t VertexCount,
		const uint16_t* indices, size_t indexCount,
		GLenum mode)
	{
		Primitive prim;
		prim.mode = mode;
		prim.count = static_cast<GLsizei>(indexCount);
		prim.indices = AddIndices(indices, indexCount);
		prim.baseVertex = AddVertices(vertices, VertexCount);
		primitives.push_back(prim);
	}

	void PrimitiveBuffer::Clear()
	{
		primitives.clear();
		vboSize = 0;
		iboSize = 0;
	}

	GLuint PrimitiveBuffer::AddVertices(const Vertex* vertices, size_t count)
	{
		const GLint offset = static_cast<GLint>(vboSize / sizeof(Vertex));

		const size_t verticesSize = count * sizeof(Vertex);
		memcpy(pVbo + vboSize, vertices, verticesSize);
		vboSize += verticesSize;

		return offset;
	}

	const void* PrimitiveBuffer::AddIndices(const uint16_t* indices, size_t count)
	{
		const void* offset = reinterpret_cast<void*>(iboSize);

		const size_t indexSize = count * sizeof(uint16_t);
		memcpy(pIbo + iboSize, indices, indexSize);
		iboSize += indexSize;

		return offset;
	}

	void Draw(const Primitive& prim)
	{
		glDrawElementsInstancedBaseVertex(prim.mode, prim.count,
			GL_UNSIGNED_SHORT, prim.indices, 1, prim.baseVertex);
	}

	void Draw(const StaticMesh& meth, GLuint program)
	{
		for (const auto& prim : meth.primitive)
		{
			//�}�e���A����ݒ�
			if (prim.materialNo >= 0 && prim.materialNo < meth.materials.size())
			{
				const Material& material = *meth.materials[prim.materialNo];
				if (program)
				{
					glProgramUniform4fv(program, 102, 1, &material.baseColor.x);
					glProgramUniform2f(program, 103,
						material.specularPower, material.normalizeFactor);
				}
				if (material.texBaseColor)
				{
					const GLuint tex = *material.texBaseColor;
					glBindTextures(0, 1, &tex);
				}
				else
				{
					glBindTextures(0, 1, nullptr);
				}
			}

			Draw(prim);
		}
	}

	MeshBuffer::MeshBuffer(size_t vboCapacity, size_t iboCapacity)
		:primitiveBuffer(vboCapacity, iboCapacity)
	{}

	std::vector<MaterialPtr> LoadMTL(
		const std::string& foldername, const char* filename)
	{
		//MTL�t�@�C�����J��
		const std::string fullpath = foldername + filename;
		std::ifstream file(fullpath);
		if (!file)
		{
			LOG_ERROR("%s���J���܂���", fullpath.c_str());
			return {};
		}

		//MTL�t�@�C������͂���
		std::vector<MaterialPtr> materials;
		MaterialPtr pMaterial;
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);

			//material�̒�`��ǂݎ������݂�
			char name[1000] = { 0 };
			if (sscanf(line.data(), " newmtl %999s", name) == 1)
			{
				pMaterial = std::make_shared<Material>();
				pMaterial->name = name;
				materials.push_back(pMaterial);
				continue;
			}

			//material����`����Ă��Ȃ��ꍇ�͖�������
			if (!pMaterial)
			{
				continue;
			}

			//��{�F�̓ǂݎ������݂�
			if (sscanf(line.data(), " Kd %f %f %f",
				&pMaterial->baseColor.x,
				&pMaterial->baseColor.y,
				&pMaterial->baseColor.z) == 3)
			{
				continue;
			}

			if (sscanf(line.data(), " d %f", &pMaterial->baseColor.w) == 1)
			{
				continue;
			}


			//���ʔ��ː��̓ǂݎ������݂�
			if (sscanf(line.data(), " Ns %f", &pMaterial->specularPower) == 1)
			{
				//���K���W�������߂�
				pMaterial->normalizeFactor =
					(pMaterial->specularPower + 8) / (8 * M_PI);
				continue;
			}

			//��{�F�e�N�X�`�����̓ǂݎ������݂�
			char textureName[1000] = { 0 };
			if (sscanf(line.data(), " map_Kd %999s", &textureName) == 1)
			{
				pMaterial->texBaseColor =
					Texture::Create(foldername + textureName, GL_LINEAR);
				continue;
			}
		}

		//�ǂݍ���material�̔z���Ԃ�
		return materials;
	}

	StaticMethPtr MeshBuffer::LoadOBJ(const char* filename)
	{
		/*�ȑO�ɓǂݍ��񂾃t�@�C���Ȃ�A�쐬�ς݂̃��b�V����Ԃ�*/
		auto itr = meshs.find(filename);
		if (itr != meshs.end())
		{
			return itr->second;
		}
		/*OBJ�t�@�C�����J��*/
		std::ifstream file(filename);
		if (!file)
		{
			LOG_ERROR(" %s���J���܂���", filename);
			return nullptr;
		}

		//�t�H���_�����擾����
		std::string foldername(filename);
		{
			const size_t p = foldername.find_last_of("\\/");
			if (p != std::string::npos)
			{
				foldername.resize(p + 1);
			}
		}

		/*OBJ�t�@�C������͂��āA���_�f�[�^�ƃC���f�b�N�X�f�[�^��ǂݍ���*/
		struct IndexSet { int v, vt, vn; };
		struct UseMaterial
		{
			std::string name;		//�}�e���A����
			size_t startOffset;		//���蓖�Ĕ͈͂̐擪�ʒu
		};

		std::vector<MyMath::vec3> positions;
		std::vector<MyMath::vec2> texcoords;
		std::vector<MyMath::vec3> normals;
		std::vector<IndexSet>faceIndexSet;
		std::vector<MaterialPtr> materials;
		std::vector<UseMaterial> usemtls;

		positions.reserve(20'000);
		texcoords.reserve(20'000);
		normals.reserve(20'000);
		faceIndexSet.reserve(20'000 * 3);
		materials.reserve(100);
		usemtls.reserve(100);

		usemtls.push_back({ std::string(),0 });

		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			const char* p = line.c_str();

			/*���_���W�̎�荞�݂����݂�*/
			MyMath::vec3 v;
			if (sscanf(p, " v %f %f %f", &v.x, &v.y, &v.z) == 3)
			{
				positions.push_back(v);
				continue;
			}

			//�e�N�X�`�����W�̓ǂݎ������݂�
			MyMath::vec2 vt;
			if (sscanf(p, " vt %f %f", &vt.x, &vt.y) == 2)
			{
				texcoords.push_back(vt);
				continue;
			}

			/*�@���̓ǂݎ������݂�*/
			MyMath::vec3 vn;
			if (sscanf(p, " vn %f %f %f", &vn.x, &vn.y, &vn.z) == 3)
			{
				normals.push_back(vn);
				continue;
			}

			IndexSet f0, f1, f2;
			int readByte;

			/*���_���W + �e�N�X�`�����W + �@��*/
			if (sscanf(p, " f %u/%u/%u %u/%u/%u%n",
				&f0.v, &f0.vt, &f0.vn, &f1.v, &f1.vt, &f1.vn, &readByte) == 6)
			{
				p += readByte; // �ǂݎ��ʒu���X�V
				for (;;) {
					if (sscanf(p, " %u/%u/%u%n", &f2.v, &f2.vt, &f2.vn, &readByte) != 3) {
						break;
					}
					p += readByte; // �ǂݎ��ʒu���X�V
					faceIndexSet.push_back(f0);
					faceIndexSet.push_back(f1);
					faceIndexSet.push_back(f2);
					f1 = f2; // ���̎O�p�`�̂��߂Ƀf�[�^���ړ�                
				}
				continue;
			}

			/*���_���W + �e�N�X�`��*/
			if (sscanf(p, " f %u/%u %u/%u%n",
				&f0.v, &f0.vt, &f1.v, &f1.vt, &readByte) == 4)
			{
				f0.vn = f1.vn = 0; //�@���Ȃ�
				p += readByte;
				for (;;)
				{
					if (sscanf(p, " %u/%u%n", &f2.v, &f2.vt, &readByte) != 2)
					{
						break;
					}
					f2.vn = 0;
					p += readByte;//�ǂݎ��ʒu���X�V
					faceIndexSet.push_back(f0);
					faceIndexSet.push_back(f1);
					faceIndexSet.push_back(f2);
					f1 = f2;    //���̎O�p�`�̂��߂Ƀf�[�^���ړ�
				}
				continue;
			}
			//MTL�t�@�C���̓ǂݎ������݂�
			char mtlFilename[1000];
			if (sscanf(line.data(), " mtllib %999s", mtlFilename))
			{
				const auto tmp = LoadMTL(foldername, mtlFilename);
				materials.insert(materials.end(), tmp.begin(), tmp.end());
				continue;
			}


			//�g�p�}�e���A�����̓ǂݎ������݂�
			char mtlName[1000];
			if (sscanf(line.data(), " usemtl %999s", mtlName) == 1)
			{
				//�O��ƈقȂ�}�e���A�����w�肳�ꂽ���̂݁A�V�����}�e���A���͈͂�ǉ�
				if (usemtls.back().name != mtlName)
				{
					usemtls.push_back({ mtlName,faceIndexSet.size() });
				}
				continue;
			}


		}

		//�����ɔԕ���ǉ�
		usemtls.push_back({ std::string(),faceIndexSet.size() });



		/*�ǂݍ��񂾃f�[�^���AOpenGL�Ŏg����f�[�^�ɕϊ�*/
		std::vector<Vertex> vertices;
		vertices.reserve(faceIndexSet.size());
		for (const auto& e : faceIndexSet)
		{
			Vertex v;
			v.position = positions[e.v - 1];
			v.texcoord = texcoords[e.vt - 1];
			if (e.vn == 0)
			{
				v.normal = MyMath::vec3(0.0f, 0.0f, 1.0f);
			}
			else
			{
				v.normal = normals[e.vn - 1];
			}
			vertices.push_back(v);
		}
		std::vector<uint16_t> indices(faceIndexSet.size());
		for (int i = 0; i < indices.size(); i++)
		{
			indices[i] = i;
		}

		/*�ϊ������f�[�^���v���~�e�B�u�o�b�t�@�ɒǉ�*/
		const GLint baseVertex =
			primitiveBuffer.AddVertices(vertices.data(), vertices.size());
		const void* indexOffset =
			primitiveBuffer.AddIndices(indices.data(), indices.size());

		/*���b�V�����쐬*/
		auto pMesh = std::make_shared<StaticMesh>();
		pMesh->name = filename;

		//�v���~�e�B�u���쐬
		size_t i = 0;
		if (usemtls.size() > 2)
		{
			i = 1;//���f�[�^�Ɣԕ��ȊO�̃}�e���A��������ꍇ�A���f�[�^���΂�
		}
		for (; i < usemtls.size() - 1; ++i)
		{
			const UseMaterial& cur = usemtls[i];
			const UseMaterial& next = usemtls[i + 1];

			if (next.startOffset == cur.startOffset)
			{
				continue;
			}


			Primitive prim;
			prim.mode = GL_TRIANGLES;
			prim.count = static_cast<GLsizei>
				(next.startOffset - cur.startOffset);
			prim.indices = indexOffset;
			prim.baseVertex = baseVertex;
			prim.materialNo = 0;//�f�t�H���g�l��ݒ�
			for (int i = 0; i < materials.size(); i++)
			{
				if (materials[i]->name == cur.name)
				{
					prim.materialNo = i;//���O�̈�v����material��ݒ�
					break;
				}
			}
			pMesh->primitive.push_back(prim);

			//�C���f�b�N�X�I�t�Z�b�g��i�߂�
			indexOffset = reinterpret_cast<void*>(
				reinterpret_cast<size_t>(indexOffset) + sizeof(uint16_t) * prim.count
				);
		}

		//�}�e���A���z�񂪋�̏ꍇ�A�f�t�H���gmaterial��ǉ�
		if (materials.empty())
		{
			pMesh->materials.push_back(std::make_shared<Material>());
		}
		else
		{
			pMesh->materials.assign(materials.begin(), materials.end());

		}


		meshs.emplace(pMesh->name, pMesh);
		LOG("%s��ǂݍ��݂܂���", filename);



		/*�쐬�������b�V����Ԃ�*/
		return pMesh;
	}


	VertexArrayPtr MeshBuffer::GetVAO() const
	{
		return primitiveBuffer.GetVAO();
	}

	void MeshBuffer::Clear()
	{
		meshs.clear();
		primitiveBuffer.Clear();

	}

	MeshBufferPtr MeshBuffer::Create(size_t vboCapacity, size_t iboCapacity)
	{
		return std::make_shared<MeshBuffer>(vboCapacity, iboCapacity);
	}
}