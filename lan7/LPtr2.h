#pragma once

//auto_ptr
/*
构造方法
析构
operator=   转移控制权
get         获得被管理的指针
operator*  访问被管理对象
operator-> 访问
reset      销毁对象(制定新对象，销毁之前的)
release    释放对象
*/
template<class T>
class LAuto_ptr{
	//内部存储指向空间的一个成员变量
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
	//对象的引用计数，当前有几个引用，归零删除对象
	int count = 0;
	//专门记录有多少个weakptr存在。
	int weak_count = 0;
	//对应的对象
	T* target = nullptr;
public:
	ref(T* target){
		this->target = target;
		count++;
	}
	//引用计数+1
	inline void up(){
		count++;
	}
	//引用计数-1
	inline void down(){
		count--;
		if (count == 0){
			delete target;
		}
	}
	//代表弱指针的引用计数的增加
	inline void upWeak(){
		weak_count++;
	}
	//代表弱指针的引用计数的减少
	inline void downWeak(){
		weak_count--;
		if (count == 0 && weak_count == 0){
			delete this;
		}
	}
	//获得引用计数对象所对应的对象
	inline T* get(){
		return target;
	}
	inline int numb(){
		return count;
	}
};
//引用计数对象负责计数，share指针指向引用计数对象
template<class T>
class LShared_ptr{
public:
	//对应的引用计数对象
	::ref<T>* object = nullptr;
	//会创建一个新的引用计数对象来 维护该指针
	LShared_ptr() = default;
	LShared_ptr(T* newObj){
		object = new ::ref<T>(newObj);
		//一个对象中的代码，不能直接操控另一个对象的内部的成员，只能通过调用方法来控制另一个对象
	}
	//拷贝一个，对应的引用计数会加1
	LShared_ptr(const LShared_ptr<T>& other){
		object = other.object;
		object->up();
	}
	//赋值成新的，如果之前引用了其他的，那么就不再引用，开始引用新的，所以之前的引用计数减1，新的引用计数+1
	LShared_ptr<T>&  operator=(LShared_ptr<T>& other){
		if (object){
			object->down();
		}
		object = other.object;
		object->up();
		return *this;
	}
	//析构该智能指针，对应的引用计数减一
	~LShared_ptr(){
		if (object){
			object->down();
		}
	}
	//指针运算符访问的指针
	T* operator->(){
		return object->get();
	}
	//获得对应的对象的指针
	T* get(){
		return object->get();
	}
	//获得对应对象的引用
	T& operator*(){
		return *(object->get());
	}
	//将之前对象的引用计数减一，并且为新对象创建引用计数
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
	//传入一个新的引用计数，让该shared指针舍弃之前的计数，使用新的计数
	void reset(::ref<T>* otherRef){
		if (object){
			object->down();
		}
		object = otherRef;
		object->up();
	}
	//指针计数减一，该智能指针失效,reset(NULL);
	void release(){
		if (object){
			object->down();
		}
		object = nullptr;
	}
	//获得当前目标对象的引用计数
	int count(){
		return object->numb();
	}
};

//weak_ptr 只能配合share_ptr来使用，一个弱引用对象
//expired() 判断对象是否存活，与lock()获得对应对象的share_ptr指针，如果获得不了，返回空指针
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
	//对象存在，返回true
	bool expired(){
		return object.numb() != 0;
	}
	//返回对应对象的指针，如果指针不存在，则该share指针内部 空的
	LShared_ptr<T> lock(){
		if (expired()){
			LShared_ptr<T> a;
			a.reset(object);
			return a;
		}
	}
};