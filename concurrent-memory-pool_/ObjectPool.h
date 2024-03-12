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
	T* New()//û��*�ᱨʲô��?
	{
		T * obj = nullptr;

		if (_freeList)
		{//����ʹ�û��������ڴ��
			void* next = *((void **)_freeList);//	ȡfreelist��ͷ4��8�ֽ�
			obj =(T*) _freeList;
			_freeList = next;//�����ڴ�أ�����ÿ��ȡ�ô�Сһ��


			//return obj;

		}
		else
		{//freelistΪ��
			//ʣ���ڴ治��һ�������С��ʱ�������¿�����¿ռ�
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
			//���ٿ�һ��ָ��Ĵ�С
			size_t objSize = sizeof(T);
			if (objSize < sizeof(void*))objSize = sizeof(void*);

			_memory += objSize;
			_remainBytes -= objSize;
			//return obj;

		}
		//��λnew����ʾ����T�Ĺ��캯����ʼ��
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
			//ͷ�巨��obj����_freelist
		obj->~T();
		*(void **)obj = _freeList;
		_freeList = obj;
		//}
	}

private:
	char* _memory = nullptr;//ָ�����ڴ��ָ��
	size_t _remainBytes = 0;//����ڴ�ʣ��Ĵ�С
	void* _freeList = nullptr;//���������������ӵ����������ͷָ��

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
	// �����ͷŵ��ִ�
	const size_t Rounds = 5;

	// ÿ�������ͷŶ��ٴ�
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