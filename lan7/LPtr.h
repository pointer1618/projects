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
//引用计数对象负责计数，share指针指向引用计数对象
template<class T>
class LShared_ptr{
	class ref{
		//计数，当前有几个引用
		int count = 0;
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
				//为什么可以 再成员函数中delete this，会怎么样呢？？？？？？？？？？？？？？？？？？？？？？？？？？？
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
public:
	//对应的引用计数对象
	ref* object = nullptr;
	//会创建一个新的引用计数对象来 维护该指针
	LShared_ptr(T* newObj){
		object = new ref(newObj);
		//一个对象中的代码，不能直接操控另一个对象的内部的成员，只能通过调用方法来控制另一个对象
	}
	//拷贝一个，对应的引用计数会加1
	LShared_ptr(const LShared_ptr& other){
		object = other.object;
		object->up();
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