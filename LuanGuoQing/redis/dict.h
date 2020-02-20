#pragma once
#include"sdshdr.h"
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef long unsigned int uint64_t;

template <typename VAL>
class dectEntry {
public:
	sdshdr* key=nullptr;
	VAL* val = nullptr;
	dectEntry<VAL>* next = nullptr;
	dectEntry(const dectEntry<VAL> & pair)
	{
		key = new sdshdr(*pair.key);
		val = new VAL(*pair.val);
		next = nullptr;
	}
	dectEntry( sdshdr & k,  VAL& v)
	{
		key = new sdshdr(k);
		val = new VAL(v);
		next = nullptr;
	}
	~dectEntry()
	{
		delete key;
		delete val;
	}
};
template <typename VAL>
class dictht
{
public:
	dectEntry<VAL>** table;
	unsigned long size;
	unsigned long sizemask;
	unsigned long used;
	dictht()
	{
		table = nullptr;
		size = 0;
		sizemask = 0;
		used = 0;
	}
	dictht(int initsize )
	{
		table = new dectEntry<VAL>* [initsize];
		size = initsize;
		sizemask = size - 1;
		used = 0;
	}
	~dictht()
	{
		for (int i = 0; i < size; i++)
		{
			delete table[i];
		}
		delete[]table;
		table = nullptr;
		size = 0;
		sizemask = 0;
		used = 0;
	}
};
template <typename VAL>
class dict
{
public:

