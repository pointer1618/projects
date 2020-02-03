
#include<iostream>
#include<memory>
#include<vector>
#include<map>
using namespace std;
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
		cout << "�����˶���" << endl;
	}
};
int main(){
	
	LAuto_ptr<A> ptr(new A);
	LAuto_ptr<A> ptr2;
	ptr2 = ptr;
	ptr.release();
	
}

//Ĭ�����ɵ��޲ι��죬���Զ����ó�Ա����� �޲ι���
//default  ��ʽ ָ������Ĭ�ϵĹ���

//��ֵ  �е�ַ�ı���
//��ֵ  �޷���õ�ַ��û����ȷ��ַ�ı���

//��ֵ���� &
//���ú�ָ�뱾����һ���£� ָ����Ҫָ��һ����ַ�����ñ��� Ҳ��Ҫָ��һ����ַ

//��ֵ���� &&
//ָ����ʱ���󣬳���