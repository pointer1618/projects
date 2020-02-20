#pragma once
#include<ctime>
#include<iostream>
class intset
{
public:
	unsigned int length;
	int * array;
	intset()
	{
		length = 0;
		array = nullptr;
	}
	intset(intset& res)
	{
		length = res.length;
		array = new int[length];
		for (int i = 0; i < length; i++)
		{
			array[i] = res.array[i];
		}
	}
	bool intsetFind(int number)
	{
		if (length == 0)
			return false;
		int begin = 0;
		int end = length - 1;
		int mid = (begin + end) / 2;
		while (begin < end)
		{
			if (array[mid] == number)
				return true;
			else if (array[mid] > number)
			{
				end = mid;
			}
			else
			{
				if(begin == mid);
					begin++;
			}
			mid = (begin + end) / 2;
		}
		if (array[mid] == number)
			return true;
		return false;
	}
	bool intsetAdd(int  number)
	{
		if (intsetFind(number))
			return false;
		auto p = new int[length + 1];
		int i=0;
		for (; i < length; i++)
		{
			if (array[i] < number)
				p[i] = array[i];
			else
			{
				break;
			}
		}
		p[i++] = number;
		for (; i <= length; i++)
		{
			p[i] = array[i - 1];
		}
		length++;
		delete []array;
		array = p;
		return true;
	}
	bool intsetRemove(int  number)
	{
		if (intsetFind(number)==false)
			return false;
		if (length == 1)
		{
			delete[]array;
			array = nullptr;
			return true;
		}
		auto p = new int[length -1];
		for (int i = 0; i < length-1; i++)
		{
			if (array[i] < number)
				p[i] = array[i];
			else
			{
				p[i] = array[i+1];
			}
		}
		length--;
		delete[]array;
		array = p;
		return true;
	}
	int intsetRandom()
	{
		int r = rand() % length;
		return array[r];
	}
	int operator [](int index)
	{
		return array[index];
	}
	int intsetGet(int index)
	{
		return array[index];
	}
	~intset()
	{
		delete[]array;
		length = 0;
		array = nullptr;
	}
};

