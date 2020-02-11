
#include<iostream>
#include<memory>
#include<vector>
#include<map>
using namespace std;
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
	T* obj=nullptr;
public:
	LAuto_ptr() = default;
	LAuto_ptr(T* p){
		obj = p;
	}
	~LAuto_ptr(){
		delete obj;
	}
	void operator=(LAuto_ptr<T>& other){
		obj = other.obj;
		other.obj=nullptr;
	}
	T* get(){
		return obj;
	}
	T& operator*(){
		return *obj;
	}
	void reset(T* ptr=nullptr){
		obj = ptr;
	}
	void release(){
		delete obj;
		obj = nullptr;
	}
};
class A{
public:
	~A(){
		cout << "析构了对象" << endl;
	}
};
int main(){
	
	LAuto_ptr<A> ptr(new A);
	LAuto_ptr<A> ptr2;
	ptr2 = ptr;
	ptr.release();
	
}

//默认生成的无参构造，会自动调用成员对象的 无参构造
//default  显式 指定，有默认的构造

//左值  有地址的变量
//右值  无法获得地址，没有明确地址的变量

//左值引用 &
//引用和指针本质是一回事， 指针需要指向一个地址，引用本质 也需要指向一个地址

//右值引用 &&
//指向临时对象，常量