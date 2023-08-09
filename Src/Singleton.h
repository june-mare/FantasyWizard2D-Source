#pragma once
#include <cassert>
#include <iostream>
#define _SINGLETON_

#ifdef _SINGLETON_ 
/*
* �V���O���g���̃e���v���[�g
*/
template <typename T>
class Singleton 
{
	//���[�u�R���X�g���N�^�h�~
	Singleton(const Singleton& obj) = delete;   
	Singleton(Singleton&&) = delete;

	//�R�s�[�R���X�g���N�^�֎~
	void operator=(const Singleton& obj) = delete;
	void operator=(const Singleton&&) = delete;

	static T* instance;
protected:
	/*
	* assert �����̒l���U�������ꍇ�v���O�������I�������G���[���o��
	*/
	Singleton()
	{
		assert(!instance);
		//�A�h���X�̈ʒu���쐬
		int offset = (int)(T*)1 - (int)(Singleton<T>*)(T*)1;
		instance = (T*)((int)this + offset);
	}
	virtual ~Singleton()
	{
		assert(instance);
		instance = nullptr;
	}
public:	
	static T& GetInstance(void)
	{
		assert(instance);
		return *instance;
	}
};
template <typename T> T* Singleton<T>::instance = nullptr;

#else
/*
  �V���O���g���̃e���v���[�g�p�^�[��2
*/
template <typename T>
class Singleton
{
public:
	static inline T* GetInstance()
	{
		static T instance;
		return &instance;
	}

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	void operator=(const Singleton& obj) {}	   //�R�s�[�������̏����Ȃ�
	void operator=(const Singleton&&) = delete;//�R�s�[�R���X�g���N�^�h�~
	Singleton(const Singleton& obj) {}		   //���[�u�������̏����Ȃ�
	Singleton(const Singleton&&) = delete;	   //���[�u�R���X�g���N�^�h�~
};

#endif // DYNAMIC_SINGLETON
