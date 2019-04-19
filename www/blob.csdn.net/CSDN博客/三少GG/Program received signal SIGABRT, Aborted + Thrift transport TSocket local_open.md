# Program received signal SIGABRT, Aborted + Thrift transport::TSocket::local_open - 三少GG - CSDN博客
2013年11月09日 22:52:50[三少GG](https://me.csdn.net/scut1135)阅读数：1850

### [thrift中TNonblockingServer的简单用法](http://blog.csdn.net/jianbinhe1012/article/details/7726738)[http://blog.csdn.net/jianbinhe1012/article/details/7726738](http://blog.csdn.net/jianbinhe1012/article/details/7726738)
分类： [c++](http://blog.csdn.net/jianbinhe1012/article/category/1173657)[thrift](http://blog.csdn.net/jianbinhe1012/article/category/1182747)
    最近在项目中需要把客户端的一些信息发送到服务器上，听起来是个很简单的需求，但是实际考虑下，觉得如果自己手工实现，工作量也不小，而且尽是些繁琐且无聊的事情，遂考虑用现成的库来实现。**对比了protocol buffer与thrift后，本着偷懒到底的原则，选择了thrift，因为thrift本身提供了RPC框架，而protocol buffer仅是个序列化的库而已。**
   首先是编译thrift，这里参考官方说明，需要先装boost库，但是如果要使用nonblocking server的话，还要再把libevent库也装上。
    thrift提供了三种服务模型，分别是TSimpleServer, TThreadPoolServer和TNonblockingServer，除去第一个一般仅做测试用，后两个都可以在实际生产中拿来用。在客户端不多的情况下，可以选用TThreadPoolServer，**但是要注意TThreadPoolServer的客户端只要不从服务器上断开连接，就会一直占据服务器的一个线程，**当服务器线程池所有线程都在被使用时，新到来的客户端将排在队列里等待，直到有客户端断开连接，使服务器端线程池出现空闲线程方可继续被提供服务，所以使用这种模型时**，一定要注意客户端不使用时不要长时间连接服务器**，如果确实有这种需求，请使用TNonblockingServer。
    说实话，单纯从代码量上来讲，使用Nonblocking server并不比ThreadPool server多了多少，谁让代码都是由thrift程序生成的，用户只需填上实际处理的代码即可。
    下面用一个简单的例子说明
    clientInfo.thrift
**[plain]**[view
 plain](http://blog.csdn.net/jianbinhe1012/article/details/7726738#)[copy](http://blog.csdn.net/jianbinhe1012/article/details/7726738#)
- namespace cpp vnmp  
- 
- enum ClientType {  
-     DOM_MANAGER,  
-     DOM_SERVICE  
- }  
- 
- enum RegistResult {  
-     SUCCESS,  
-     NAME_EXISTED,  
-     INVALIE_PARA,  
- }  
- 
- struct ClientInfo {  
-     1: string name,  
-     2: string realIP,  
-     3: string vpnIP,  
-     4: ClientType type,  
-     5: optional string description,  
- }  
- 
- 
- service Regist {  
-     RegistResult registClient(1:ClientInfo clientInfo),  
-     bool heartbeat(1:string name, 2:ClientType type)  
- }  
对这个文件做一个简单的说明，client需要把自己的信息ClientInfo发送到server上注册，调用registClient方法，heartbeat方法是用来做心跳的。
执行 thrift -r --gen cpp clientInfo.thrift
如果没有语法错误的话，在gen-cpp目录下会生成  
clientInfo_constants.h clientInfo_constants.cpp 
clientInfo_types.h clientInfo_types.cpp
Regist_server.skeleton.cpp
其中的skeleton文件包含了一个简单的TSimpleServer实现，是可以直接编译使用的，这个文件也就是我们要修改的文件，建议另外建一个main文件，并将其中内容拷过来，其他几个强烈建议不要做修改，一来没需要，二来如果做了修改，下次执行thrift文件时，也会被新生成的文件覆盖，这也是我前面建议另外建一个main文件还不是直接修改skeketon文件的原因。
下面是main文件的主要内容，略去了头文件的包含和命名空间的使用等等，这里假定读者已有了一定的boost基础。
**[cpp]**[view
 plain](http://blog.csdn.net/jianbinhe1012/article/details/7726738#)[copy](http://blog.csdn.net/jianbinhe1012/article/details/7726738#)
- class RegistHandler : virtualpublic RegistIf {  
- public:  
- 
-     RegistHandler() {  
- // Your initialization goes here
-     }  
- 
-     RegistResult::type registClient(const ClientInfo& clientInfo) {  
- // Your implementation goes here
-         printf("registClient\n");  
-     }  
- 
- void heartbeat(const std::string& name, const ClientType::type type) {  
- // Your implementation goes here
-         printf("heartbeat\n");  
-     }  
- 
- };  
- 
- int main(int argc, char **argv) {  
- int port = 9090;  
-     shared_ptr<RegistHandler> handler(new RegistHandler());  
-     shared_ptr<TProcessor> processor(new RegistProcessor(handler));  
-     shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());  
-     shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(15);  
-     shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory > (new PosixThreadFactory());  
-     threadManager->threadFactory(threadFactory);  
-     threadManager->start();  
-     TNonblockingServer server(processor, protocolFactory, port, threadManager);  
-     server.serve();  
- return 0;  
- }  
regist service的实际处理方法写在registHandler对应的方法里。主要是main方法做个简单说明：
这里使用了thrift库自带的ThreadManager，建立了一个拥有15个线程的线程池，也就是说这个NonblockingServer拥有15个工作线程。
除了shared_ptr来自boost，其他均是thrift自带，命名空间是apache::thrift 
client端
**[cpp]**[view
 plain](http://blog.csdn.net/jianbinhe1012/article/details/7726738#)[copy](http://blog.csdn.net/jianbinhe1012/article/details/7726738#)
- int main(int argc, char** argv) {  
-     boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));  
-     boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));  
-     boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));  
- 
-     RegistClient client(protocol);  
- 
-     transport->open();  
- //insert your code here
- //      ....
-     transport->close();  
- return 0;  
- }  
注意对于nonblocking server，client端的TTransport只能选用TFramedTransport；如果通信过程中出现异常，会抛出异常，可以用try catch捕获并做处理。

