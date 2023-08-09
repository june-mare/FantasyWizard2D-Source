#pragma once
#ifndef ASSIMPENTITY_H_INCLUDE
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include <list>
#include <array>
#include <memory>

class AssimpTransform
{
protected:
	//ローカル情報
	glm::vec3 pos = { 0.0f,0.0f,0.0f };
	glm::vec3 eulerRot = { 0.0f,0.0f,0.0f };//回転軸
	glm::vec3 scale = { 1.0f,1.0f,1.0f };

	//グローバル情報用のマトリクス
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	//スケールがいじられたかどうか
	bool isDirty = false;

protected:
	glm::mat4 getLocalModelMatrix()
	{
		const glm::mat4 transformX =
			glm::rotate(
				glm::mat4(1.0f),
				glm::radians(eulerRot.x),
				glm::vec3(1.0f, 0.0f, 0.0f)
			);
		const glm::mat4 transformY =
			glm::rotate(
				glm::mat4(1.0f),
				glm::radians(eulerRot.y),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);
		const glm::mat4 transformZ =
			glm::rotate(
				glm::mat4(1.0f),
				glm::radians(eulerRot.z),
				glm::vec3(0.0f, 0.0f, 1.0f)
			);

		const glm::mat4 roationMatrix = transformY * transformX * transformZ;

		return glm::translate(glm::mat4(1.0f), pos) * roationMatrix * glm::scale(glm::mat4(1.0f), scale);
	}

public:

	void computeModelMatrix()
	{
		modelMatrix = getLocalModelMatrix();
	}
	//override 親モデルのマトリクスぶん回す
	void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
	{
		modelMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
	}

	void setLocalPosition(const glm::vec3& newPosition)
	{
		pos = newPosition;
		isDirty = true;
	}

	void setLocalRotation(const glm::vec3& newRotation)
	{
		eulerRot = newRotation;
		isDirty = true;
	}

	void setLocalScale(const glm::vec3& newScale)
	{
		scale = newScale;
		isDirty = true;
	}

	const glm::vec3& getGlobalPosition()const
	{
		return modelMatrix[3];
	}

	const glm::vec3& getLocalPosition()const
	{
		return pos;
	}

	const glm::vec3& getLocalRotation()const
	{
		return eulerRot;
	}
	const glm::vec3& getLocalScale()const
	{
		return scale;
	}
	const glm::mat4& getModelMatrix()const
	{
		return modelMatrix;
	}
	glm::vec3 getRight()const
	{
		return modelMatrix[0];
	}
	glm::vec3 getUp()const
	{
		return modelMatrix[1];
	}
	glm::vec3 getBackward()const
	{
		return modelMatrix[2];
	}
	glm::vec3 getForward()const
	{
		return -modelMatrix[2];
	}
	glm::vec3 getGlobalScale()const
	{
		return { glm::length(getRight),glm::length(getUp()),glm::length(getBackward) };
	}
	bool isDirty()const
	{
		return isDirty;
	}
};

//当たり判定をとるための部品
struct Plane
{
	glm::vec3 normal = { 0.0f,1.0f,0.0f };//単位ベクトル
	float distance = 0.0f;

	Plane() = default;
	Plane(const glm::vec3& P1, const glm::vec3& norm)
		:normal(glm::normalize(norm)), distance(glm::dot(normal, P1))
	{}
	float getSignedDistanceToPlan(const glm::vec3& point)const
	{
		return glm::dot(normal, point) - distance;
	}
};

//全方向の頂点位置
struct Frustum
{
	Plane topFace;
	Plane bottomFace;

	Plane rightFace;
	Plane leftFace;

	Plane farFace;
	Plane nearFace;
};

//範囲
struct BoundingVolume
{
	virtual bool isOnFrustum(const Frustum& camFrustum,
		const AssimpTransform& transform) const = 0;

	virtual bool isOnOrForwardPlan(const Plane& plan)const = 0;

