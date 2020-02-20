#pragma once
#include<sys/time.h>
#include"dict.h"
#include"sdshdr.h"
#include"intset.h"
#include"list.h"
#include"skiplist.h"
#include"ziplist.h"
#include"zset.h"
/* Object types */
#define REDIS_STRING 0
#define REDIS_LIST 1
#define REDIS_SET 2
#define REDIS_ZSET 3
#define REDIS_HASH 4
/* Objects encoding. Some kind of objects like Strings and Hashes can be
 * internally represented in multiple ways. The 'encoding' field of the object
 * is set to one of this fields for this object. */
#define REDIS_ENCODING_RAW 0     /* Raw representation */
#define REDIS_ENCODING_INT 1     /* Encoded as integer */
#define REDIS_ENCODING_HT 2      /* Encoded as hash table */
#define REDIS_ENCODING_ZIPMAP 3  /* Encoded as zipmap */
#define REDIS_ENCODING_LINKEDLIST 4 /* Encoded as regular linked list */
#define REDIS_ENCODING_ZIPLIST 5 /* Encoded as ziplist */
#define REDIS_ENCODING_INTSET 6  /* Encoded as intset */
#define REDIS_ENCODING_SKIPLIST 7  /* Encoded as skiplist */


struct redisCommand {
	char* name;
	int arity;
	char* sflags; /* Flags as string representation, one char per flag. */
	int flags;    /* The actual flags, obtained from the 'sflags' field. */
	/* Use a function to determine keys arguments in a command line. */
	/* What keys should be loaded in background when calling this command? */
	int firstkey; /* The first argument that's a key (0 = no keys) */
	int lastkey;  /* The last argument that's a key */
	int keystep;  /* The step between first and last key */
	long long microseconds, calls;
};
//struct redisCommand redisCommandTable[] = {
//	{"get",2,"r",0,1,1,1,0,0},
//	{"set",setCommand,-3,"wm",0,noPreloadGetKeys,1,1,1,0,0},
//	{"setnx",setnxCommand,3,"wm",0,noPreloadGetKeys,1,1,1,0,0},
//	{"setex",setexCommand,4,"wm",0,noPreloadGetKeys,1,1,1,0,0},
//	{"psetex",psetexCommand,4,"wm",0,noPreloadGetKeys,1,1,1,0,0},
//	{"append",appendCommand,3,"wm",0,NULL,1,1,1,0,0},
//	{"strlen",strlenCommand,2,"r",0,NULL,1,1,1,0,0},
//	{"del",delCommand,-2,"w",0,noPreloadGetKeys,1,-1,1,0,0},
//	{"exists",existsCommand,2,"r",0,NULL,1,1,1,0,0},
//	{"setbit",setbitCommand,4,"wm",0,NULL,1,1,1,0,0},
//	{"getbit",getbitCommand,3,"r",0,NULL,1,1,1,0,0},
//	{"setrange",setrangeCommand,4,"wm",0,NULL,1,1,1,0,0},
//	{"getrange",getrangeCommand,4,"r",0,NULL,1,1,1,0,0},
//	{"substr",getrangeCommand,4,"r",0,NULL,1,1,1,0,0},
//	{"incr",incrCommand,2,"wm",0,NULL,1,1,1,0,0},
//	{"decr",decrCommand,2,"wm",0,NULL,1,1,1,0,0},
//	{"mget",mgetCommand,-2,"r",0,NULL,1,-1,1,0,0},
//	{"rpush",rpushCommand,-3,"wm",0,NULL,1,1,1,0,0},
//	{"lpush",lpushCommand,-3,"wm",0,NULL,1,1,1,0,0},
//	{"rpushx",rpushxCommand,3,"wm",0,NULL,1,1,1,0,0},
//	{"lpushx",lpushxCommand,3,"wm",0,NULL,1,1,1,0,0},
//	{"linsert",linsertCommand,5,"wm",0,NULL,1,1,1,0,0},
//	{"rpop",rpopCommand,2,"w",0,NULL,1,1,1,0,0},
//	{"lpop",lpopCommand,2,"w",0,NULL,1,1,1,0,0},
//	{"brpop",brpopCommand,-3,"ws",0,NULL,1,1,1,0,0},
//	{"brpoplpush",brpoplpushCommand,4,"wms",0,NULL,1,2,1,0,0},
//	{"blpop",blpopCommand,-3,"ws",0,NULL,1,-2,1,0,0},
//	{"llen",llenCommand,2,"r",0,NULL,1,1,1,0,0},
//	{"lindex",lindexCommand,3,"r",0,NULL,1,1,1,0,0},
//	{"lset",lsetCommand,4,"wm",0,NULL,1,1,1,0,0},
//	{"lrange",lrangeCommand,4,"r",0,NULL,1,1,1,0,0},
//	{"ltrim",ltrimCommand,4,"w",0,NULL,1,1,1,0,0},
//	{"lrem",lremCommand,4,"w",0,NULL,1,1,1,0,0},
//	{"rpoplpush",rpoplpushCommand,3,"wm",0,NULL,1,2,1,0,0},
//	{"sadd",saddCommand,-3,"wm",0,NULL,1,1,1,0,0},
//	{"srem",sremCommand,-3,"w",0,NULL,1,1,1,0,0},
//	{"smove",smoveCommand,4,"w",0,NULL,1,2,1,0,0},
//	{"sismember",sismemberCommand,3,"r",0,NULL,1,1,1,0,0},
//	{"scard",scardCommand,2,"r",0,NULL,1,1,1,0,0},
//	{"spop",spopCommand,2,"wRs",0,NULL,1,1,1,0,0},
//	{"srandmember",srandmemberCommand,-2,"rR",0,NULL,1,1,1,0,0},
//	{"sinter",sinterCommand,-2,"rS",0,NULL,1,-1,1,0,0},
//	{"sinterstore",sinterstoreCommand,-3,"wm",0,NULL,1,-1,1,0,0},
//	{"sunion",sunionCommand,-2,"rS",0,NULL,1,-1,1,0,0},
//	{"sunionstore",sunionstoreCommand,-3,"wm",0,NULL,1,-1,1,0,0},
//	{"sdiff",sdiffCommand,-2,"rS",0,NULL,1,-1,1,0,0},
//	{"sdiffstore",sdiffstoreCommand,-3,"wm",0,NULL,1,-1,1,0,0},
//	{"smembers",sinterCommand,2,"rS",0,NULL,1,1,1,0,0},
//	{"sscan",sscanCommand,-3,"rR",0,NULL,1,1,1,0,0},
//	{"zadd",zaddCommand,-4,"wm",0,NULL,1,1,1,0,0},
//	{"zincrby",zincrbyCommand,4,"wm",0,NULL,1,1,1,0,0},
//	{"zrem",zremCommand,-3,"w",0,NULL,1,1,1,0,0},
//	{"zremrangebyscore",zremrangebyscoreCommand,4,"w",0,NULL,1,1,1,0,0},
//	{"zremrangebyrank",zremrangebyrankCommand,4,"w",0,NULL,1,1,1,0,0},
//	{"zunionstore",zunionstoreCommand,-4,"wm",0,zunionInterGetKeys,0,0,0,0,0},
//	{"zinterstore",zinterstoreCommand,-4,"wm",0,zunionInterGetKeys,0,0,0,0,0},
//	{"zrange",zrangeCommand,-4,"r",0,NULL,1,1,1,0,0},
//	{"zrangebyscore",zrangebyscoreCommand,-4,"r",0,NULL,1,1,1,0,0},
//	{"zrevrangebyscore",zrevrangebyscoreCommand,-4,"r",0,NULL,1,1,1,0,0},
//	{"zcount",zcountCommand,4,"r",0,NULL,1,1,1,0,0},
//	{"zrevrange",zrevrangeCommand,-4,"r",0,NULL,1,1,1,0,0},
//	{"zcard",zcardCommand,2,"r",0,NULL,1,1,1,0,0},
//	{"zscore",zscoreCommand,3,"r",0,NULL,1,1,1,0,0},
//	{"zrank",zrankCommand,3,"r",0,NULL,1,1,1,0,0},
//	{"zrevrank",zrevrankCommand,3,"r",0,NULL,1,1,1,0,0},
//	{"zscan",zscanCommand,-3,"rR",0,NULL,1,1,1,0,0},
//	{"hset",hsetCommand,4,"wm",0,NULL,1,1,1,0,0},
//	{"hsetnx",hsetnxCommand,4,"wm",0,NULL,1,1,1,0,0},
//	{"hget",hgetCommand,3,"r",0,NULL,1,1,1,0,0},
//	{"hmset",hmsetCommand,-4,"wm",0,NULL,1,1,1,0,0},
//	{"hmget",hmgetCommand,-3,"r",0,NULL,1,1,1,0,0},
//	{"hincrby",hincrbyCommand,4,"wm",0,NULL,1,1,1,0,0},
//	{"hincrbyfloat",hincrbyfloatCommand,4,"wm",0,NULL,1,1,1,0,0},
//	{"hdel",hdelCommand,-3,"w",0,NULL,1,1,1,0,0},
//	{"hlen",hlenCommand,2,"r",0,NULL,1,1,1,0,0},
//	{"hkeys",hkeysCommand,2,"rS",0,NULL,1,1,1,0,0},
//	{"hvals",hvalsCommand,2,"rS",0,NULL,1,1,1,0,0},
//	{"hgetall",hgetallCommand,2,"r",0,NULL,1,1,1,0,0},
//	{"hexists",hexistsCommand,3,"r",0,NULL,1,1,1,0,0},
//	{"hscan",hscanCommand,-3,"rR",0,NULL,1,1,1,0,0},
//	{"incrby",incrbyCommand,3,"wm",0,NULL,1,1,1,0,0},
//	{"decrby",decrbyCommand,3,"wm",0,NULL,1,1,1,0,0},
//	{"incrbyfloat",incrbyfloatCommand,3,"wm",0,NULL,1,1,1,0,0},
//	{"getset",getsetCommand,3,"wm",0,NULL,1,1,1,0,0},
//	{"mset",msetCommand,-3,"wm",0,NULL,1,-1,2,0,0},
//	{"msetnx",msetnxCommand,-3,"wm",0,NULL,1,-1,2,0,0},
//	{"randomkey",randomkeyCommand,1,"rR",0,NULL,0,0,0,0,0},
//	{"select",selectCommand,2,"rl",0,NULL,0,0,0,0,0},
//	{"move",moveCommand,3,"w",0,NULL,1,1,1,0,0},
//	{"rename",renameCommand,3,"w",0,renameGetKeys,1,2,1,0,0},
//	{"renamenx",renamenxCommand,3,"w",0,renameGetKeys,1,2,1,0,0},
//	{"expire",expireCommand,3,"w",0,NULL,1,1,1,0,0},
//	{"expireat",expireatCommand,3,"w",0,NULL,1,1,1,0,0},
//	{"pexpire",pexpireCommand,3,"w",0,NULL,1,1,1,0,0},
//	{"pexpireat",pexpireatCommand,3,"w",0,NULL,1,1,1,0,0},
//	{"keys",keysCommand,2,"rS",0,NULL,0,0,0,0,0},
//	{"scan",scanCommand,-2,"rR",0,NULL,0,0,0,0,0},
//	{"dbsize",dbsizeCommand,1,"r",0,NULL,0,0,0,0,0},
//	{"auth",authCommand,2,"rslt",0,NULL,0,0,0,0,0},
//	{"ping",pingCommand,1,"rt",0,NULL,0,0,0,0,0},
//	{"echo",echoCommand,2,"r",0,NULL,0,0,0,0,0},
//	{"save",saveCommand,1,"ars",0,NULL,0,0,0,0,0},
//	{"bgsave",bgsaveCommand,1,"ar",0,NULL,0,0,0,0,0},
//	{"bgrewriteaof",bgrewriteaofCommand,1,"ar",0,NULL,0,0,0,0,0},
//	{"shutdown",shutdownCommand,-1,"arl",0,NULL,0,0,0,0,0},
//	{"lastsave",lastsaveCommand,1,"rR",0,NULL,0,0,0,0,0},
//	{"type",typeCommand,2,"r",0,NULL,1,1,1,0,0},
//	{"multi",multiCommand,1,"rs",0,NULL,0,0,0,0,0},
//	{"exec",execCommand,1,"sM",0,NULL,0,0,0,0,0},
//	{"discard",discardCommand,1,"rs",0,NULL,0,0,0,0,0},
//	{"sync",syncCommand,1,"ars",0,NULL,0,0,0,0,0},
//	{"psync",syncCommand,3,"ars",0,NULL,0,0,0,0,0},
//	{"replconf",replconfCommand,-1,"arslt",0,NULL,0,0,0,0,0},
//	{"flushdb",flushdbCommand,1,"w",0,NULL,0,0,0,0,0},
//	{"flushall",flushallCommand,1,"w",0,NULL,0,0,0,0,0},
//	{"sort",sortCommand,-2,"wm",0,NULL,1,1,1,0,0},
//	{"info",infoCommand,-1,"rlt",0,NULL,0,0,0,0,0},
//	{"monitor",monitorCommand,1,"ars",0,NULL,0,0,0,0,0},
//	{"ttl",ttlCommand,2,"r",0,NULL,1,1,1,0,0},
//	{"pttl",pttlCommand,2,"r",0,NULL,1,1,1,0,0},
//	{"persist",persistCommand,2,"w",0,NULL,1,1,1,0,0},
//	{"slaveof",slaveofCommand,3,"ast",0,NULL,0,0,0,0,0},
//	{"debug",debugCommand,-2,"as",0,NULL,0,0,0,0,0},
//	{"config",configCommand,-2,"ar",0,NULL,0,0,0,0,0},
//	{"subscribe",subscribeCommand,-2,"rpslt",0,NULL,0,0,0,0,0},
//	{"unsubscribe",unsubscribeCommand,-1,"rpslt",0,NULL,0,0,0,0,0},
//	{"psubscribe",psubscribeCommand,-2,"rpslt",0,NULL,0,0,0,0,0},
//	{"punsubscribe",punsubscribeCommand,-1,"rpslt",0,NULL,0,0,0,0,0},
//	{"publish",publishCommand,3,"pltr",0,NULL,0,0,0,0,0},
//	{"pubsub",pubsubCommand,-2,"pltrR",0,NULL,0,0,0,0,0},
//	{"watch",watchCommand,-2,"rs",0,noPreloadGetKeys,1,-1,1,0,0},
//	{"unwatch",unwatchCommand,1,"rs",0,NULL,0,0,0,0,0},
//	{"restore",restoreCommand,4,"awm",0,NULL,1,1,1,0,0},
//	{"migrate",migrateCommand,6,"aw",0,NULL,0,0,0,0,0},
//	{"dump",dumpCommand,2,"ar",0,NULL,1,1,1,0,0},
//	{"object",objectCommand,-2,"r",0,NULL,2,2,2,0,0},
//	{"client",clientCommand,-2,"ar",0,NULL,0,0,0,0,0},
//	{"eval",evalCommand,-3,"s",0,zunionInterGetKeys,0,0,0,0,0},
//	{"evalsha",evalShaCommand,-3,"s",0,zunionInterGetKeys,0,0,0,0,0},
//	{"slowlog",slowlogCommand,-2,"r",0,NULL,0,0,0,0,0},
//	{"script",scriptCommand,-2,"ras",0,NULL,0,0,0,0,0},
//	{"time",timeCommand,1,"rR",0,NULL,0,0,0,0,0},
//	{"bitop",bitopCommand,-4,"wm",0,NULL,2,-1,1,0,0},
//	{"bitcount",bitcountCommand,-2,"r",0,NULL,1,1,1,0,0}
//};


