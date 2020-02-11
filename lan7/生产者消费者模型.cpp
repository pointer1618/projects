
#include<iostream>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<queue>
#include<Windows.h>
using namespace std;

//c++11线程库 各种功能等 相比较windows库要少一些，基本使用是足够
class goods{
public:
	//货物号
	int gid;
	//仓库号
	int cid;
	goods(int value,int cid):gid(value),cid(cid){
		
	}
};
condition_variable cv;
//一个仓库，存储着生产者的生产的货物
queue<goods*> factor;
//专门保护仓库的
mutex factor_mutex;

void creator(int id){
	int n = 0;//货物号
	while (1){
		auto g = new goods(n++, id);
		Sleep(1000);
		unique_lock<mutex> ul(factor_mutex);
		factor.push(g);
		//条件发生变化，让消费者判断一下他们等待的条件是否成立
		cv.notify_all();
	}
}

void consumer(int id){
	while (1){
		unique_lock<mutex> ul(factor_mutex);
		//等待条件，仓库不为空
		cv.wait(ul, [](){return !factor.empty(); });
		auto g = factor.front();
		factor.pop();
		ul.unlock();
		printf("消费者%d号消费了%d号仓库的%d货物\n", id, g->cid, g->gid);
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

//临界区 问题 ：多线程同时 修改 共享资源
//临界区问题解决方案:使用mutex

//每当被唤醒时，会调用，返回true，代表被唤醒成功  如果返回false，代表唤醒失败，继续等待
//cv.wait(ul, []()

//lock 占有这把锁，等待锁
//unlock 释放锁，其他线程可以有一个线程 占有锁，
//trylock 尝试获得锁，不会阻塞

//std::ref 专门用在线程中不想值传递时 传递对象的引用 
//std::cref 传递const 引用

//什么叫一个程序
//qq，他在传文件的时候，就不看视频了
//传文件:循环拷贝不停拷贝内存，
//看视频:循环的读取文件，读取文件中的像素信息，将像素点输出屏幕
//并发让程序执行

//缓存视频,很多
//缓存一个，缓存另一个，缓存另一个
//同时缓存多个视频，网速有限， 并发的例子

//最初使用进程 实现并发效果,进程创建比较慢，并发

//DWORD WINAPI run(void* p){
//	for (int i = 0; i < 100;i++){
//		printf("线程%d运行到了第%d部分\n",p,i);
//	}
//	return 0;
//}