#pragma once
#include<string.h>
#include<iostream>
class sdshdr
{
private:
	size_t len;
	size_t free;
	char* buf;
public:

	sdshdr() {
		buf = NULL;
		len = 0;
		free = 0;
	}

	sdshdr(const char* str) :sdshdr(str, strlen(str)) {

	}

	sdshdr(const void* init, size_t initlen)
	{
		buf = new char[initlen + 1];
		for (size_t i = 0; i < initlen; i++)
		{
			buf[i] = ((char*)init)[i];
		}
		buf[initlen] = '\0';
		len = initlen;
		free = 0;
	}

	sdshdr(const sdshdr& res)
	{
		buf = new char[res.len + res.free + 1];
		for (size_t i = 0; i < res.len; i++)
		{
			buf[i] = res.buf[i];
		}
		buf[res.len] = '\0';
		len = res.len;
		free = res.free;
	}

	sdshdr& operator = (const sdshdr& res)
	{
		delete[] buf;
		if (res.buf != NULL)
		{
			buf = new char[res.len + res.free + 1];
			for (unsigned int i = 0; i < res.len; i++)
			{
				buf[i] = ((char*)res.buf)[i];
			}
			buf[res.len] = '\0';

		}
		else
			buf = NULL;
		len = res.len;
		free = res.free;
		return *this;
	}

	size_t sdslen()
	{
		return len;
	}

	size_t sdsfree()
	{
		return free;
	}
	const char* sdsbuf()
	{
		return buf;
	}
	void sdsclear()
	{
		delete[] buf;
		buf = NULL;
		len = 0;
		free = 0;
	}
	void sdscatlen(const char* str, size_t catlen)
	{
		if (free < catlen)
		{
			//sdsgrowzero(2 * (len + catlen));
			size_t newlen = 2 * (len + catlen);
			char* tmp = new char[newlen + 1];
			for (size_t i = 0; i < len; i++)
			{
				tmp[i] = buf[i];
			}
			delete[] buf;
			buf = tmp;
			for (size_t i = len; i <= newlen; i++)
			{
				buf[i] = '\0';
			}
			free = newlen - len;
		}
		for (size_t i = len; i < len + catlen; i++)
		{
			buf[i] = str[i - len];
		}
		buf[len + catlen] = '\0';
		len += catlen;
		free -= catlen;

	}
	void sdscat(const char* str)
	{
		sdscatlen(str, strlen(str));
	}
	void sdscat(const sdshdr& res)
	{
		sdscatlen(res.buf, res.len);
	}
	void sdsgrowzero(size_t newlen)
	{
		char* tmp = new char[newlen + 1];
		for (size_t i = 0; i < len; i++)
		{
			tmp[i] = buf[i];
		}
		delete[]buf;
		buf = tmp;
		for (size_t i = len; i <= newlen; i++)
		{
			buf[i] = '\0';
		}
		free = newlen - len;
	}
	void sdsrange(size_t begin, size_t end)
	{
		for (size_t i = begin; i < end; i++)
		{
			buf[i - begin] = buf[i];
		}
		buf[end - begin] = '\0';
		free += len;
		len = end - begin;
		free -= len;

	}
	void sdstrim(const char* str)
	{
		for (size_t i = 0; i < strlen(str); i++)
		{
			int flag = -1;
			for (size_t j = 0; j < len; j++)
			{
				if (buf[j] == str[i])
				{
					buf[j] = '\0';
				}
				else
				{
					buf[++flag] = buf[j];
					buf[j] = '\0';
				}
			}
		}
		free += len;
		len = strlen(buf);
		free -= len;
	}
	int operator == (const sdshdr& cmpsds)
	{
		if (sdscmp(cmpsds) ==0)
			return 1;
		else
			return 0;
	}
	int sdscmp(const sdshdr& cmpsds) {
		if (len != cmpsds.len)
			return len < cmpsds.len ? -1 : 1;
		unsigned int i = 0;
		while (buf[i] == cmpsds.buf[i] && i < len) i++;
		if (i == len)
			return 0;
		return buf[i] < cmpsds.buf[i] ? -1 : 1;
	}
	~sdshdr()
	{
		delete[] buf;
		buf = NULL;
	}
};



