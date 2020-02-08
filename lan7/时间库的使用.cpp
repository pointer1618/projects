
#include<iostream>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<queue>
#include<Windows.h>
#include<ctime>
#include<atomic>
#include<chrono>
using namespace std;

//������
class SpinLock{
	//���λ��ʼ�ǿգ�һ���̻߳�����ͽ���Ǳ��true
	atomic_flag f;
public:
	SpinLock(){
		f.clear();
	}
	void lock(){
		while (f.test_and_set() == true);
	}
	void unlock(){
		f.clear();
	}
	bool try_lock(){
		//return f.test_and_set()==false;
		if (f.test_and_set() == true){
			return false;
		}
		else{
			return true;
		}
	}
};


SpinLock sl;

mutex mx;//5ms����    ������:500ms    lock_guard 600ms unique_lock<mutex> 650ms  ԭ�ӱ���30ms  ������150
long number;
void run(){
	for (int i = 0; i < 100000; i++){
		lock_guard<SpinLock> lg(sl);
		number++;
	}
}
using namespace chrono;
#include <iostream>
#include <chrono>
using namespace std;
int main()
{
	cout << "����1sǰ" << endl;
	this_thread::sleep_for(seconds(2));
	this_thread::sleep_until(system_clock::now()+seconds(2));
	cout << "����1s��" << endl;
	cout << "system clock          : ";
	cout << system_clock::period::num << "/" << system_clock::period::den << "s" << endl;
	cout << "steady clock          : ";
	cout << steady_clock::period::num << "/" <<steady_clock::period::den << "s" << endl;
	cout << "high resolution clock : ";
	cout << high_resolution_clock::period::num << "/" << high_resolution_clock::period::den << "s" << endl;
	
	return 0;
}

//int main(){
//	thread t[10];
//	//��õ�ǰʱ���
//	auto start=system_clock::now();
//	for (int i = 0; i < 5; i++){
//		thread tem(run);
//		t[i] = std::move(tem);
//	}
//	for (int i = 0; i < 5; i++){
//		t[i].join();
//	}
//	//��õ�ǰʱ���
//	auto end =system_clock::now();
//	//����ʱ�������õ�ʱ��Σ�ʱ��εľ����� ����ʱ���ľ��ȵ����
//	auto d=end - start;
//	//ת�����Ժ���Ϊ��λ
//	cout << duration_cast<milliseconds>(d).count() << endl;
//	system("pause");
//}


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