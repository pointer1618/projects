#include<iostream>
#include<stdio.h>
#include<ctime>
#include"ziplist.h"
#include"redis.h"
#include"intset.h"
using namespace std;
class A {

};
template<typename T>
class B :public A
{
	T val;
};
int main()
{
	struct timeval stamp;
	gettimeofday(&stamp, NULL);
	cout << stamp.tv_sec<< endl;
	cout << stamp.tv_usec<< endl;
	return 0;
}