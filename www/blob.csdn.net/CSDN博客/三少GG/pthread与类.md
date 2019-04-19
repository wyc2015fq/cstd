# pthread与类 - 三少GG - CSDN博客
2013年10月31日 23:20:41[三少GG](https://me.csdn.net/scut1135)阅读数：1009
一。 [向 pthread 传递多个参数的方法](http://blog.csdn.net/easecom/article/details/4508292)
**1：传Struct结构的指针**
typedef union {
   size_t arg_cnt;
   any_possible_arg_types;
} arg_type;
arg_type args[ARG_NUM + 1];
args[0].arg_cnt = ARG_NUM;
args[1].xxx = ...;
pthread_create (..., ..., thread_function, &args[0]);
*******************************
涉及多参数传递给线程的，都需要使用结构体将参数封装后，将结构体指针传给线程
定义一个结构体
struct mypara
{
       var para1;//参数1
       var para2;//参数2
}
将这个结构体指针，作为void *形参的实际参数传递
struct mypara pstru;
pthread_create(&ntid, NULL, thr_fn,& (pstru));
函数中需要定义一个mypara类型的结构指针来引用这个参数 
void *thr_fn(void *arg)
{
       mypara *pstru;
       pstru = (* struct mypara) arg;
       pstru->para1;//参数1
       pstru->para2;//参数2
}
**2：传void*[]**
你创建一个void*[]，里面保存指向你的所有参数的指针，然后把这个void*[]传递给pthread_create就可以了么……   
多少个参数还不是随便你？给你个例子   
```cpp
#include <pthread.h>   
void*   route(void*   args)   
{   
      int*   iptr   =   ((void**)args)[0];   
      float*   fptr   =   ((void**)args)[1];   
      char*   str   =   ((void**)args)[2];   
      printf("integer:   %d/nfloat:   %f/nstring:   %s/n",   *iptr,   *fptr,   str);   
      return   0;   
}   
    
int main(void)   
{   
      pthread_t   thr_id;   
      int   ival   =   1;   
      float   fval   =   10.0F;   
      char   buf[]   =   "func";   
      void*   arg[3]={&ival,   &fval,   buf};   
      pthread_create(&thr_id,   NULL,   route,   arg);   
      sleep(1);   
}
```
二。 类成员函数作为pthread_create函数参数
缺点： 无法传递其余参数。需要设为类的成员对象。
近日需要将线程池封装成C++类，类名为Threadpool。在类的成员函数exec_task中调用pthread_create去启动线程执行例程thread_rounter。编译之后报错如下：
spfs_threadpool.cpp: In member function ‘int Threadpool::exec_task(task*)’:
spfs_threadpool.cpp:174: error: argument of type ‘void* (Threadpool::)(void*)’ does not match ‘void* (*)(void*)’
    出现类型不匹配的问题。因为pthread_create需要的参数类型为void* (*)(void*)，而thread_rounter作为类的成员函数时其类型是void* (Threadpool::)(void*)的成员函数指针。我们知道类的成员函数在经过编译器处理之后，会变成带有this指针参数的全局函数，所以类型注定是不会匹配的。但是如果将thread_rounter声明为static类型，那么编译器会将static形式的函数，转换成不带this指针的全局函数，所以其类型可以与pthread_create需要的参数类型相匹配。但是类的静态成员函数无法访问类的非静态成员，不过这可以通过传递this指针解决这个问题。
综上，我的这个问题可以这个样子解决。
出问题之前的代码：
void *thread_rounter(void *)//线程执行函数
{
   //直接访问类的成员
}
exec_task函数中调用：
pthread_create(&tid,NULL,thread_rounter,NULL);//启动线程执行例程
修复这个问题的代码：
static void *thread_rounter(void *tmp)/线程执行函数
{
  Threadpool *p=(Threadpool *)tmp;
    //通过p指针间接访问类的非静态成员
}
exec_task函数中调用：
pthread_create(&tid,NULL,thread_rounter,(void *)this);//启动线程执行例程
----------------------------------------------------------------------------------------------------------------------
在网上搜索一下还有其他的解决方案，摘录如下，为了以示尊重标明文章来源，感谢原文作者。
方案二：
将线程启动函数声明为模板函数。
摘录自：[http://hi.baidu.com/angelevil2006/item/e1806ec30574ff11515058d1](http://hi.baidu.com/angelevil2006/item/e1806ec30574ff11515058d1)
“
  最近做C++项目时需要在一个类里面启动线程，使用了pthread_create,但是把类成员函数作为参数传递，编译时却报出参数类型 不匹配错误，上网查了一下，原因如下：
    在 C 中，这个函数使用很简单，只要定义一个参数和返回值均为 void * 类型的函数，使用函数名字作为参数即可。就算不完全符合，可以使用 (void *(*)(void *)) 将其强制转换为符合类型检查规格的函数指针。但是，类的非静态成员函数隐含 this 指针作为第一个参数，假设类为MyClass,,则其参数类型为(void*)(MyClass::)(void*)，所以参数完全不可能转化为 void * 类型，而 C++ 的类型检查要比 C 严格许多。
”
下面是另外一种方法，将C++普通成员函数作为线程函数：
代码如下:
#include <iostream>
#include <stdio.h>
#include <pthread.h>
using namespace std;
class TMyClass{
    int m_nCount;
    int m_nId;
public:
    TMyClass(int nId,int nCount)
        :m_nId(nId),m_nCount(nCount)
    {
    }
    void * ThreadProc(void *)            // 类成员方法
    {
        //for(int i=0; i<m_nCount; i++)       // 根据m_nCount成员打印一排数字
        //{
            //printf("Class%d : %d\n",m_nId,i);
        //}
        cout << "hello, world" << endl;
    }
};
int main(int argc, char* argv[])
{
    union {                                
//联合类，用于转换类成员方法指针到普通函数指针（试过编译器不允许在这两种函数之间强制转换），不知道有没更好的方法。
        void* (*ThreadProc)(void *);
        void* (TMyClass::*MemberProc)(void *);
    } Proc;                                // 尽管联合里的两种函数类型现在看起来有很大不同，但它们的最终形式是相同的。
    Proc.MemberProc = &TMyClass::ThreadProc;   // 转换，Proc.ThreadProc就是对应的普通函数指针了
    pthread_t tid;
    int err = pthread_create(&tid, NULL, Proc.ThreadProc, NULL);
    if (err) 
        cout << "thread create error!\n";
    return 0;
}
实际使用如下：
```cpp
union { 
		void (*MemberThread)(void *);
		void (QueryHandler::*MemberProc)(void *);
	}Proc;
	Proc.MemberProc = &QueryHandler::GetTableDataThreadTask;
	pthread_mutex_init(&lock, NULL);
	assert((pool = threadpool_create(THREAD, QUEUE, 0)) != NULL);
	INFO_LOG("Pool started with %d threads and queue size of %d\n", THREAD, QUEUE);
	for(size_t i = 0; i < term.attrIdList.size(); i++) {
		void* arg[4]={&ht, &term, &term.attrIdList[i], &dimData};
		assert(threadpool_add(pool, Proc.MemberThread, arg, 0) == 0);
	}
```
