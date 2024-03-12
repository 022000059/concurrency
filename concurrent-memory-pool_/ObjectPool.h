#pragma once
#pragma once
#pragma once
#include<iostream>
#include<vector>
#include <new>
#include<time.h>
using std::cout;
using std::endl;
using std::bad_alloc;

template<class T>
class ObjectPool
{
public:
	T* New()//没有*会报什么错?
	{
		T * obj = nullptr;

		if (_freeList)
		{//优先使用还回来的内存块
			void* next = *((void **)_freeList);//	取freelist的头4或8字节
			obj =(T*) _freeList;
			_freeList = next;//定长内存池，所以每次取得大小一样


			//return obj;

		}
		else
		{//freelist为空
			//剩余内存不够一个对象大小的时候，则重新开大块新空间
			if (_remainBytes < sizeof(T))
			{
				_remainBytes= 128 * 1024;
				_memory = (char*)malloc(_remainBytes);
				if (_memory == nullptr)
				{
					throw bad_alloc();

				}
			}
			obj = (T*)_memory;
			//至少开一个指针的大小
			size_t objSize = sizeof(T);
			if (objSize < sizeof(void*))objSize = sizeof(void*);

			_memory += objSize;
			_remainBytes -= objSize;
			//return obj;

		}
		//定位new，显示调用T的构造函数初始化
		new(obj)T;
		return obj;
	}

	void Delete(T *obj)
	{
		//if (_freeList == nullptr)
		//{
		//	_freeList = obj;
		//	*(void **)obj = NULL;

		//}
		//else
		//{
			//头插法把obj插入_freelist
		obj->~T();
		*(void **)obj = _freeList;
		_freeList = obj;
		//}
	}

private:
	char* _memory = nullptr;//指向大块内存的指针
	size_t _remainBytes = 0;//大块内存剩余的大小
	void* _freeList = nullptr;//换回来过程中链接的自由链表的头指针

};

struct TreeNode
{
	int _val;
	TreeNode* _left;
	TreeNode* _right;

	TreeNode()
		:_val(0)
		, _left(nullptr)
		, _right(nullptr)
	{}
};

void TestObjectPool()
{
	// 申请释放的轮次
	const size_t Rounds = 5;

	// 每轮申请释放多少次
	const size_t N = 100000;

	std::vector<TreeNode*> v1;
	v1.reserve(N);

	size_t begin1 = clock();
	for (size_t j = 0; j < Rounds; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			v1.push_back(new TreeNode);
		}
		for (int i = 0; i < N; ++i)
		{
			delete v1[i];
		}
		v1.clear();
	}

	size_t end1 = clock();

	std::vector<TreeNode*> v2;
	v2.reserve(N);

	ObjectPool<TreeNode> TNPool;
	size_t begin2 = clock();
	for (size_t j = 0; j < Rounds; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			v2.push_back(TNPool.New());
		}
		for (int i = 0; i < N; ++i)
		{
			TNPool.Delete(v2[i]);
		}
		v2.clear();
	}
	size_t end2 = clock();

	cout << "new cost time:" << end1 - begin1 << endl;
	cout << "object pool cost time:" << end2 - begin2 << endl;
}