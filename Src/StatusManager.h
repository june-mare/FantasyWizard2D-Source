#pragma once
#include "Status.h"
#include "Singleton.h"
#include <unordered_map>

class StatusManager :public Singleton<StatusManager>
{
	unordered_map<string , StatusPtr> StatusList;
public:
	StatusManager():StatusList(){}
	virtual ~StatusManager() { StatusList.clear(); }

	/*
	  �o�^
	  name�@���O
	  status �X�e�[�^�X�̒l
	*/
	void Register(const string& name, StatusPtr status)
	{
		StatusList[name] = status;
	}

	//�X�e�[�^�X�|�C���^�[��Ԃ�
	StatusPtr GetStatus(const string& name)
	{
		if (StatusList.find(name) != StatusList.end())
		{
			return StatusList[name];
		}
		return nullptr;
	}
};