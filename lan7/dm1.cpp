
#include<iostream>
#include<memory>
#include<vector>
#include<map>
using namespace std;
//�Զ�����ַ�����
class tool{
	
};
//c++11  ֧�� ��Ա ��ʼ��
//����ֱ���� ��Ա�ϸ�ֵ
class String{
	
public:
	//�洢��
	char* obj = nullptr;
	//�����Ŀռ�Ĵ�С
	int capacity = 0;
	//����ָ�����ȣ�����һ���ռ�
	String(size_t size){
		std::cout << "������ָ����������ռ�Ĺ��캯��" << std::endl;
		obj = new char[size];
		memset(obj,0,size);
		capacity = size;
	}
	//��ָ���ַ���ǰlen���ַ������µ��ַ�����
	String(char* other,int len):String(len+1){
		std::cout << "�����˸�ֵĳ������ ָ���ַ����Ĺ��캯��" << std::endl;
		/*obj = new char[len+1];
		capacity = len + 1;*/
		strncpy(obj,other,len);
	}
	
	//���� ���캯��
	String(const String& other) :capacity(other.capacity){
		std::cout << "������String�Ŀ������캯��" << std::endl;
		obj = new char[other.capacity];
		strcpy(obj,other.obj);
	}
	String(String&& other){
		std::cout << "������String���ƶ����캯��" << std::endl;
		obj = other.obj;
		capacity = other.capacity;
		other.obj = 0;
		other.capacity = 0;
	}
	~String(){
		std::cout << "����������" << std::endl;
		delete obj;
	}
	String()=delete;
};
String getString(){
	String s(10000);
	return s;
}
//��ģ�弼����vectorʵ��һ��Stack  push pop top
//template<typename T>
template<class T>
class stack{
	vector<T> arr;
public:
	//����ջһ������
	void push(const T& t){
		arr.push_back(t);
	}
	//���������ı���
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
	cout << "ʹ����ģ�庯��" << endl;
	return a > b ? a : b;
}



int main(){
	int a = 5;
	double b = 3;

	cout << max(a, b) << endl;
}

//Ĭ�����ɵ��޲ι��죬���Զ����ó�Ա����� �޲ι���
//default  ��ʽ ָ������Ĭ�ϵĹ���

//��ֵ  �е�ַ�ı���
//��ֵ  �޷���õ�ַ��û����ȷ��ַ�ı���

//��ֵ���� &
//���ú�ָ�뱾����һ���£� ָ����Ҫָ��һ����ַ�����ñ��� Ҳ��Ҫָ��һ����ַ

//��ֵ���� &&
//ָ����ʱ���󣬳���