	//各々の面に衝突しているかの確認(形による)
	bool isOnFrustum(const Frustum& camFrustum)const
	{
		return (isOnOrForwardPlan(camFrustum.leftFace) &&
			isOnOrForwardPlan(camFrustum.rightFace) &&
			isOnOrForwardPlan(camFrustum.topFace) &&
			isOnOrForwardPlan(camFrustum.bottomFace) &&
			isOnOrForwardPlan(camFrustum.nearFace) &&
			isOnOrForwardPlan(camFrustum.farFace)
			);
	}
};

struct Sphere : public BoundingVolume
{
	glm::vec3 center{ 0.0f, 0.0f, 0.0};
	float radius{ 0.0f };

	Sphere(const glm::vec3& inCenter,float inRadius)
		:BoundingVolume{},center{inCenter},radius(inRadius)
	{}

	bool isOnOrForwardPlan(const Plane& plan)const final
	{
		return plan.getSignedDistanceToPlan(center) > -radius;
	}

	bool isOnFrustum(const Frustum& camFrustum, const AssimpTransform& transform)const final
	{
		//グローバル座標系のサイズに変える
		const glm::vec3 globalScale = transform.getGlobalScale();
		
		//変換するために使用する物のセンターポジションを取得
		const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center,1.0f) };

		//形状を正しく囲うには最大スケールの上方が必要
		const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

		//上記で取得したものは直径に当たるので半分にしる
		Sphere globalSphere(globalCenter, radius * (maxScale * 0.5));

		//失敗の確率が高いものを先行させることで処理を軽くする
		return (
				globalSphere.isOnOrForwardPlan(camFrustum.leftFace) &&
				globalSphere.isOnOrForwardPlan(camFrustum.rightFace) &&
				globalSphere.isOnOrForwardPlan(camFrustum.farFace) &&
				globalSphere.isOnOrForwardPlan(camFrustum.nearFace) &&
				globalSphere.isOnOrForwardPlan(camFrustum.topFace) &&	
				globalSphere.isOnOrForwardPlan(camFrustum.bottomFace)	
			);
	}
};

struct SquareAABB :
	public BoundingVolume
{
	glm::vec3 center{ 0.0f,0.0f,0.0f };
	float extent{ 0.0f };
	SquareAABB(const glm::vec3& inCenter, float inExtent)
		: BoundingVolume{}, center{ inCenter }, extent{ inExtent }
	{}
	virtual ~SquareAABB() = default;

	bool isOnOrForwardPlan(const Plane& plan)const override final
	{
		//投影間隔半径を計算する
		const float r = extent *
			(std::abs(plan.normal.x) + std::abs(plan.normal.y) + std::abs(plan.normal.z));
		return -r <= plan.getSignedDistanceToPlan(center);
	}

	bool isOnFrustum(const Frustum& camFrustum,
		const AssimpTransform& transform)const override final
	{
		//グローバル座標系のサイズに変える
		const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center,1.0f) };

		//各大きさを借入する
		const glm::vec3 right = transform.getRight() * extent;
		const glm::vec3 up = transform.getUp() * extent;
		const glm::vec3 forward = transform.getForward() * extent;

		const float newIi = std::abs(glm::dot(glm::vec3{ 1.0f,0.0f,0.0f }, right))
			+ std::abs(glm::dot(glm::vec3{ 1.0f,0.0f,0.0f }, up))
			+ std::abs(glm::dot(glm::vec3{ 1.0f,0.0f,0.0f }, forward));
				
		const float newIj = std::abs(glm::dot(glm::vec3{ 0.0f,1.0f,0.0f }, right))
			+ std::abs(glm::dot(glm::vec3{ 0.0f,1.0f,0.0f }, up))
			+ std::abs(glm::dot(glm::vec3{ 0.0f,1.0f,0.0f }, forward));

		const float newIk = std::abs(glm::dot(glm::vec3{ 0.0f,0.0f,1.0f }, right))
			+ std::abs(glm::dot(glm::vec3{ 0.0f,0.0f,1.0f }, up))
			+ std::abs(glm::dot(glm::vec3{ 0.0f,0.0f,1.0f }, forward));

		const SquareAABB globalAABB(globalCenter, std::max(std::max(newIi, newIj), newIk));

		return globalAABB.isOnOrForwardPlan(camFrustum.leftFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.rightFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.topFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.bottomFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.nearFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.farFace);
	}
};

