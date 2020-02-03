
#include<iostream>
#include<memory>
#include<vector>
#include<map>
using namespace std;
//自定义的字符串类
class tool{
	
};
//c++11  支持 成员 初始化
//可以直接在 成员上赋值
class String{
	
public:
	//存储了
	char* obj = nullptr;
	//创建的空间的大小
	int capacity = 0;
	//根据指定长度，创建一个空间
	String(size_t size){
		std::cout << "调用了指定长度申请空间的构造函数" << std::endl;
		obj = new char[size];
		memset(obj,0,size);
		capacity = size;
	}
	//将指定字符串前len个字符放入新的字符串里
	String(char* other,int len):String(len+1){
		std::cout << "调用了赋值某个长度 指定字符串的构造函数" << std::endl;
		/*obj = new char[len+1];
		capacity = len + 1;*/
		strncpy(obj,other,len);
	}
	
	//拷贝 构造函数
	String(const String& other) :capacity(other.capacity){
		std::cout << "调用了String的拷贝构造函数" << std::endl;
		obj = new char[other.capacity];
		strcpy(obj,other.obj);
	}
	String(String&& other){
		std::cout << "调用了String的移动构造函数" << std::endl;
		obj = other.obj;
		capacity = other.capacity;
		other.obj = 0;
		other.capacity = 0;
	}
	~String(){
		std::cout << "调用了析构" << std::endl;
		delete obj;
	}
	String()=delete;
};
String getString(){
	String s(10000);
	return s;
}
//用模板技术和vector实现一个Stack  push pop top
//template<typename T>
template<class T>
class stack{
	vector<T> arr;
public:
	//放入栈一个变量
	void push(const T& t){
		arr.push_back(t);
	}
	//弹出最外侧的变量
	void pop(){
		arr.pop_back();
	}
	T& top(){
		return arr.back();
	}
	size_t size(){
		return arr.size();
	}
};

template<class T,class B> 
T max(T a,B b){
	cout << "使用了模板函数" << endl;
	return a > b ? a : b;
}



int main(){
	int a = 5;
	double b = 3;

	cout << max(a, b) << endl;
}

//默认生成的无参构造，会自动调用成员对象的 无参构造
//default  显式 指定，有默认的构造

//左值  有地址的变量
//右值  无法获得地址，没有明确地址的变量

//左值引用 &
//引用和指针本质是一回事， 指针需要指向一个地址，引用本质 也需要指向一个地址

//右值引用 &&
//指向临时对象，常量