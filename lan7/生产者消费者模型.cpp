
#include<iostream>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<queue>
#include<Windows.h>
using namespace std;

//c++11�߳̿� ���ֹ��ܵ� ��Ƚ�windows��Ҫ��һЩ������ʹ�����㹻
class goods{
public:
	//�����
	int gid;
	//�ֿ��
	int cid;
	goods(int value,int cid):gid(value),cid(cid){
		
	}
};
condition_variable cv;
//һ���ֿ⣬�洢�������ߵ������Ļ���
queue<goods*> factor;
//ר�ű����ֿ��
mutex factor_mutex;

void creator(int id){
	int n = 0;//�����
	while (1){
		auto g = new goods(n++, id);
		Sleep(1000);
		unique_lock<mutex> ul(factor_mutex);
		factor.push(g);
		//���������仯�����������ж�һ�����ǵȴ��������Ƿ����
		cv.notify_all();
	}
}

void consumer(int id){
	while (1){
		unique_lock<mutex> ul(factor_mutex);
		//�ȴ��������ֿⲻΪ��
		cv.wait(ul, [](){return !factor.empty(); });
		auto g = factor.front();
		factor.pop();
		ul.unlock();
		printf("������%d��������%d�Ųֿ��%d����\n", id, g->cid, g->gid);
		delete g;
	}
	
}
int main(){
	for (int i = 0; i < 3; i++){
		thread t(creator, i);
		t.detach();
	}
	for (int i = 0; i < 10; i++){
		thread t(consumer, i);
		t.detach();
	}
	system("pause");
}


	/*
	thread t[5];
	for (int i = 0; i < 5;i++){
		thread tem(run,i);
		t[i]=std::move(tem);
	}
	for (int i = 0; i < 5; i++){
		t[i].join();
	}*/
	/*thread* t[5];
	for (int i = 0; i < 5;i++){
		t[i] = new thread(run,i);
	}
	for (int i = 0; i < 5; i++){
		t[i]->join();
	}
	for (int i = 0; i < 5; i++){
		delete t[i];
	}*/
	
	/*HANDLE h[5];
	for (int i = 0; i < 5;i++){
		h[i]=CreateThread(0, 0, run, (void*)i, 0, 0);
	}
	WaitForMultipleObjects(5,h,true,INFINITE);*/

//�ٽ��� ���� �����߳�ͬʱ �޸� ������Դ
//�ٽ�������������:ʹ��mutex

//ÿ��������ʱ������ã�����true���������ѳɹ�  �������false��������ʧ�ܣ������ȴ�
//cv.wait(ul, []()

//lock ռ����������ȴ���
//unlock �ͷ����������߳̿�����һ���߳� ռ������
//trylock ���Ի��������������

//std::ref ר�������߳��в���ֵ����ʱ ���ݶ�������� 
//std::cref ����const ����

//ʲô��һ������
//qq�����ڴ��ļ���ʱ�򣬾Ͳ�����Ƶ��
//���ļ�:ѭ��������ͣ�����ڴ棬
//����Ƶ:ѭ���Ķ�ȡ�ļ�����ȡ�ļ��е�������Ϣ�������ص������Ļ
//�����ó���ִ��

//������Ƶ,�ܶ�
//����һ����������һ����������һ��
//ͬʱ��������Ƶ���������ޣ� ����������

//���ʹ�ý��� ʵ�ֲ���Ч��,���̴����Ƚ���������

//DWORD WINAPI run(void* p){
//	for (int i = 0; i < 100;i++){
//		printf("�߳�%d���е��˵�%d����\n",p,i);
//	}
//	return 0;
//}