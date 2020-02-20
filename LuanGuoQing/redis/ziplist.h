#pragma once
#include"sdshdr.h"
#include <string.h>
#include <stdint.h>
#include<stdlib.h>
#define ZIP_END 255
#define ZIP_BIGLEN 254

/* Different encoding/length possibilities */
#define ZIP_STR_MASK 0x01
#define ZIP_INT_MASK 0x00

#define ZIP_NODE_HEADER_SIZE    (sizeof(uint16_t)*2+sizeof(unsigned char))
#define ZIP_NODE_PREVRAWLEN(p)  (*((uint16_t*)p))
#define ZIP_NODE_ENCODING(p)    (*(p + sizeof(uint16_t)))
#define ZIP_NODE_LEN(p)			(*((uint16_t*)(p+ sizeof(uint16_t)+sizeof(unsigned char))))
#define ZIP_NODE_BYTES(p)		(ZIP_NODE_HEADER_SIZE+ZIP_NODE_LEN(p))
#define ZIP_NODE_VALUE(p)		(p+ZIP_NODE_HEADER_SIZE)
/* Utility macros */
#define ZIPLIST_BYTES(zl)       (*((uint32_t*)(zl)))
#define ZIPLIST_TAIL_OFFSET(zl) (*((uint32_t*)((zl)+sizeof(uint32_t))))
#define ZIPLIST_LENGTH(zl)      (*((uint16_t*)((zl)+sizeof(uint32_t)*2)))
#define ZIPLIST_HEADER_SIZE     (sizeof(uint32_t)*2+sizeof(uint16_t))
#define ZIPLIST_ENTRY_HEAD(zl)  ((zl)+ZIPLIST_HEADER_SIZE)
#define ZIPLIST_ENTRY_TAIL(zl)  ((zl)+(ZIPLIST_TAIL_OFFSET(zl)))
#define ZIPLIST_ENTRY_END(zl)   ((zl)+(ZIPLIST_BYTES(zl))-1)

