#pragma once
#pragma once
#pragma once
#include<iostream>
#include <new>
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
			obj = _freeList;
			_freeList = next;//�����ڴ�أ�����ÿ��ȡ�ô�Сһ��


			//return obj;

		}
		else
		{//freelistΪ��
			//ʣ���ڴ治��һ�������С��ʱ�������¿�����¿ռ�
			if (_remainBytes < sizeof(T))
			{
				_memory = (char*)malloc(128 * 1024);
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
		obj->��T();
		*(void **)obj = _freeList;
		_freeList = obj;
		//}
	}

private:
	char* _memory = nullptr;//ָ�����ڴ��ָ��
	size_t _remainBytes = 0;//����ڴ�ʣ��Ĵ�С
	void* _freeList = nullptr;//���������������ӵ����������ͷָ��

};