**thrift wiki**
[http://wiki.apache.org/thrift/ThriftUsageC%2B%2B](http://wiki.apache.org/thrift/ThriftUsageC%2B%2B)
[thrift shows CLOSE_WAIL error](http://blog.csdn.net/z_l_l_m/article/details/10859913)
[http://blog.csdn.net/heavendai/article/details/8614941](http://blog.csdn.net/heavendai/article/details/8614941)
[http://people.apache.org/~thejas/thrift-0.9/javadoc/org/apache/thrift/transport/TServerSocket.html#acceptImpl()](http://people.apache.org/~thejas/thrift-0.9/javadoc/org/apache/thrift/transport/TServerSocket.html#acceptImpl())
Error:
# [thrift之TTransport层的堵塞的套接字I/O传输类TSocket](http://www.cnblogs.com/brucewoo/p/3226858.html)
　上面这个函数代码确实比较长，不过还好都是比较简单的代码实现，没有什么很绕的代码，整个流程也很清晰，在代码中也有比较详细的注释了。下面继续看通用socket打开函数local_open（它也真正的执行打开功能也是调用上面刚才介绍的那个函数，只是传递了具体的地址信息）：
**整个local_open函数就是根据主机名得到所有的网卡信息，然后依次尝试打开，直到打开一个为止就退出循环，如果所有都不成功就抛出一个异常信息。**

Thriftclient exceptions in Python
[https://groups.google.com/forum/#!msg/hypertable-dev/_vLymV5YENk/4Y3imVoY8PMJ](https://groups.google.com/forum/#!msg/hypertable-dev/_vLymV5YENk/4Y3imVoY8PMJ)
Hi,
Actually the failed open_scanner/next_cells calls are being called from newly spawned threads. I wrote a sample program to reproduce this. See the program and its output.
Hi Sreejith,
**Instead of sharing the ThriftClient across all the threads, try creating one ThriftClient per thread**(ie 'push "client =ThriftClient('localhost', 38080)"
 '  into the 'Get' method in your program),
-Sanjit
Thanks for the reply Sanjit. That seems to have solved the issue.
Hi Sanjit,
So, the only way to use ThriftClient in a multithreaded application is to instantiate the client in all the threads. Any plans to make it thread-safe ? Just curious to know why it is behaving like this.
--
Regards,
Sreejith K
You're right, i**t looks like the thrift client isn't threadsafe.** I'll take a look
 at whether theres a thrift knob to generate threadsafe client code. There shouldn't be any other reason that the Hypertable thrift client shouldn't be thread safe. 
Looking at this [post](http://mail-archives.apache.org/mod_mbox/incubator-thrift-user/200911.mbox/%3C6cf16f130911230449h1ab500e7y1c5ea787548f9da7@mail.gmail.com%3E) it
 looks like **Thrift expects thread safety to be implemented in the application layer.**So if you protect the client calls with a mutex that would work too and shouldn't have any more of a performance hit than if the locks were pushed into the generated thrift code.
*******************************************************************************************************************************************************************
## [stack] Program received signal SIGABRT, Aborted. 0x00130416 in __kernel_vsyscall ()
为了写一个字符串替换的程序，先测试一些想法。
结果出现了一个不好找的而且低端错误，但是很值得琢磨。总结起来如下：
char *str = (char*)malloc(sizeof(char)*temp);
str =  "jeijfiowjfowjifwiejf";
free(str);
//以上三行程序在linux ubuntu环境下运行时产生段错误，gdb下运行至free(str)时出现 [stack] Program received signal SIGABRT, Aborted.
//以下四行则正确
char  *str=(char*)malloc(sizeof(char)*temp);                                                                                            
char *ss = "jeijfiowjfowjifwiejf";
strcpy(str,ss);
free(str);
出错原因在于释放了一个静态空间。
char *str = (char*)malloc(sizeof(char)*temp);           // malloc新空间，返回 char * 指针，str保存新空间的这个指针。
str =  "jeijfiowjfowjifwiejf";                          // str更新地址，指向保存："jeijdlfjslafjlsajfsldafjlsdfjsladkjf" 的地方，这是静态空间。
free(str);                                              // 此时free 静态空间，必然错。
这种错误类似于
int global_var;
int *p;
int main()
{
       p = &global_var;
       free(p);
}
在此感谢大牛周有松，此人一眼就看出问题所在。并且指出我使用strcpy函数也会存在隐患，比如temp过小如为4时，strcpy(str,ss）将导致缓冲区溢出。他推荐我使用strncpy。良师益友呀。
以下是完成的验证程序。
  1 #include <stdio.h>                                                                                                                       
  2 #include <string.h>
  3 #include <stdlib.h>
  4 
  5 int main()
  6 {
  7     int temp = 22;
  8     int i;
  9     char *tt;
 10     char *str = (char *)malloc(sizeof(char)*temp);
 11     str = "jeijfiowjfowjifwiejf";
 12 
 13     printf("\n");
 14     for (i = 0; i < temp-2; i++)
 15     {
 16         printf("%c",str[i]);
 17     }
 18     printf("\n");
 19 
 20     printf("str length is : %d \n",strlen(str));
 21     printf("%s\n",str);
 22 
 23     tt = (char *)malloc(sizeof(char)*temp);
 24 /*  if (tt == NULL)
 25     {
 26             printf("Not enough memory \n");
 27             return -1;
 28     }
 29     */
 30     tt = "jfwjfeioj['pk'vnie-[";
 31     printf("tt length is : %d\n",strlen(tt));
 32 
 33     free(str);                                                  //此处出现[stack]  Program received signal SIGABRT, Aborted.错误。                    
 34     free(tt);
 35 
 36     return 0;
 37 }