struct zlentry {
	unsigned int  prevrawlen;
	unsigned int  len;
	unsigned char encoding;
	unsigned char* p;
};
class ziplist
{
public:
	unsigned char* zl;
	ziplist()
	{
		unsigned int bytes = ZIPLIST_HEADER_SIZE + 1;
		zl = new unsigned char[bytes];
		ZIPLIST_BYTES(zl) = bytes;
		ZIPLIST_TAIL_OFFSET(zl) = ZIPLIST_HEADER_SIZE;
		ZIPLIST_LENGTH(zl) = 0;
		zl[bytes - 1] = ZIP_END;
	}
	ziplist(ziplist& res)
	{
		auto size = ZIPLIST_BYTES(res.zl);
		zl = new unsigned char[size];
		for (uint32_t i = 0; i < size; i++)
		{
			zl[i] = res.zl[i];
		}
	}
	zlentry zipEntry(unsigned char* p) {
		zlentry e;
		e.prevrawlen = ZIP_NODE_PREVRAWLEN(p);
		e.encoding =ZIP_NODE_ENCODING(p); 
		e.len = ZIP_NODE_LEN(p);
		e.p = p;
		return e;
	}
	bool ziplistInsert(unsigned char* p, int number)
	{
		if (p == nullptr)
			return false;
		if (ziplistFind(number) != nullptr)
			return false;
		unsigned int bytes = ZIPLIST_BYTES(zl) + ZIP_NODE_HEADER_SIZE + sizeof(int);
		unsigned char* newzl = new unsigned char[bytes];
		unsigned int offset = p-zl;
		if (p[0] == ZIP_END)
		{
			if(ZIPLIST_LENGTH(zl) == 0)
			{
				unsigned char* node = newzl + offset;
				ZIP_NODE_PREVRAWLEN(node) = 0;
				ZIP_NODE_ENCODING(node) = 0;
				ZIP_NODE_LEN(node) = 4;
				*((int*)ZIP_NODE_VALUE(node)) = number;
				ZIPLIST_BYTES(newzl) = bytes;
				ZIPLIST_TAIL_OFFSET(newzl) = ZIPLIST_HEADER_SIZE;
				ZIPLIST_LENGTH(newzl) = 1;
				newzl[bytes - 1] = ZIP_END;
			}
			else
			{
				
				for (int i = 0; i < offset; i++)
				{
					newzl[i] = zl[i];
				}
				unsigned char* node = newzl + offset;
				ZIP_NODE_PREVRAWLEN(node) = ZIP_NODE_BYTES(ZIPLIST_ENTRY_TAIL(zl));
				ZIP_NODE_ENCODING(node) = 0;
				ZIP_NODE_LEN(node) = 4;
				*((int*)(node + ZIP_NODE_HEADER_SIZE)) = number;

				ZIPLIST_BYTES(newzl) += ZIP_NODE_BYTES(node);
				ZIPLIST_TAIL_OFFSET(newzl) = offset;
				ZIPLIST_LENGTH(newzl)++;
				newzl[bytes - 1] = ZIP_END;
			}
			
		}
		else
		{
			for (int i = 0; i < offset; i++)
			{
				newzl[i] = zl[i];
			}
			unsigned char* node = newzl + offset;
			ZIP_NODE_PREVRAWLEN(node) = ZIP_NODE_PREVRAWLEN(p);
			ZIP_NODE_ENCODING(node) = 0;
			ZIP_NODE_LEN(node) = 4;
			*((int*)(node + ZIP_NODE_HEADER_SIZE)) = number;

			ZIP_NODE_PREVRAWLEN(p) = ZIP_NODE_BYTES(node);
			for (int i = offset + ZIP_NODE_BYTES(node); i < bytes;i++)
			{
				newzl[i] = zl[i- ZIP_NODE_BYTES(node)];
			}
			ZIPLIST_BYTES(newzl) += ZIP_NODE_BYTES(node);
			ZIPLIST_TAIL_OFFSET(newzl) += ZIP_NODE_BYTES(node);
			ZIPLIST_LENGTH(newzl)++;
			newzl[bytes - 1] = ZIP_END;
		}
		delete[] zl;
		zl = newzl;
		return 1;
	}
	bool ziplistInsert(unsigned char* p, const char * str)
	{
		if (p == nullptr)
			return false;
		if (ziplistFind(str) != nullptr)
			return false;
		unsigned int strLength = strlen(str);
		unsigned int bytes = ZIPLIST_BYTES(zl) + ZIP_NODE_HEADER_SIZE + strLength;
		unsigned char* newzl = new unsigned char[bytes];
		unsigned int offset = p - zl;
		if (p[0] == ZIP_END)
		{
			if (ZIPLIST_LENGTH(zl) == 0)
			{
				unsigned char* node = newzl + offset;
				ZIP_NODE_PREVRAWLEN(node) = 0;
				ZIP_NODE_ENCODING(node) = 1;
				ZIP_NODE_LEN(node) = strLength;
				for (int i = 0; i < strLength; i++)
				{
					*((char*)(ZIP_NODE_VALUE(node)+i)) = str[i];
				}
				ZIPLIST_BYTES(newzl) = bytes;
				ZIPLIST_TAIL_OFFSET(newzl) = offset;
				ZIPLIST_LENGTH(newzl) = 1;
				newzl[bytes - 1] = ZIP_END;
			}
			else
			{

				for (int i = 0; i < offset; i++)
				{
					newzl[i] = zl[i];
				}
				unsigned char* node = newzl + offset;
				ZIP_NODE_PREVRAWLEN(node) = ZIP_NODE_BYTES(ZIPLIST_ENTRY_TAIL(zl));
				ZIP_NODE_ENCODING(node) = 1;
				ZIP_NODE_LEN(node) = strLength;
				for (int i = 0; i < strLength; i++)
				{
					*((char*)(ZIP_NODE_VALUE(node) + i)) = str[i];
				}
				ZIPLIST_BYTES(newzl) += ZIP_NODE_BYTES(node);
				ZIPLIST_TAIL_OFFSET(newzl) = offset;
				ZIPLIST_LENGTH(newzl)++;
				newzl[bytes - 1] = ZIP_END;
			}

		}
		else
		{
			for (int i = 0; i < offset; i++)
			{
				newzl[i] = zl[i];
			}
			unsigned char* node = newzl + offset;
			ZIP_NODE_PREVRAWLEN(node) = ZIP_NODE_PREVRAWLEN(p);
			ZIP_NODE_ENCODING(node) = 1;
			ZIP_NODE_LEN(node) = strLength;
			for (int i = 0; i < strLength; i++)
			{
				*((char*)(node + ZIP_NODE_HEADER_SIZE + i)) = str[i];
			}
			ZIP_NODE_PREVRAWLEN(p) = ZIP_NODE_BYTES(node);
			for (int i = offset + ZIP_NODE_BYTES(node); i < bytes; i++)
			{
				newzl[i] = zl[i - ZIP_NODE_BYTES(node)];
			}
			ZIPLIST_BYTES(newzl) += ZIP_NODE_BYTES(node);
			ZIPLIST_TAIL_OFFSET(newzl) += ZIP_NODE_BYTES(p);
			ZIPLIST_LENGTH(newzl)++;
			newzl[bytes - 1] = ZIP_END;
		}
		delete[] zl;
		zl = newzl;
		return 1;
	}
	bool zipPushBack(int number)
	{
		return ziplistInsert(zl + ZIPLIST_BYTES(zl) - 1, number);
	}
	bool zipPushBack(const char * str)
	{
		return ziplistInsert(zl + ZIPLIST_BYTES(zl) - 1, str);
	}
	bool zipPushHead(int number)
	{
		return ziplistInsert(zl + ZIPLIST_HEADER_SIZE, number);
	}
	bool zipPushHead(const char* str)
	{
		return ziplistInsert(zl + ZIPLIST_HEADER_SIZE, str);
	}
	bool ziplistInsert(unsigned char* p, int number, int insertBack)
	{
		if (insertBack == 1)
		{
			if (p[0] == ZIP_END)
			{
				return false;
			}
			else
			{
				p += ZIP_NODE_BYTES(p);
				return ziplistInsert(p, number);
			}
		}
		return ziplistInsert(p, number);
	}
	bool ziplistInsert(unsigned char* p, const char* str, int insertBack)
	{
		if (insertBack == 1)
		{
			if (p[0] == ZIP_END)
			{
				return false;
			}
			else
			{
				p += ZIP_NODE_BYTES(p);
				return ziplistInsert(p, str);
			}
		}
		return ziplistInsert(p, str);
	}
	unsigned char* ziplistFind(int number) {
		unsigned char* p = ZIPLIST_ENTRY_TAIL(zl);
		if (p[0] == ZIP_END)
			return nullptr;
		while (1)
		{
			if (ZIP_NODE_ENCODING(p) == 0)
			{
				if (*((int*)ZIP_NODE_VALUE(p)) == number)
					return p;
			}
			if (ZIP_NODE_PREVRAWLEN(p) == 0)
				return nullptr;
			p -= ZIP_NODE_PREVRAWLEN(p);
		}
	}
	unsigned char* ziplistFind(const char * str) {
		unsigned char* p = ZIPLIST_ENTRY_TAIL(zl);
		if(p[0]==ZIP_END)
			return nullptr;
		while (1)
		{
			if (ZIP_NODE_ENCODING(p) == 1)
			{
				if (ZIP_NODE_LEN(p) == strlen(str))
				{
					for (int i = 0; i < ZIP_NODE_LEN(p); i++)
					{
						if (*((char*)(ZIP_NODE_VALUE(p) + i)) != str[i])
						{
							break;
						}
						if (i == ZIP_NODE_LEN(p)-1)
							return p;
					}
				}
				
			}
			if (ZIP_NODE_PREVRAWLEN(p) == 0)
				return nullptr;
			p -= ZIP_NODE_PREVRAWLEN(p);
		}
	}
	unsigned char* operator[] (unsigned int index) {
		if (index >= ZIPLIST_LENGTH(zl))
			return nullptr;
		unsigned char* p;
		for (unsigned int i = 0; i <= index; i++)
		{
			if (i == 0)
			{
				p = ZIPLIST_ENTRY_HEAD(zl);
			}
			else
			{
				p += ZIP_NODE_BYTES(p);
			}
		}
		return p;
	}
	unsigned char* ziplistIndex(unsigned int index){
		if (index >= ZIPLIST_LENGTH(zl))
			return nullptr;
		unsigned char* p;
		for (unsigned int i = 0; i <= index; i++)
		{
			if (i == 0)
			{
				p = ZIPLIST_ENTRY_HEAD(zl);
			}
			else
			{
				p += ZIP_NODE_BYTES(p);
			}
		}
		return p;
	}
	unsigned char* ziplistPrev(unsigned char* p) {
		if (ZIP_NODE_PREVRAWLEN(p) == 0)
			return nullptr;
		return p -= ZIP_NODE_PREVRAWLEN(p);
	}
	unsigned char* ziplistNext(unsigned char* p) {
		if (p[0]==ZIP_END)
			return nullptr;
		return p += ZIP_NODE_BYTES(p);
	}
	unsigned char* ziplistHeadNode()
	{
		return ZIPLIST_ENTRY_HEAD(zl);
	}
	unsigned char* ziplistTailNode()
	{
		return ZIPLIST_ENTRY_TAIL(zl);
	}
	bool ziplistDelete(unsigned char* p)
	{
		if (p == nullptr||p[0]==ZIP_END)
			return false;
		unsigned int psize = ZIP_NODE_BYTES(p);
		unsigned int bytes = ZIPLIST_BYTES(zl) - psize;
		unsigned char* newzl = new unsigned char[bytes];
		unsigned int offset = p - zl;
		
		auto pnext = ziplistNext(p);
		if (pnext)
		{
			ZIP_NODE_PREVRAWLEN(pnext) = ZIP_NODE_PREVRAWLEN(p);
		}

		for (int i = 0; i < offset; i++)
		{
			newzl[i] = zl[i];
		}
		for (int i = offset + psize; i < ZIPLIST_BYTES(zl); i++)
		{
			newzl[i - psize] = zl[i];
		}
		ZIPLIST_BYTES(newzl) -= psize;
		if (pnext)
		{
			ZIPLIST_TAIL_OFFSET(newzl) -= psize;
		}
		else
		{
			ZIPLIST_TAIL_OFFSET(newzl) -= ZIP_NODE_PREVRAWLEN(p);
		}
		ZIPLIST_LENGTH(newzl)--;
		newzl[bytes - 1] = ZIP_END;
		delete[] zl;
		zl = newzl;
		return true;
	}
	char *  ziplistGetStr(unsigned char* p )
	{
		char * val=new char[ZIP_NODE_LEN(p)+1];
		int i = 0;
		for (; i < ZIP_NODE_LEN(p); i++)
		{
			val[i] = *(ZIP_NODE_VALUE(p) + i);
		}
		val[i] = '\0';
		return val;
	}
	int ziplistGetInt(unsigned char* p)
	{
		return *((int *)ZIP_NODE_VALUE(p));
	}
	~ziplist()
	{
		delete[]zl;
	}
};
