
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

//自旋锁
class SpinLock{
	//标记位初始是空，一个线程获得锁就将标记变成true
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

mutex mx;//5ms无锁    互斥量:500ms    lock_guard 600ms unique_lock<mutex> 650ms  原子变量30ms  自旋锁150
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
	cout << "休眠1s前" << endl;
	this_thread::sleep_for(seconds(2));
	this_thread::sleep_until(system_clock::now()+seconds(2));
	cout << "休眠1s后" << endl;
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
//	//获得当前时间点
//	auto start=system_clock::now();
//	for (int i = 0; i < 5; i++){
//		thread tem(run);
//		t[i] = std::move(tem);
//	}
//	for (int i = 0; i < 5; i++){
//		t[i].join();
//	}
//	//获得当前时间点
//	auto end =system_clock::now();
//	//两个时间点做差，得到时间段，时间段的精度是 两个时间点的精度的最大
//	auto d=end - start;
//	//转换成以毫秒为单位
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