class redisObject
{
public:
	unsigned type : 4;
	unsigned encoding : 4;
	unsigned lru : 22;
	int * refcount;
	void * ptr;
	redisObject()
	{
		type = 0xf;
		encoding = 0xf;
		lru = 0;
		refcount = new int(1);
		ptr = nullptr;
	}
	redisObject(redisObject& res)
	{
		type = res.type;
		encoding = res.encoding;
		lru = res.lru;
		refcount = res.refcount;
		*refcount++;
		ptr = res.ptr;
		
	}
	redisObject & operator =(redisObject & res)
	{
		*refcount--;
		if (*refcount == 0)
		{
			if (ptr)
			{
				switch (encoding)
				{
				case(REDIS_ENCODING_RAW):
					ptr = (sdshdr*)ptr;
					break;
				case(REDIS_ENCODING_INT):
					ptr = nullptr;
					break;
				case(REDIS_ENCODING_HT):
					ptr = (dict<sdshdr>*)ptr;
					break;
				case(REDIS_ENCODING_ZIPMAP):
					ptr = (ziplist*)ptr;
					break;
				case(REDIS_ENCODING_LINKEDLIST):
					ptr = (list<sdshdr>*)ptr;
					break;
				case(REDIS_ENCODING_ZIPLIST):
					ptr = (zset*)ptr;
					break;
				case(REDIS_ENCODING_INTSET):
					ptr = (intset*)ptr;
					break;
				case(REDIS_ENCODING_SKIPLIST):
					ptr = (skiplist*)ptr;
					break;
				default:
					break;
				}
				delete ptr;
				ptr = nullptr;
			}
			delete refcount;
		}
		type = res.type;
		encoding = res.encoding;
		lru = res.lru;
		refcount = res.refcount;
		*refcount++;
		ptr = res.ptr;
		return *this;
	}
	~redisObject()
	{
		*refcount--;
		if (*refcount == 0)
		{
			if (ptr)
			{
				switch (encoding)
				{
				case(REDIS_ENCODING_RAW):
					ptr = (sdshdr*)ptr;
					break;
				case(REDIS_ENCODING_INT):
					ptr = nullptr;
					break;
				case(REDIS_ENCODING_HT):
					ptr = (dict<sdshdr>*)ptr;
					break;
				case(REDIS_ENCODING_ZIPMAP):
					ptr = (ziplist*)ptr;
					break;
				case(REDIS_ENCODING_LINKEDLIST):
					ptr = (list<sdshdr>*)ptr;
					break;
				case(REDIS_ENCODING_ZIPLIST):
					ptr = (zset*)ptr;
					break;
				case(REDIS_ENCODING_INTSET):
					ptr = (intset*)ptr;
					break;
				case(REDIS_ENCODING_SKIPLIST):
					ptr = (skiplist*)ptr;
					break;
				default:
					break;
				}
				delete ptr;
				ptr = nullptr;
			}
			delete refcount;
		}
		
	}
};

class redisDb {
public:
	dict<redisObject>* dictionaries;
	dict<long long>* expries;
	redisDb()
	{
		dictionaries = new dict<redisObject>();
		expries = new dict<long long>();
	}
	bool add(const sdshdr  key,redisObject  val)
	{
		return dictionaries->dictAdd(key, val);
	}
	bool del(const sdshdr  key) {
		return dictionaries->dictDelete(key);
	}
	bool replace(const sdshdr  key, redisObject  val)
	{
		return dictionaries->dictReplace(key, val);
	}
	bool removeAll()
	{
		delete dictionaries;
		dictionaries = new dict<redisObject>();
	}
	unsigned int size()
	{
		if (dictionaries->trehashhidx != -1)
			return dictionaries->ht[0].used;
		else
			return dictionaries->ht[0].used + dictionaries->ht[1].used;
	}
	bool rename(const sdshdr k)
	{
		return  dictionaries->dictReName(k);
	}
	~redisDb()
	{
		delete dictionaries;
		delete expries;
	}
};
class redisServer {
public:
	redisDb* db;
	int dbnum;
	redisServer(int num=16)
	{
		dbnum = num;
		db = new redisDb[dbnum];
	}
	~redisServer()
	{
		delete[]db;
	}
};