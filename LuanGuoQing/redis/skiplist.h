#pragma once
#include"sdshdr.h"
#include<iostream>
#include<ctime>
class skiplistNode;
class skiplistLevel
{
public:
	skiplistNode* forword;
	unsigned int span;
	skiplistLevel()
	{
		forword = nullptr;
		span = 0;
	}
};
class skiplistNode
{
public:
	skiplistLevel *levelArray;
	skiplistNode* backword;
	int level;
	double score;
	sdshdr * obj;
	skiplistNode(unsigned int l)
	{
		levelArray = new skiplistLevel[l];
		backword = nullptr;
		level = l-1;
		score =-1000000;
		obj = nullptr;
	}
	skiplistNode(unsigned int l,double s,sdshdr &o)
	{
		levelArray = new skiplistLevel[l];
		backword = nullptr;
		level = l-1;
		score = s;
		obj = new sdshdr(o);
	}
	~skiplistNode()
	{
		delete []levelArray;
		levelArray = nullptr;
		level = 0;
		backword = nullptr;
		score = 0;
		obj = nullptr;
	}
};

class skiplist
{
public:
	skiplistNode* header;
	skiplistNode* tail;
	int level;
	unsigned int length;
	skiplist()
	{
		header = new skiplistNode(32);
		tail = header;
		level = 0;
		length = 0;
	}
	skiplist(skiplist& res)
	{
		auto p = res.header->levelArray[0].forword;
		while (p)
		{
			insert(p->score, *p->obj);
			p = p->levelArray[0].forword;
		}
	}
	bool insert(double s, sdshdr o)
	{
		int l = randomLevel();
		auto newNode = new skiplistNode(l, s, o);
		skiplistNode* p=header;
		int tmplevel = level;
		while (1)
		{
			if (p->levelArray[tmplevel].forword == nullptr)
			{
				if (tmplevel == 0)
				{
					insertNode(p, newNode);
					return true;
				}
				else
				{
					tmplevel--;
				}
			}
			else if (p->levelArray[tmplevel].forword->score < s)
			{
				p = p->levelArray[tmplevel].forword;
			}
			else if (p->levelArray[tmplevel].forword->score > s)
			{
				if(p->levelArray[tmplevel].span==1)
				{
					
					return insertNode(p, newNode);
				}
				else
				{
					tmplevel--;
				}
			}
			else
			{
				
				
				return insertNode(p->levelArray[tmplevel].forword, newNode);
			}
		}	
	}
	bool insertNode(skiplistNode* p,skiplistNode* newNode)
	{
		while (p->score == newNode->score)
		{
			p = p->backword;
		}
		while (p->levelArray[0].forword&&newNode->obj->sdscmp(*p->levelArray[0].forword->obj) > 0)
		{
			if (p->levelArray[0].forword->score == newNode->score)
				p = p->levelArray[0].forword;
			else
				break;
		}
		if (p->levelArray[0].forword && p->levelArray[0].forword->score == newNode->score&& newNode->obj->sdscmp(*p->levelArray[0].forword->obj) == 0)
		{
			delete newNode;
			return false;
		}
		unsigned int tmpSpan = 1;
		level = level > newNode->level ? level : newNode->level;
		newNode->backword = p;
		if(p->levelArray[0].forword)
		  p->levelArray[0].forword->backword = newNode;
		for (int i = 0; i <= level;)
		{
			if (i <= newNode->level)
			{
				if (i > p->level)
				{
					p = p->backword;
					tmpSpan++;
				}
				else
				{
					if (p->levelArray[i].forword)
					{
						newNode->levelArray[i].forword = p->levelArray[i].forword;
						newNode->levelArray[i].span = p->levelArray[i].span + 1 - tmpSpan;
					}
					p->levelArray[i].forword = newNode;
					p->levelArray[i].span = tmpSpan;
					i++;
				}
			}
			else
			{
				if (i > p->level)
				{
					p = p->backword;
				}
				else
				{
					if(p->levelArray[i].forword)
						p->levelArray[i].span++;
					i++;
				}
			}
			
		}
		length++;
		return true;
	}
	bool zslDelete(double s, sdshdr o)
	{
		skiplistNode* p = header;
		int tmplevel = level;
		while (1)
		{
			if (p->levelArray[tmplevel].forword == nullptr)
			{
				if (tmplevel > 0)
					tmplevel--;
				else
				    return false;
			}
			else if (p->levelArray[tmplevel].forword->score < s)
			{
				p = p->levelArray[tmplevel].forword;
			}
			else if (p->levelArray[tmplevel].forword->score > s)
			{
				if (p->levelArray[tmplevel].span == 1)
				{
					return false;
				}
				else
				{
					tmplevel--;
				}
			}
			else
			{
				p = p->levelArray[tmplevel].forword;
				while (p->score == p->backword->score)
				{
					p = p->backword;
				}
				while (o.sdscmp(*p->obj) > 0)
				{
					if (p->levelArray[0].forword && p->levelArray[0].forword->score == s)
						p = p->levelArray[0].forword;
					else
						break;
				}
				if (p&& p->score == s && o.sdscmp(*p->obj) == 0)
				{
					return deleteNode(p);
				}
				return false;
			}
		}
	}
	bool deleteNode(skiplistNode* p)
	{
		auto pre = p->backword;
		if (p->levelArray[0].forword)
		{
			p->levelArray[0].forword->backword = pre;
		}
		for (int i = 0; i <= level;)
		{
			if (i <= p->level)
			{
				if (i > pre->level)
				{
					pre = pre->backword;
				}
				else
				{
					if (p->levelArray[i].forword)
					{
						pre->levelArray[i].forword = p->levelArray[i].forword;
						pre->levelArray[i].span += (p->levelArray[i].span - 1);
					}
					else
					{
						pre->levelArray[i].forword = nullptr;
						pre->levelArray[i].span = 0;
					}
					i++;
				}
			}
			else
			{
				if (i > pre->level)
				{
					pre = pre->backword;
				}
				else
				{
					if(pre->levelArray[i].forword)
						pre->levelArray[i].span--;
					i++;
				}
			}
			
		}
		if (level == p->level)
		{
			for (int i = level; i >= 0; i--)
			{
				if (header->levelArray[i].forword)
				{
					level = i;
					break;
				}

			}
			level = 0;
		}
		delete p;
		length--;
		return true;
	}
	int randomLevel()
	{
		int level=1;
		int i = rand() % 2;
		while (i)
		{
			level++;
			i = rand() % 2;
		}
		return level;
	}
	int zslGetRank(double s, sdshdr o)
	{
		auto p = header->levelArray[0].forword;
		int rank = 0;
		while (p)
		{
			if (p->score == s && *p->obj == o)
			{
				return rank;
			}
			else if (p->score > s)
			{
				return -1;
			}
			else
			{
				p = p->levelArray[0].forword;
				rank++;
			}
		}
		return -1;
	}
	skiplistNode* zslGetElementByRank(int rank)
	{
		if (rank < 0)
			return nullptr;
		auto p = header->levelArray[0].forword;
		int i = 0;
		while (p)
		{
			if (rank == i)
			{
				return p;
			}
			i++;
			p = p->levelArray[0].forword;

		}
		return nullptr;
	}
	bool zslIsInRange(double begin, double end)
	{
		skiplistNode* p = header;
		int tmplevel = level;
		while (1)
		{
			if (p->levelArray[tmplevel].forword == nullptr)
			{
				if (tmplevel > 0)
					tmplevel--;
				else
					return false;
			}
			else if (p->levelArray[tmplevel].forword->score < begin)
			{
				p = p->levelArray[tmplevel].forword;
			}
			else if (p->levelArray[tmplevel].forword->score >= begin)
			{
				if (p->levelArray[tmplevel].forword->score <= end)
				{
					return true;
				}
				if (p->levelArray[tmplevel].span == 1)
				{
					return false;
				}
				else
				{
					tmplevel--;
				}
			}
			
		}
	}
	skiplistNode* zslFirstInRange(double begin, double end)
	{
		skiplistNode* p = header;
		int tmplevel = level;
		while (1)
		{
			if (p->levelArray[tmplevel].forword == nullptr)
			{
				if (tmplevel > 0)
					tmplevel--;
				else
					return nullptr;
			}
			else if (p->levelArray[tmplevel].forword->score < begin)
			{
				p = p->levelArray[tmplevel].forword;
			}
			else if (p->levelArray[tmplevel].forword->score >= begin)
			{
				if (p->levelArray[tmplevel].forword->score <= end)
				{
					p = p->levelArray[tmplevel].forword;
					while (p->score == p->backword->score)
					{
						p = p->backword;
					}
					return p;
				}
				if (p->levelArray[tmplevel].span == 1)
				{
					return nullptr;
				}
				else
				{
					tmplevel--;
				}
			}
		}
	}
	skiplistNode* zslLastInRange(double begin, double end)
	{
		skiplistNode* p = header;
		int tmplevel = level;
		while (1)
		{
			if (p->levelArray[tmplevel].forword == nullptr)
			{
				if (tmplevel > 0)
					tmplevel--;
				else
				{
					if (p->score<=end&&p->score >= begin )
					{
						return p;
					}
					return nullptr;
				}
				
			}
			else if (p->levelArray[tmplevel].forword->score <= end)
			{
				p = p->levelArray[tmplevel].forword;
			}
			else if (p->levelArray[tmplevel].forword->score > end)
			{
				if (p->levelArray[tmplevel].span == 1)
				{
					if (p->score >= begin&&p!=header)
					{
						while (p->score == p->levelArray[0].forword->score)
						{
							p = p->levelArray[0].forword;
						}
						return p;
					}
					return nullptr;
				}
				else
				{
					tmplevel--;
				}
			}
		}
	}
	void zslDeleteRangeByScore(double begin, double end)
	{
		if (end < begin)
			return;
		while (auto p = zslFirstInRange(begin, end))
		{
			deleteNode(p);
		}
	}
	void zslDeleteRangeByRank(int begin, int end)
	{
		if (end < begin)
			return;
		begin = begin < 0 ? 0 : begin;
		end = end < length ? end : length - 1;
		for (int i = begin; i <= end;i++)
		{
				if (auto p = zslGetElementByRank(begin))
				{
					deleteNode(p);
				}	
		}
	}
	~skiplist()
	{
		auto p = header;
		auto next = p->levelArray[0].forword;
		while (p)
		{
			next = p->levelArray[0].forword;
			delete p;
			p = next;
		}
		header = nullptr;
		tail = header;
		level = 0;
		length = 0;
	}
};