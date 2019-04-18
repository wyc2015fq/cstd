# ACE的内存管理 - 深之JohnChen的专栏 - CSDN博客

2009年12月12日 18:01:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2546标签：[os																[server																[unix																[null																[服务器																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=os&t=blog)
个人分类：[ACE网络编程](https://blog.csdn.net/byxdaz/article/category/628882)


ACE构架含有一组非常丰富的内存管理类。这些类使得你能够很容易和有效地管理动态内存（从堆中申请的内存）和共享内存（在进程间共享的内存）。你可以**使用**若干不同的方案来管理内存。你需要决定何种方案最适合你正在**开发**的应用，然后采用恰当的ACE类来实现此方案。

ACE含有两组不同的类用于内存管理。

　　第一组是那些基于ACE_Allocator的类。这组类使用动态绑定和策略模式来提供灵活性和**可扩展**性。它们只能用于局部的动态内存分配。

　　第二组类基于ACE_Malloc模板类。这组类使用C++模板和*外部多态性*（External Polymorphism）来为内存分配机制提供灵活性。在这组类中的类不仅包括了用于局部动态内存管理的类，也包括了管理进程间共享内存的类。这些共享内存类使用底层OS（OS）共享内存接口。

　　为什么使用一组类而不是另外一组呢？这是由在性能和灵活性之间所作的权衡决定的。因为实际的分配器对象可以在运行时改变，ACE_Allocator类更为灵活。这是通过动态绑定（这在C++里需要使用虚函数）来完成的，因此，这样的灵活性并非不需要代价。虚函数调用带来的间接性使得这一方案成了更为昂贵的选择。

　　另一方面，ACE_Malloc类有着更好的性能。在编译时，malloc类通过它将要使用的内存分配器进行配置。这样的编译时配置被称为“外部多态性”。基于ACE_Malloc的分配器不能在运行时进行配置。尽管ACE_Malloc效率更高，它不像ACE_Allocator那样灵活。

      具体介绍请参考《ACE程序员教程》内容。

[http://www.acejoy.com/space/html/50/n-50.html](http://www.acejoy.com/space/html/50/n-50.html)

这里给出两个实例的代码，虽然书上有代码，有些不是太全，少些头文件之类。

Allocator实例代码：

#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h" 
#include "ace/malloc.h"
#include  "ace/Malloc_T.h"  
#include "ace/log_msg.h"
#include "ace/os.h"
#ifdef _DEBUG
#pragma comment(lib,"aced.lib")
#else
#pragma comment(lib,"ace.lib")
#endif

typedef char MEMORY_BLOCK[1024];
typedef ACE_Cached_Allocator <MEMORY_BLOCK,ACE_SYNCH_MUTEX>  Allocator;
class MessageManager
{
public:
 MessageManager(int n_blocks):allocator_(n_blocks),message_count_(0)
 {
  mesg_array = new char *[n_blocks];
 }

 void allocate_msg(const char *msg)
 {
  mesg_array[message_count_] = (char *)allocator_.malloc(ACE_OS::strlen(msg)+1);
  ACE_OS::strcpy(mesg_array[message_count_],msg);
  message_count_++;
 }

 void free_all_msg()
 {
  for(int i=0;i<message_count_;i++)
  {
   allocator_.free(mesg_array[i]);
  }
  message_count_=0;
 }

 void display_all_msg()
 {
  for(int i=0;i<message_count_;i++)
  {
   ACE_OS::printf("%s/n",mesg_array[i]);
  }
 }
protected:
private:
 char **mesg_array;
 Allocator allocator_;
 int message_count_;
};

int main(int argc,char *argv[])
{
 if(argc<2)
 {
  ACE_DEBUG((LM_DEBUG,"usage:%s <number of blocks>/n",argv[0]));
  exit(1);
 }
 int n_blocks = ACE_OS::atoi(argv[1]);
 MessageManager mm(n_blocks);
 while (1) 
 {
  ACE_DEBUG((LM_DEBUG,"/n/n/nAllocating Messages/n"));
  char message[1024];
  for(int i=0;i<n_blocks;i++)
  {
   memset(message,0,sizeof(message));
   ACE_OS::sprintf(message,"Message %d:Hi There",i);
   mm.allocate_msg(message);
  }
  ACE_DEBUG((LM_DEBUG,"Displaying the messages/n"));
  ACE_OS::sleep(2);
  mm.display_all_msg();
  ACE_DEBUG((LM_DEBUG,"Releasing message/n"));
  ACE_OS::sleep(2);
  mm.free_all_msg();
 }
 return 0;
}

Malloc实例代码：

#include "ace/synch_traits.h"
#include "ace/thread_mutex.h"
#include "ace/Null_Mutex.h"
//#include "ace/shared_memory_mm.h"
#include <ace/mmap_Memory_Pool.h>
#include "ace/malloc.h"
#include "ace/malloc_T.h"
#include "ace/os.h"
#pragma comment(lib,"aced.lib")
#define DATA_SIZE 100
#define MESSAGE1 "Hiya over there client process"
#define MESSAGE2 "Did you hear me the first time?"
LPCTSTR poolname = "My_Pool";
//typedef ACE_Malloc<ACE_Shared_Memory_Pool,ACE_Null_Mutex> Malloc_Allocator; //支持UNIX环境,在NT下无法编译通过。如果想在NT下编译成功使用下面的内存池类型
typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> Malloc_Allocator;
//服务器进程
static void server(void)
{
 Malloc_Allocator shm_allocator(poolname);
 char *Message1 = (char *)shm_allocator.malloc(strlen(MESSAGE1));
 ACE_OS::strcpy(Message1,MESSAGE1);
 shm_allocator.bind("FirstMessage",Message1);
 ACE_DEBUG((LM_DEBUG,"<<%s/n",Message1));

 char *Message2 = (char *)shm_allocator.malloc(strlen(MESSAGE2));
 ACE_OS::strcpy(Message2,MESSAGE2);
 shm_allocator.bind("SecondMessage",Message2);
 ACE_DEBUG((LM_DEBUG,"<<%s/n",Message2));
 ACE_DEBUG((LM_DEBUG,"Server done writing ..going to sleep zzz../n/n/n"));
 ACE_OS::sleep(5);
    shm_allocator.remove();
}

//客户端进程
static void client(void)
{
 Malloc_Allocator shm_allocator(poolname);
 void *Message1;
 if(shm_allocator.find("FirstMessage",Message1)==-1)
 {
  ACE_ERROR((LM_ERROR,"client: problem cant find data that server has sent/n"));
  ACE_OS::exit(1);
 }
 ACE_OS::printf(">>%s/n",(char*)Message1);
 ACE_OS::fflush(stdout);

 void *Message2;
 if(shm_allocator.find("SecondMessage",Message2)==-1)
 {
  ACE_ERROR((LM_ERROR,"client: problem cant find data that server has sent/n"));
  ACE_OS::sleep(2);
  ACE_OS::exit(1);
 }
 ACE_OS::printf(">>%s/n",(char*)Message2);
 ACE_OS::fflush(stdout);
 ACE_DEBUG((LM_DEBUG,"client done reading! BYE NOW/n"));
 ACE_OS::fflush(stdout);
 ACE_OS::sleep(2);
}

int main(int argc,char *argv[])
{
 //NT环境无法运行，fork调用失败（fork需要支持UNIX环境）。
 switch(ACE_OS::fork()) 
 {
 case -1:
  ACE_ERROR_RETURN((LM_ERROR,"%p/n","fork"),1);
  break;
 case 0:
  ACE_OS::sleep(1);
  client();
  break;
 default:
  server();
  break;
 }
 //NT 环境下测试
 //main函数放server()函数生成一个版本server.exe，main函数放client()函数生成另一个版本client.exe
 //先运行server.exe，接着运行client.exe，就可以看到共享内存数据的效果。（注意：设置server.exe等待时间比client.exe长）

 return 0;
}


