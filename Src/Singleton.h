#pragma once
#include <cassert>
#include <iostream>
#define _SINGLETON_

#ifdef _SINGLETON_ 
/*
* シングルトンのテンプレート
*/
template <typename T>
class Singleton 
{
	//ムーブコンストラクタ防止
	Singleton(const Singleton& obj) = delete;   
	Singleton(Singleton&&) = delete;

	//コピーコンストラクタ禁止
	void operator=(const Singleton& obj) = delete;
	void operator=(const Singleton&&) = delete;

	static T* instance;
protected:
	/*
	* assert 引数の値が偽だった場合プログラムを終了させエラーを出す
	*/
	Singleton()
	{
		assert(!instance);
		//アドレスの位置を作成
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
  シングルトンのテンプレートパターン2
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
	void operator=(const Singleton& obj) {}	   //コピー生成時の処理なし
	void operator=(const Singleton&&) = delete;//コピーコンストラクタ防止
	Singleton(const Singleton& obj) {}		   //ムーブ生成時の処理なし
	Singleton(const Singleton&&) = delete;	   //ムーブコンストラクタ防止
};

#endif // DYNAMIC_SINGLETON
