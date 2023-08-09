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
	  登録
	  name　名前
	  status ステータスの値
	*/
	void Register(const string& name, StatusPtr status)
	{
		StatusList[name] = status;
	}

	//ステータスポインターを返す
	StatusPtr GetStatus(const string& name)
	{
		if (StatusList.find(name) != StatusList.end())
		{
			return StatusList[name];
		}
		return nullptr;
	}
};