
# memcache的使用入门C++代码 - 高科的专栏 - CSDN博客

2019年01月24日 17:32:06[高科](https://me.csdn.net/pbymw8iwm)阅读数：29


下载源码编译，memcached就是生成的主程序，启动可指定端口，memcached作为server端，依然是我们熟悉的cs模式，使用两个client一个setkey，一个getkey一百万个做测试。
`./memcached -d -m 300 -p 11211 -u root -c 4096``#include "../libmemcached-1.0.7/libmemcached/memcached.h"
#include<iostream>
#include<string>
#include<time.h>
using std::string;
using std::cout;
using std::endl;
 
class MemCachedClient
{
    public:
        ~MemCachedClient()
        {
            memcached_free(memc);
        };
 
        MemCachedClient()
        {
            memcached_return rc;
            memcached_server_st *server = NULL;
 
            memc = memcached_create(NULL);
 
            server =memcached_server_list_append(server, "127.0.0.1", 11211, &rc);
 
            rc=memcached_server_push(memc,server);
 
            if (MEMCACHED_SUCCESS != rc)
            {
                cout <<"memcached_server_push failed! rc: " << rc << endl;
            }
 
            memcached_server_list_free(server);
        };
 
 
        int Insert(const char* key, const char* value,time_t expiration = 3)
        {
            if (NULL == key || NULL == value)
            {
                return -1;
            }
 
            uint32_t flags = 0;
 
            memcached_return rc;
 
            rc = memcached_set(memc, key, strlen(key),value, strlen(value)+1, expiration, flags);
 
            if (MEMCACHED_SUCCESS == rc)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        };
 
 
        string Get(const char* key)
        {
            if (NULL == key)
            {
                return "";
            }
 
            uint32_t flags = 0;
 
            memcached_return rc;
 
            size_t value_length;
            char* value = memcached_get(memc, key, strlen(key), &value_length, &flags, &rc);
 
            if(rc == MEMCACHED_SUCCESS)
            {
                return value;
            }
 
            return "";
        };
 
    private:
        memcached_st* memc;
};
 
#include<iostream>
using std::cout;
using std::endl;
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
 
int main()
{
    MemCachedClient mc;
    clock_t begin_tick_counts = clock();
    long object_nums = 1000000;
    char buff[32];
    for (int i=0; i<=object_nums; ++i)
    {
        sprintf(buff, "key-%d",i);
        mc.Insert(buff,buff, 60);
        //printf("%s\n", mc.Get(buff).c_str());
    }
    clock_t end_tick_counts = clock();
    printf("set %ld values costs %f ticks\n", object_nums, (double)(end_tick_counts - begin_tick_counts)/CLOCKS_PER_SEC);
    return 1;
}`
```python
TARGET = test_memcached_getkey
SOURCE = test_memcached_getkey.cpp
CFLAG  = -g3 -O0 -Wl,-rpath=./ -lmemcached
 
TARGET:
	g++ $(SOURCE) -o $(TARGET) $(CFLAG)
test_memcached_setkey:
	g++ test_memcached_setkey.cpp -o test_memcached_setkey $(CFLAG)
.phony:clean
clean:
	rm $(TARGET)
	rm test_memcached_setkey
```


