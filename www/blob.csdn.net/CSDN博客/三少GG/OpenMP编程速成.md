# OpenMP编程速成 - 三少GG - CSDN博客
2013年02月27日 12:39:48[三少GG](https://me.csdn.net/scut1135)阅读数：1209

参考文献：
1. [一起来学OpenMP（1）——初体验](http://blog.csdn.net/donhao/article/details/5651156)**系列好文，精读！ 尤其一起来学OpenMP（4）——数据的共享与私有化**
2. [OpenMP编程指南](http://blog.csdn.net/drzhouweiming/article/details/4093624)**系列好文，精读！**
当前多核多线程CPU大行其道，如果不能充分利用岂不是太可惜了！特别在图像处理领域，简直是为并行计算而生的！在网上看了不少文章，还是自己总结一下吧。 
**一  VS2008中怎样开启OpenMP编程？**
      Windows平台下，在VS系列编译器中，要想使用OpenMP，只需在工程设置中把C/C++ --> Language --> OpenMP Support设为Yes就可以了。在运行时可能会提示找不到运行库的情况，只要把vcomp90d.dll以及vcomp90.dll复制到工程目录，或系统的Path变量指向的目录下就可以了。在VS2008自带的OpenMP C/C++开发库是2.0版，发行日期为2002年，当前最新的版本为3.1版，可以到官方网站www.openmp.org上下载相关。
**二 开始体验并行吧！**
     要使程序并行化，没有想象中的那么复杂，OpenMP让一切变得很简单，第一个OpenMP程序，只需添加一行代码！看下面的代码：     
**[cpp]**[view plain](http://blog.csdn.net/wyjkk/article/details/6612108#)[copy](http://blog.csdn.net/wyjkk/article/details/6612108#)[print](http://blog.csdn.net/wyjkk/article/details/6612108#)[?](http://blog.csdn.net/wyjkk/article/details/6612108#)
- #include <iostream>
- 
- usingnamespace std;  
- int main()  
- {  
- #pragma omp parallel  
- for (int i=0; i<10; i++)   
-     {  
-         cout << i;  
-     }   
- 
- return 0;  
- }  
就是这一行代码，#pragma omp parallel， 至于#pragma是什么如果不明白的话，建议GOOGLE一下， omp表示这个指令是OpenMP的指令，事实上所有OpenMP的指令都带有omp关键字
在我的机器上4核CPU，运行结果：0000111122223333444455556666777788889999
咋一看程序好像抽风了，产生了4个线程同时执行了for循环。通常这不是我们想要的，我们想要的是把for中的任务等分成4份，分别由4个线程各执行其中的一份。这样做其实很简单，只要在parallel后面加上for就可以了。
**[cpp]**[view plain](http://blog.csdn.net/wyjkk/article/details/6612108#)[copy](http://blog.csdn.net/wyjkk/article/details/6612108#)[print](http://blog.csdn.net/wyjkk/article/details/6612108#)[?](http://blog.csdn.net/wyjkk/article/details/6612108#)
- #include <iostream>
- 
- usingnamespace std;  
- int main()  
- {  
- #pragma omp parallel for
- for (int i=0; i<10; i++)   
-     {  
-         cout << i;  
-     }   
- 
- return 0;  
- }  
现在的运行结果是：3068417952。什么？乱七八糟的！然而这是正确的，因为在同一段并行代码中，我们并不能保证各线程执行的先后顺序。扩散开来，在并行代码中，各线程的工作不能有依赖性，比如如果一个线程的输入和另一个线程的输出相依赖，那么此程序不适合并行计算。
**三 OpenMP API函数**
     打开VS2008自带的omp.h，可以发现OpenMP的一些API函数。先从最简单的说起，都很好理解。没有写中文注释的放在后面讲解。
**[cpp]**[view plain](http://blog.csdn.net/wyjkk/article/details/6612108#)[copy](http://blog.csdn.net/wyjkk/article/details/6612108#)[print](http://blog.csdn.net/wyjkk/article/details/6612108#)[?](http://blog.csdn.net/wyjkk/article/details/6612108#)
- // 设置并行线程数
- _OMPIMP void _OMPAPI omp_set_num_threads(int _Num_threads);  
- 
- // 获取当前并行线程数
- _OMPIMP int  _OMPAPI omp_get_num_threads(void);  
- 
- // 获取当前系统最大可并行运行的线程数
- _OMPIMP int  _OMPAPI omp_get_max_threads(void);  
- 
- // 获取当前运行线程的ID，注意和操作系统中的线程ID不同
- _OMPIMP int  _OMPAPI omp_get_thread_num(void);  
- 
- 
- // 获取当前系统中处理器数目
- _OMPIMP int  _OMPAPI omp_get_num_procs(void);  
- 
- _OMPIMP void _OMPAPI omp_set_dynamic(int _Dynamic_threads);  
- 
- _OMPIMP int  _OMPAPI omp_get_dynamic(void);  
- 
- _OMPIMP int  _OMPAPI omp_in_parallel(void);  
- 
- _OMPIMP void _OMPAPI omp_set_nested(int _Nested);  
- 
- _OMPIMP int  _OMPAPI omp_get_nested(void);  
- 
- _OMPIMP void _OMPAPI omp_init_lock(omp_lock_t * _Lock);  
- 
- _OMPIMP void _OMPAPI omp_destroy_lock(omp_lock_t * _Lock);  
- 
- _OMPIMP void _OMPAPI omp_set_lock(omp_lock_t * _Lock);  
- 
- _OMPIMP void _OMPAPI omp_unset_lock(omp_lock_t * _Lock);  
- 
- _OMPIMP int  _OMPAPI omp_test_lock(omp_lock_t * _Lock);  
- 
- _OMPIMP void _OMPAPI omp_init_nest_lock(omp_nest_lock_t * _Lock);  
- 
- _OMPIMP void _OMPAPI omp_destroy_nest_lock(omp_nest_lock_t * _Lock);  
- 
- _OMPIMP void _OMPAPI omp_set_nest_lock(omp_nest_lock_t * _Lock);  
- 
- _OMPIMP void _OMPAPI omp_unset_nest_lock(omp_nest_lock_t * _Lock);  
- 
- _OMPIMP int  _OMPAPI omp_test_nest_lock(omp_nest_lock_t * _Lock);  
- 
- _OMPIMP double _OMPAPI omp_get_wtime(void);  
- 
- _OMPIMP double _OMPAPI omp_get_wtick(void);  
**四 for循环**
在(二)中已经给出了一个for循环并行化的例子。其实OpenMP中for循环并行化指令有两种形式：
**[cpp]**[view plain](http://blog.csdn.net/wyjkk/article/details/6612108#)[copy](http://blog.csdn.net/wyjkk/article/details/6612108#)[print](http://blog.csdn.net/wyjkk/article/details/6612108#)[?](http://blog.csdn.net/wyjkk/article/details/6612108#)
- #include <iostream>  
- #include <omp.h>  
- 
- usingnamespace std;    
- 
- int main()    
- {    
- //for循环并行化声明形式1  
- #pragma omp parallel   
-     {    
-         cout << "OK" << endl;    
- #pragma omp for   
- for (int i = 0; i < 4; ++i)    
-         {    
-             cout << i << endl;    
-         }    
-     }    
- 
- //for循环并行化声明形式2  
- #pragma omp parallel for  
- //cout << "ERROR" << endl; 
- for (int j = 0; j < 4; ++j)    
-     {    
-         cout << j << endl;    
-     }    
- return 0;    
- }    
形式1和2相比，其优点是在for循环体前可以有其他执行代码，当然在一个#pragma omp parallel块内，可以有多个#pragma omp parallel for循环体。
**for循环并行化的约束条件**
尽管OpenMP可以方便地对for循环进行并行化，但并不是所有的for循环都可以进行并行化。以下几种情况不能进行并行化：
1. for循环中的循环变量必须是有符号整形。例如，for (unsigned int i = 0; i < 10; ++i){}会编译不通过；
2. for循环中比较操作符必须是<, <=, >, >=。例如for (int i = 0; i ！= 10; ++i){}会编译不通过；
3. for循环中的第三个表达式，必须是整数的加减，并且加减的值必须是一个循环不变量。例如for (int i = 0; i != 10; i = i + 1){}会编译不通过；感觉只能++i; i++; --i; 或i--；
4. 如果for循环中的比较操作为<或<=，那么循环变量只能增加；反之亦然。例如for (int i = 0; i != 10; --i)会编译不通过；
5. 循环必须是单入口、单出口，也就是说循环内部不允许能够达到循环以外的跳转语句，exit除外。异常的处理也必须在循环体内处理。例如：若循环体内的break或goto会跳转到循环体外，那么会编译不通过。
下面来看看在并行程序中各个线程工作的分配，运行下面的程序：
**[cpp]**[view plain](http://blog.csdn.net/wyjkk/article/details/6612108#)[copy](http://blog.csdn.net/wyjkk/article/details/6612108#)[print](http://blog.csdn.net/wyjkk/article/details/6612108#)[?](http://blog.csdn.net/wyjkk/article/details/6612108#)
- void TestAPIs()  
- {  
-     cout << "Num of Procs: " << omp_get_num_procs() << endl;  
-     cout << "Max Threads: " << omp_get_max_threads() << endl;  
-     cout << "Set Num of Threads = 2 " << endl;  
-     omp_set_num_threads(2);  
- 
-     #pragma omp parallel
-     cout << "Get Thread Num: " << omp_get_thread_num() << endl;  
- 
- 
-     omp_set_num_threads(omp_get_num_procs() -1);  
-     #pragma omp parallel 
-     {  
-         cout << "OPENMP\n" ;   
-     }     
- }  
输出结果为：
ThreadID = 0; i = 0
ThreadID = 0; i = 1
ThreadID = 1; i = 4
ThreadID = 2; i = 8
ThreadID = 3; i = 12
ThreadID = 0; i = 2
ThreadID = 1; i = 5
ThreadID = 2; i = 9
ThreadID = 3; i = 13
ThreadID = 0; i = 3
ThreadID = 1; i = 6
ThreadID = 2; i = 10
ThreadID = 3; i = 14
ThreadID = 1; i = 7
ThreadID = 2; i = 11
ThreadID = 3; i = 15
从输出结果中可以清晰地看出，线程0处理了第0~3次循环，线程1处理了第4~7次循环，线程2处理了第8~1次循环，线程3处理了第12~15次循环，可见for循环的工作量被分成4等份分别交给每一个线程运行完成。
**五  数据的可见性**
**六 一个有意义的并行程序**
     在(一)中我们已经完成了第一个并行的程序，但是没有太大意义，下面我们通过一个例子来实现一个有意义的并行程序。
**[cpp]**[view plain](http://blog.csdn.net/wyjkk/article/details/6612108#)[copy](http://blog.csdn.net/wyjkk/article/details/6612108#)[print](http://blog.csdn.net/wyjkk/article/details/6612108#)[?](http://blog.csdn.net/wyjkk/article/details/6612108#)
- #include <iostream>
- #include <math.h>
- #include <omp.h>
- 
- usingnamespace std;  
- int main()  
- {  
- constint NUMBER = 100;  
- int* dataA = newint[NUMBER];  
- int* dataB = newint[NUMBER];  
- for (int i= 0; i < NUMBER; i++)  
-     {  
-         dataA[i] = i+1;  
-         dataB[i] = 2*(i+1);  
-     }      
- longdouble sum = 0.0;  
- 
-     omp_set_num_threads(4);  
- #pragma omp parallel for reduction(+:sum)
- for (int i = 0; i < NUMBER;  i++)   
-     {  
-         sum += dataA[i] + dataB[i];  
-     }     
-     cout<<sum<<endl;  
- 
- delete [] dataA;  
- delete [] dataB;  
- 
- return 0;  
- }  
上面代码中我们首先开辟了2块的缓冲区，长度均为100个int，并且初始化为1，2，3~100和2，4，6，8~200。在并行段执行前，设置线程数为4。接下来是并行指令：
**[cpp]**[view plain](http://blog.csdn.net/wyjkk/article/details/6612108#)[copy](http://blog.csdn.net/wyjkk/article/details/6612108#)[print](http://blog.csdn.net/wyjkk/article/details/6612108#)[?](http://blog.csdn.net/wyjkk/article/details/6612108#)
- #pragma omp parallel for reduction(+:sum)
指令中#pragma omp parallel for上文已经讲了，而后面reduction(+:sum)起什么左右的呢？要解决这个问题，首先读懂for循环中代码，可以看出只不过是把dataA和dataB中的元素相加，然后赋值给sum，这样循环结束后
