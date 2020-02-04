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
template<class T>
class ref{
	//��������ü�������ǰ�м������ã�����ɾ������
	int count = 0;
	//ר�ż�¼�ж��ٸ�weakptr���ڡ�
	int weak_count = 0;
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
		}
	}
	//������ָ������ü���������
	inline void upWeak(){
		weak_count++;
	}
	//������ָ������ü����ļ���
	inline void downWeak(){
		weak_count--;
		if (count == 0 && weak_count == 0){
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
//���ü��������������shareָ��ָ�����ü�������
template<class T>
class LShared_ptr{
public:
	//��Ӧ�����ü�������
	::ref<T>* object = nullptr;
	//�ᴴ��һ���µ����ü��������� ά����ָ��
	LShared_ptr() = default;
	LShared_ptr(T* newObj){
		object = new ::ref<T>(newObj);
		//һ�������еĴ��룬����ֱ�Ӳٿ���һ��������ڲ��ĳ�Ա��ֻ��ͨ�����÷�����������һ������
	}
	//����һ������Ӧ�����ü������1
	LShared_ptr(const LShared_ptr<T>& other){
		object = other.object;
		object->up();
	}
	//��ֵ���µģ����֮ǰ�����������ģ���ô�Ͳ������ã���ʼ�����µģ�����֮ǰ�����ü�����1���µ����ü���+1
	LShared_ptr<T>&  operator=(LShared_ptr<T>& other){
		if (object){
			object->down();
		}
		object = other.object;
		object->up();
		return *this;
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
	//����һ���µ����ü������ø�sharedָ������֮ǰ�ļ�����ʹ���µļ���
	void reset(::ref<T>* otherRef){
		if (object){
			object->down();
		}
		object = otherRef;
		object->up();
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

//weak_ptr ֻ�����share_ptr��ʹ�ã�һ�������ö���
//expired() �ж϶����Ƿ����lock()��ö�Ӧ�����share_ptrָ�룬�����ò��ˣ����ؿ�ָ��
template<class T>
class LWeak_ptr{
public:
	::ref<T>* object = nullptr;
	LWeak_ptr() = default;
	~LWeak_ptr(){
		if (object){
			object->downWeak();
		}
		
	}
	LWeak_ptr<T>& operator=(LShared_ptr<T>& sp){
		if (object){
			object->downWeak();
		}
		object = sp.object;
		sp.object->upWeak();
		return *this;
	}
	//������ڣ�����true
	bool expired(){
		return object.numb() != 0;
	}
	//���ض�Ӧ�����ָ�룬���ָ�벻���ڣ����shareָ���ڲ� �յ�
	LShared_ptr<T> lock(){
		if (expired()){
			LShared_ptr<T> a;
			a.reset(object);
			return a;
		}
	}
};