	dictht<VAL> ht[2];
	int trehashhidx = -1;
	dict(int initsize=4)
	{
		initsize = initsize < 4 ? 4 : initsize;
		ht[0].table = new dectEntry<VAL>* [initsize];
		ht[0].size = initsize;
		ht[0].sizemask = ht[0].size - 1;
		ht[0].used = 0;

		ht[1].table = nullptr;
		ht[1].size = 0;
		ht[1].sizemask = 0;
		ht[1].used = 0;
	}
	bool dictAdd( sdshdr k,  VAL v)
	{
		return dictAdd(dectEntry<VAL>(k, v));
	}
	bool dictAdd(const dectEntry<VAL> & pair)
	{
		unsigned int hashValue = dictGenHashFunction(pair.key->sdsbuf(), pair.key->sdslen());
		hashValue = hashValue & ht[0].sizemask;
		auto p1 = ht[0].table[hashValue];
		while (p1)
		{
			if (*pair.key == *p1->key)
				return false;
			p1 = p1->next;
		}
		if (trehashhidx == -1)
		{
			if ((double)ht[0].used / (double)ht[0].size >= 1)//扩容
			{
				ht[1].table= new dectEntry<VAL>* [ht[0].size*2];
				ht[1].size = ht[0].size * 2;
				ht[1].sizemask = ht[1].size-1;
				ht[1].used = 0;
				trehashhidx = 0;
			}
			else//在ht0中放入节点
			{
				
				auto p= new  dectEntry<VAL>(pair);
				p->next = ht[0].table[hashValue];
				ht[0].table[hashValue] = p;
				ht[0].used++;
			}
		}
		if (trehashhidx != -1)//在ht1中放入节点
		{
			//新节点加入到ht1
			unsigned int hashValue = dictGenHashFunction(pair.key->sdsbuf(), pair.key->sdslen());
			hashValue = hashValue & ht[1].sizemask;
			p1 = ht[1].table[hashValue];
			while (p1)
			{
				if (*pair.key == *p1->key)
					return false;
				p1 = p1->next;
			}
			auto p = new  dectEntry<VAL>(pair);
			p->next = ht[1].table[hashValue];
			ht[1].table[hashValue] = p;
			ht[1].used++;
			//旧节点加入到ht1
			if (ht[0].used > 0)
			{
				reHash();
			}
			//转换
			
		}
		return true;
	}
	bool dictReplace( sdshdr k, VAL v)
	{
		return dictReplace(dectEntry<VAL>(k, v));
	}
	bool dictReplace(const dectEntry<VAL>& pair){
		unsigned int hashValue = dictGenHashFunction(pair.key->sdsbuf(), pair.key->sdslen());
		hashValue = hashValue & ht[0].sizemask;
		auto p1 = ht[0].table[hashValue];
		while (p1)
		{
			if (*pair.key == *p1->key)
			{
				delete p1->val;
				p1->val = new VAL(*pair.val);
				return true;
			}
			p1 = p1->next;
		}
		if (trehashhidx == -1)
		{
			if (ht[0].used / ht[0].size >= 1)//扩容
			{
				ht[1].table = new dectEntry<VAL>* [ht[0].size * 2];
				ht[1].size = ht[0].size * 2;
				ht[1].sizemask = ht[1].size - 1;
				ht[1].used = 0;
				trehashhidx = 0;
			}
			else//在ht0中放入节点
			{

				auto p = new  dectEntry<VAL>(pair);
				p->next = ht[0].table[hashValue];
				ht[0].table[hashValue] = p;
				ht[0].used++;
			}
		}
		if (trehashhidx != -1)//在ht1中放入节点
		{
			//新节点加入到ht1
			unsigned int hashValue = dictGenHashFunction(pair.key, sizeof(*pair.key));
			hashValue = hashValue & ht[1].sizemask;
			p1 = ht[1].table[hashValue];
			while (p1)
			{
				if (*pair.key == *p1->key)
				{
					delete p1->val;
					p1->val = new VAL(*pair.val);
					return true;
				}
					
				p1 = p1->next;
			}
			auto p = new  dectEntry<VAL>(pair);
			p->next = ht[1].table[hashValue];
			ht[1].table[hashValue] = p;
			ht[1].used++;
			//旧节点加入到ht1
			if (ht[0].used > 0)
			{
				reHash();
			}

		}
		return true;
	}
	bool dictReName(sdshdr k)
	{
		unsigned int hashValue = dictGenHashFunction(k.sdsbuf(), k.sdslen());
		hashValue = hashValue & ht[0].sizemask;
		auto p1 = ht[0].table[hashValue];
		while (p1)
		{
			if (k == *p1->key)
			{
				delete p1->key;
				p1->key = new sdshdr(k);
				return true;
			}
			p1 = p1->next;
		}
		return false;
	}
	const dectEntry<VAL>* dictFetchValue(sdshdr  k)
	{
		unsigned int hashValue = dictGenHashFunction(&k, sizeof(k));
		hashValue = hashValue & ht[0].sizemask;
		auto p = ht[0].table[hashValue];
		while (p)
		{
			if (k == *p->key)
			{
				return  p;
			}
			p = p->next;
		}
		if (trehashhidx != -1)
		{
			if (ht[0].used > 0)
			{
				reHash();
			}
			unsigned int hashValue = dictGenHashFunction(&k, sizeof(k));
			hashValue = hashValue & ht[1].sizemask;
			auto p = ht[1].table[hashValue];
			while (p)
			{
				if (k == *p->key)
				{
					return p;
				}
				p = p->next;
			}
		}
		return nullptr;
	}
	const dectEntry<VAL>* dictGetRandomKey()
	{
		if (trehashhidx != -1)
		if (ht[0].used > 0)
		{
			reHash();
		}
	}
	bool dictDelete(sdshdr  k)
	{
		unsigned int hashValue = dictGenHashFunction(k.sdsbuf(), k.sdslen());
		hashValue = hashValue & ht[0].sizemask;
		auto p = ht[0].table[hashValue];
		auto pre = ht[0].table[hashValue];

		while (p)
		{
			if (k == *p->key)
			{
				if (p = ht[0].table[hashValue])
				{
					ht[0].table[hashValue]= p->next;
				}
				else
				{
					pre->next = p->next;
				}
				delete p;
				ht[0].used--;
				if (trehashhidx == -1)
				{
					if (ht[0].size > 4 && (double)ht[0].used / (double)ht[0].size <= 0.1)//缩容
					{
						ht[1].table = new dectEntry<VAL>* [ht[0].size / 2];
						ht[1].size = ht[0].size / 2;
						ht[1].sizemask = ht[1].size - 1;
						ht[1].used = 0;
						trehashhidx = 0;
					}
				}
				if (trehashhidx != -1)
				{
					if (ht[0].used > 0)
					{
						reHash();
					}
				}
				return  true;
			}
			pre = p;
			p = p->next;
		}
		if (trehashhidx != -1)
		{
			unsigned int hashValue = dictGenHashFunction(k.sdsbuf(), k.sdslen());
			hashValue = hashValue & ht[1].sizemask;
			p = ht[1].table[hashValue];
			pre = ht[1].table[hashValue];
			while (p)
			{
				if (k == *p->key)
				{
					if (p = ht[1].table[hashValue])
					{
						ht[1].table[hashValue] = p->next;
					}
					else
					{
						pre->next = p->next;
					}
					delete p;
					ht[1].used--;
					if (ht[0].used > 0)
					{
						reHash();
					}
					return  true;
				}
				pre = p;
				p = p->next;
			}
			if (ht[0].used > 0)
			{
				reHash();
			}
		}
		return false;
	}
	void reHash()
	{
		while (ht[0].table[trehashhidx] == 0) trehashhidx++;
		if (ht[0].table[trehashhidx])
		{
			unsigned int hashValue = dictGenHashFunction(ht[0].table[trehashhidx]->key->sdsbuf(), ht[0].table[trehashhidx]->key->sdslen());
			hashValue = hashValue & ht[1].sizemask;
			auto p = ht[0].table[trehashhidx];
			ht[0].table[trehashhidx] = ht[0].table[trehashhidx]->next;
			p->next = ht[1].table[hashValue];
			ht[1].table[hashValue] = p;


			ht[1].used++;
			ht[0].used--;
		}
		if (ht[0].used == 0)
		{
			delete[] ht[0].table;

			ht[0].table = ht[1].table;
			ht[0].size = ht[1].size;
			ht[0].sizemask = ht[0].size - 1;
			ht[0].used = ht[1].used;

			ht[1].table = nullptr;
			ht[1].size = 0;
			ht[1].sizemask = 0;
			ht[1].used = 0;
			trehashhidx = -1;
		}
	}
	unsigned int dictGenHashFunction(const void* key, int len) {
		uint32_t seed = 5381;
		const uint32_t m = 0x5bd1e995;
		const int r = 24;
		uint32_t h = seed ^ len;
		const unsigned char* data = (const unsigned char*)key;
		while (len >= 4) {
			uint32_t k = *(uint32_t*)data;

			k *= m;
			k ^= k >> r;
			k *= m;

			h *= m;
			h ^= k;

			data += 4;
			len -= 4;
		}

		switch (len) {
		case 3: h ^= data[2] << 16;
		case 2: h ^= data[1] << 8;
		case 1: h ^= data[0]; h *= m;
		};


		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;
		return (unsigned int)h;
	}
	~dict()
	{
		delete[]ht;
	}
};

