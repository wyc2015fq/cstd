# C++操作Redis数据库 - fanyun的博客 - CSDN博客
2017年05月31日 21:21:50[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：516
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
用C++来操作redis数据库。通过hiredis.h接口来实现，目前只能在Linux环境使用。
hiredis.h的下载地址为：https://github.com/redis/hiredis
主要包括如下四个方法
1. redisContext* redisConnect(const char*ip, int port)
该函数用来连接redis数据库，两个参数分别是redis数据库的ip和端口，端口号一般为6379。类似的还提供了一个函数，供连接超时限定，即
redisContext* redisConnectWithTimeout(constchar *ip, int port, timeval tv)。
2. void *redisCommand(redisContext *c,const char *format...)
该函数用于执行redis数据库中的命令，第一个参数为连接数据库返回的redisContext，剩下的参数为变参，如同C语言中的prinf()函数。此函数的返回值为void*，但是一般会强制转换为redisReply类型，以便做进一步的处理。
3. void freeReplyObject(void *reply)
释放redisCommand执行后返回的的redisReply所占用的内存。
4. void redisFree(redisContext *c)
释放redisConnect()所产生的连接。
接下来就是就让本人来教大家如何安装hiredis吧！
首先上网站下载hiredis.tar.gz包，解压后发现里面有一个Makefile文件，然后执行make进行编译，得到
接下来把libhiredis.so放到/usr/local/lib/中，把hiredis.h放到/usr/local/inlcude/hiredis/中。
或者直接用命令make install配置。如下图
接下来在程序中就可以直接用了。在程序中包含#include <hiredis/hiredis.h>即可。
为了操作方便，一般情况下我们需要写一个头文件类，这个类的方法根据自己项目需要适当添加。如下
代码：redis.h
#ifndef _REDIS_H_
#define _REDIS_H_
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <hiredis/hiredis.h>
class Redis
{
public:
   Redis(){}
   ~Redis()
 {
       this->_connect = NULL;
 this->_reply = NULL;        
 }
 boolconnect(std::string host, int port)
 {
   this->_connect = redisConnect(host.c_str(), port);
 if(this->_connect != NULL && this->_connect->err)
  {
     printf("connect error: %s\n", this->_connect->errstr);
  return 0;
  }
 return 1;
 }
   std::string get(std::string key)
 {
 this->_reply = (redisReply*)redisCommand(this->_connect, "GET%s", key.c_str());
 std::string str = this->_reply->str;
 freeReplyObject(this->_reply);
 return str;
 }
 voidset(std::string key, std::string value)
 {
       redisCommand(this->_connect, "SET %s %s", key.c_str(),value.c_str());
 }
private:
   redisContext* _connect;
 redisReply* _reply;
};
#endif //_REDIS_H_
redis.cpp
#include "redis.h"
int main()
{
 Redis *r = new Redis();
 if(!r->connect("192.168.13.128",6379))
 {
 printf("connect error!\n");
 return 0;
 }
 r->set("name","Mayuyu");
 printf("Get the name is %s\n",r->get("name").c_str());
 delete r;
 return 0;
}
Makefile文件
redis: redis.cpp redis.h
 g++redis.cpp -o redis -L/usr/local/lib/ -lhiredis
clean:
 rmredis.o redis
注意在g++和rm之前都是一个tab键。
在编译的时候需要加参数，假设文件为redis.cpp，那么编译命令如下
g++ redis.cpp -o redis -L/usr/local/lib/-lhiredis
在执行的时候如果出现动态库无法加载，那么需要进行如下配置
在/etc/ld.so.conf.d/目录下新建文件usr-libs.conf，内容是：/usr/local/lib
如下图所示
然后使用命令/sbin/ldconfig更新一下配置即可。
