#pragma once
//auto_ptr
/*
���췽��
����
operator=   ת�ƿ���Ȩ
get         ��ñ������ָ��
operator*  ���ʱ��������
operator-> ����
reset      ���ٶ���(�ƶ��¶�������֮ǰ��)
release    �ͷŶ���
*/
template<class T>
class LAuto_ptr{
	//�ڲ��洢ָ��ռ��һ����Ա����
	T* obj = nullptr;
public:
	LAuto_ptr(const LAuto_ptr&) = delete;
	LAuto_ptr() = default;
	LAuto_ptr(T* p){
		obj = p;
	}
	~LAuto_ptr(){
		delete obj;
	}
	void operator=(LAuto_ptr<T>& other){
		obj = other.obj;
		other.obj = nullptr;
	}
	T* operator->(){
		return obj;
	}
	T* get(){
		return obj;
	}
	T& operator*(){
		return *obj;
	}

	void reset(T* ptr = nullptr){
		obj = ptr;
	}
	void release(){
		delete obj;
		obj = nullptr;
	}
};

template<class T>
class LUnique_ptr{
	T* obj;
public:
	LUnique_ptr(const LUnique_ptr&) = delete;
	LUnique_ptr<T>& operator=(LUnique_ptr&) = delete;
	LUnique_ptr(LUnique_ptr&& other){
		obj = other.obj;
		other.obj = 0;
	}
	~LUnique_ptr(){
		delete obj;
	}
	T* operator->(){
		return obj;
	}
	T* get(){
		return obj;
	}
	T& operator*(){
		return *obj;
	}
	void reset(T* ptr = nullptr){
		obj = ptr;
	}
	void release(){
		delete obj;
		obj = nullptr;
	}
};
//���ü��������������shareָ��ָ�����ü�������
template<class T>
class LShared_ptr{
	class ref{
		//��������ǰ�м�������
		int count = 0;
		//��Ӧ�Ķ���
		T* target = nullptr;
	public:
		ref(T* target){
			this->target = target;
			count++;
		}
		//���ü���+1
		inline void up(){
			count++;
		}
		//���ü���-1
		inline void down(){
			count--;
			if (count == 0){
				delete target;
				//Ϊʲô���� �ٳ�Ա������delete this������ô���أ�����������������������������������������������������
				delete this;
			}
		}
		//������ü�����������Ӧ�Ķ���
		inline T* get(){
			return target;
		}
		inline int numb(){
			return count;
		}
	};
public:
	//��Ӧ�����ü�������
	ref* object = nullptr;
	//�ᴴ��һ���µ����ü��������� ά����ָ��
	LShared_ptr(T* newObj){
		object = new ref(newObj);
		//һ�������еĴ��룬����ֱ�Ӳٿ���һ��������ڲ��ĳ�Ա��ֻ��ͨ�����÷�����������һ������
	}
	//����һ������Ӧ�����ü������1
	LShared_ptr(const LShared_ptr& other){
		object = other.object;
		object->up();
	}
	//����������ָ�룬��Ӧ�����ü�����һ
	~LShared_ptr(){
		if (object){
			object->down();
		}
	}
	//ָ����������ʵ�ָ��
	T* operator->(){
		return object->get();
	}
	//��ö�Ӧ�Ķ����ָ��
	T* get(){
		return object->get();
	}
	//��ö�Ӧ���������
	T& operator*(){
		return *(object->get());
	}
	//��֮ǰ��������ü�����һ������Ϊ�¶��󴴽����ü���
	void reset(T* ptr = nullptr){
		if (object){
			object->down();
		}

		if (ptr != nullptr){
			object = new ref(ptr);
		}
		else{//
			object = nullptr;
		}
	}
	//ָ�������һ��������ָ��ʧЧ,reset(NULL);
	void release(){
		if (object){
			object->down();
		}
		object = nullptr;
	}
	//��õ�ǰĿ���������ü���
	int count(){
		return object->numb();
	}
};