struct AABB : public BoundingVolume
{
	glm::vec3 center{ 0.0f,0.0f,0.0f };
	glm::vec3 extent{ 0.0f,0.0f,0.0f };

	AABB(const glm::vec3& min,const glm::vec3& max)
		:BoundingVolume{}, center{ (max + min) * 0.5f}
		, extent{ max.x - center.x, max.y - center.y, max.z - center.z}
	{}
	AABB(const glm::vec3& inCenter,float iI,float iJ,float iK)
		:BoundingVolume{},center{inCenter},extent{iI,iJ,iK}
	{}

	std::array<glm::vec3, 8>getVertice()const
	{
		std::array<glm::vec3, 8>vertice;
		vertice[0] = { center.x - extent.x,center.y - extent.y,center.z - extent.z };
		vertice[1] = { center.x + extent.x,center.y - extent.y,center.z - extent.z };
		vertice[2] = { center.x - extent.x,center.y + extent.y,center.z - extent.z };
		vertice[3] = { center.x + extent.x,center.y + extent.y,center.z - extent.z };
		vertice[4] = { center.x - extent.x,center.y - extent.y,center.z + extent.z };
		vertice[5] = { center.x + extent.x,center.y - extent.y,center.z + extent.z };
		vertice[6] = { center.x - extent.x,center.y + extent.y,center.z + extent.z };
		vertice[7] = { center.x + extent.x,center.y + extent.y,center.z + extent.z };

		return vertice;
	}

	bool isOnOrForwardPlan(const Plane& plan)const override final
	{
		const float r = extent.x * std::abs(plan.normal.x) +
			extent.y * std::abs(plan.normal.y) +
			extent.z * std::abs(plan.normal.z);
		return -r <= plan.getSignedDistanceToPlan(center);
	}

	bool isOnFrustum(const Frustum& camFrustum,
		const AssimpTransform& transform)const override final
	{
		const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(center,1.0f) };

		const glm::vec3 right = transform.getRight() * extent.x;
		const glm::vec3 up = transform.getUp() * extent.y;
		const glm::vec3 forward = transform.getForward() * extent.z;

		const float newIi =
			std::abs(glm::dot(glm::vec3{ 1.0f,0.0f,0.0f }, right))
			+ std::abs(glm::dot(glm::vec3{ 1.0f,0.0f,0.0f }, up))
			+ std::abs(glm::dot(glm::vec3{ 1.0f,0.0f,0.0f }, forward));

		const float newIj = 
			std::abs(glm::dot(  glm::vec3{ 0.0f,1.0f,0.0f }, right))
			+ std::abs(glm::dot(glm::vec3{ 0.0f,1.0f,0.0f }, up))
			+ std::abs(glm::dot(glm::vec3{ 0.0f,1.0f,0.0f }, forward));
			 
		const float newIk =
			std::abs(glm::dot(  glm::vec3{ 0.0f,0.0f,1.0f }, right))
			+ std::abs(glm::dot(glm::vec3{ 0.0f,0.0f,1.0f }, up))
			+ std::abs(glm::dot(glm::vec3{ 0.0f,0.0f,1.0f }, forward));

		const AABB globalAABB(globalCenter, newIi, newIj, newIk);

		return (globalAABB.isOnOrForwardPlan(camFrustum.leftFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.rightFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.topFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.bottomFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.nearFace) &&
			globalAABB.isOnOrForwardPlan(camFrustum.farFace));
	}

};



//Frustum createFrustumFromCamera(const AssimpCamera& cam)

#endif