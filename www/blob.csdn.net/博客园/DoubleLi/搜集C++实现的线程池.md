# 搜集C++实现的线程池 - DoubleLi - 博客园






现在很多语言都内置了线程池实现，但C++中却没有。本文列举一些C++实现的线程池工具。



Boost.Threadpool

      项目首页：https://threadpool.sourceforge.net



      Boost.Threadpool是一个基于Boost、跨平台的C++线程池库。Boost.Threadpool提供了一个便捷的任务调度异步的途径。线程池可以定制，动态管理，易于集成到您的软件项目中。该库已经在多个商业服务器程序中使用，并没有任何问题的处理高负荷业务。



      使用示例:





复制代码

#include "threadpool.hpp"

void normal_task();

void important_task();

void execute_prioritized()

{

  // Create prioritized thread pool

  boost::threadpool::prio_pool tp; //scoped_pool<boost::threadpool::prio_pool, 0> tp;



  // Add some tasks to the pool.

  tp += boost::threadpool::prio_task_func(5,   &normal_task);

  tp += boost::threadpool::prio_task_func(100, &important_task);

  tp += boost::threadpool::prio_task_func(7,   &normal_task);



  // Add the some threads to the pool. This will start the execution of the tasks.

  tp->resize(2);



  // The tasks are processed according to their priority:

  // important_task(100), nonrelevant_task(7), nonrelevant_task(5).

  tp->wait();

  // Now all tasks are finished and the pool will be destroyed safely when tp goes out of scope.

}

复制代码

ffead-cpp

      项目首页：https://code.google.com/p/ffead-cpp。



      这是一个雄心勃勃的项目，它旨在将C++带入Web开发的世界。正像在它的介绍页面所声明的那样：



　　“该框架是为C++平台上快速发展的企业级应用而开发。 这是一个C++的Web框架，C++应用程序框架，C++的REST框架和C++的soap框架，这些框架都内置其中。 它包含[Linux](https://www.2cto.com/os/linux/)/Windows（通过Cygwin）的实现。它是第一个也是唯一的提供非侵入性的依赖注入、业务驱动的[组件](https://www.2cto.com/kf/all/zujian/)逻辑、基于POCO发展的C++应用程序框架。大部分的功能是由配置文件控制。”



POCO C++ libraries

      项目首页：https://pocoproject.org/。



      POCO库是一个使用现代标准ANSI C++以及C++标准库实现的框架。该框架近似完美实现（压缩，数据访问，SSL，[加密](https://www.2cto.com/article/jiami/)，XML，线程，IPC-任何你能想到的在[程序开发](https://www.2cto.com/kf)中的东西，它都包含）。它采用Boost授权许可证，而且似乎有很多的用户。



Linux下的一个ThreadPool

      项目首页：https://www.hlnum.org/english/projects/tools/threadpool/index.html。



      是一个非常简单的基于lib_pthread的实现。你可以基于它实现你的代码，而且很可能会按需进行一些进一步的修改。正如在其主页的介绍，它是：



“一个基于POSIX线程库的线程池实现。它实现的池是阻塞的，直到有一个job到来，然后它选择一个空闲线程，并在其上运行这个job。如果此时没有可用的空闲线程，该线程池会阻塞直到某一正在运行job的线程结束”。









