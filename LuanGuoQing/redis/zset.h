#pragma once
#include"dict.h"
#include"skiplist.h"
class zset
{
public:
	dict<double> d;
	skiplist s;
	bool zsetnsert(double doub, sdshdr str)
	{
		d.dictAdd(str, doub);
		s.insert(doub, str);

	}
	int zsetGetRank(double doub,sdshdr str)
	{
		return s.zslGetRank(doub, str);
	}
	unsigned int zsetLength()
	{
		return s.length;
	}
	sdshdr  zsetGetElementByRank(int rank)
	{
		return * s.zslGetElementByRank(rank)->obj;
	}
	double zsetGetCore(sdshdr str) {
		return *d.dictFetchValue(str)->val;
	}
	bool zsetRemove()
	{
		d.~dict();
		s.~skiplist();
	}
	~zset()
	{

	}
};

