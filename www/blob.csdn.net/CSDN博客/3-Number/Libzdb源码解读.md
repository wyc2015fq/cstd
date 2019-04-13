
# Libzdb源码解读 - 3-Number - CSDN博客


置顶2014年10月11日 21:25:35[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：953



```python
<pre name="code" class="cpp" style="text-align: center; font-size: 24px; font-weight: bold;">Libzdb源码解读
```

# 1  Libzdb简介
一个非常小，简单的数据库连接池库。
## 1.1  Libzdb特点
1） 线程安全。
2） 连接不同的数据库系统。
3） 零运行时配置，连接使用URL。
4） 目前支持的数据库MySQL, PostgreSQL, SQLite and Oracle。
5） 可以不断进行扩展。
## 1.2  支持平台及开发语言
### 1.2.1  OS
支持ios、Linux、FreeBSD、Solaris、OpenBSD和其他 POSIX 系统。
### 1.2.2  语言
C、C++、Object C
# 2  Libzdb技术框架
## 2.1  数据库连接池
连接池基本的思想是在系统初始化的时候，将数据库连接作为对象存储在内存中，当用户需要访问数据库时，并非建立一个新的连接，而是从连接池中取出一个已建立的空闲连接对象。使用完毕后，用户也并非将连接关闭，而是将连接放回连接池中，以供下一个请求访问使用。而连接的建立、断开都由连接池自身来管理。同时，还可以通过设置连接池的参数来控制连接池中的初始连接数、连接的上下限数以及每个连接的最大使用次数、最大空闲时间等等。也可以通过其自身的管理机制来监视数据库连接的数量、使用情况等。
## 2.2  线程安全
如果你的代码所在的进程中有多个线程在同时运行，而这些线程可能会同时运行这段代码。如果每次运行结果和单线程运行的结果是一样的，而且其他的变量的值也和预期的是一样的，就是线程安全的。
或者说:一个类或者程序所提供的接口对于线程来说是原子操作或者多个线程之间的切换不会导致该接口的执行结果存在二义性,也就是说我们不用考虑同步的问题。
线程安全问题都是由全局变量及静态变量引起的。
若每个线程中对全局变量、静态变量只有读操作，而无写操作，一般来说，这个全局变量是线程安全的；若有多个线程同时执行写操作，一般都需要考虑线程同步，否则的话就可能影响线程安全。
## 2.3  设计模式之代理模式
### 2.3.1简介
为其他对象提供一种代理以控制对这个对象（过程化中，可以考虑一下特殊的接口，如数据库API接口）的访问。在某些情况下，一个对象不适合或者不能直接引用另一个对象，而代理对象可以在客户端和目标对象之间起到中介的作用。
### 2.3.2  优点
1）职责清晰
真实的角色就是实现实际的业务逻辑，不用关心其他非本职责的事务，通过后期的代理完成一件完成事务，附带的结果就是编程简洁清晰。
2）代理对象可以在客户端和目标对象之间起到中介的作用，这样起到了的作用和保护了目标对象的作用。
3）高扩展性，假如引入其他数据库。
### 2.3.3  代理模式实例
在公司上网的时候，并不是每一台PC都有上网的权限的。比如说，现在有PC1、PC2、PC3，但是只有PC1有上网权限，但是PC2、PC3也想上网，此时应该怎么办呢？
此时，我们需要做的就是在PC1上开启代理软件，同时把PC2、PC3的IE代理指向PC1即可。这个时候，如果PC2或者PC3想上网，那么报文会先指向PC1，然后PC1把Internet传回的报文再发给PC2或者PC3。这样一个代理的过程就完成了整个的上网过程。

```python
#include <stdio.h>
#include <assert.h>
#include <sdlib.h>
typedef struct _PcClient
{
     void (*request)();
}PcClient;
void ftp_request()  
{
     printf("request from ftp!\n");  
}  
void http_request()  
{  
     printf("request from http!\n");  
}  
void smtp_request()  
{  
     printf("request from smtp!\n");  
}  
typedef struct _Proxy  
{  
     PcClient* pClient;  
}Proxy;  
void process(Proxy* pProxy)  
{  
     assert(NULL != pProxy);  
     pProxy->pClient->request();  
}  
/* test programe*/
int main()
{
    PcClient *pc = (PcClient *)malloc(sizeof(PcClient));
    if(pc == NULL)
    {
          printf("malloc fail\n");
          return 0;      
    }
    Proxy *pProxy = (Proxy *)malloc(sizeof(Proxy));
    if(pProxy == NULL)
    {
          printf("malloc fail\n");
          return 0;      
    }
    pc->request = &http_request;
    pProxy->pClient = pc;
    process(pProxy); 
    return 0;
}
```
## 2.4  各类数据库开发API
### 2.4.1  关系数据库
MYSQL
ORALCE    OCI
SQLITE
## 2.5  模块化设计
要编写复杂软件又不至于一败涂地的唯一方法，就是用定义清晰的接口把若干简单模块组合起来，如此一来，多数问题只会出现在局部，那么还有希望对局部进行改进或优化，而不至于牵动全身。（参考Unix编程艺术）
# 3  Libzdb使用场景
要使系统兼容各类数据库、多进程等。
# 4  Libzdb源码结构
![](https://img-blog.csdn.net/20141011213631421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 4.1  各模块简单分析
### 4.1.1  Exception
异常模块，主要是为整个系统提供异常处理接口。处理异常包括数据库异常和内存异常等其他非法操作。捕获异常的好处是，不使程序由于异常而非法退出。
### 4.1.2  System
一些时间处理、内存操作、系统操作等接口。方便其他模块调用。
### 4.1.3  Net
统一资源定位符（UniformResource Locator，缩写为URL）是对可以从互联网上得到的资源的位置和访问方法的一种简洁的表示，是互联网上标准资源的地址。互联网上的每个文件都有一个唯一的URL，它包含的信息指出文件的位置以及浏览器应该怎么处理它。
整个数据库的初始化和代理对象的初始化，都是通过这个URL来进行处理。
示例：
mysql://localhost:3306/test?user=root&password=swordfish
这个模块式针对这个URL地址进行处理的函数接口。
### 4.1.4  Util
这个模块主要负责一些工具的封装，封装了字符串的处理函数，还封装了面向对象中的向量（vector）、StringBuffer等工具，方面整个模块的处理。
### 4.1.5  Db
这个模块主要包括实现连接代理模式、结果集代理模式、SQL语句代理模式和数据库连接池、各类数据库连接的真实实现等。这个模块是整个程序的核心。
主题代码实现细节，各位可以网上下载，自行阅读。
# 5、结束语
这是第一次写博文，写得错误的还望各位见谅！希望以上能对想了解这方便的人有所帮助。了解更多的内容（http://www.tildeslash.com/libzdb/\#），后期也会补充博文。

