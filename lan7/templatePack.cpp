
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
		other.obj=nullptr;
	}
	T* get(){
		return obj;
	}
	T& operator*(){
		return *obj;
	}
	T& operator->(){
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
	int a;
	~A(){
		cout << "析构了对象" << endl;
	}
};
//0个到多个任意类型的参数
template<class T> 
void run(T g){
	cout << "单参数模板" << endl;
	cout << g << endl;
}
//sizeof...
template<class T,class... args> 
void run(T g, args... a){
	if (sizeof...(a) > 3){
		cout << "参数大于三个，我不算了！" << endl;
		return;
	}
	cout << "多参数模板,参数个数" << sizeof... (a)<< endl;
	cout << g << endl;
	run(a...);
}
//void run(int g,int a1,int a2,const char* a3,const char* a4){
//	cout << g << endl;
//	run(a1,a2,a3,a4);
//}
//void run(int g, int a1, const char* a2, const char* a3){
//	cout << g << endl;
//	run(a1, a2, a3);
//}
//void run(int g,const char* a1, const char* a2){
//	cout << g << endl;
//	run(a1, a2);
//}
//void run(const char* g, const char* a1){
//	cout << g << endl;
//	run(a1);
//}

int main(){
	LAuto_ptr<A> ptr(new A);
	LAuto_ptr<A> ptr2;
	ptr2 = ptr;
	ptr.release();
	run(1,2,3,"255","12512");
}

//默认生成的无参构造，会自动调用成员对象的 无参构造
//default  显式 指定，有默认的构造

//左值  有地址的变量
//右值  无法获得地址，没有明确地址的变量

//左值引用 &
//引用和指针本质是一回事， 指针需要指向一个地址，引用本质 也需要指向一个地址

//右值引用 &&
//指向临时对象，常量