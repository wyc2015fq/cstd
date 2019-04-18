# 使用Boost_MPI进行并行编程 - wishchinYang的专栏 - CSDN博客
2013年09月14日 12:56:52[wishchin](https://me.csdn.net/wishchin)阅读数：1772
关于Boost的介绍：[http://www.boost.org/doc/libs/1_53_0/doc/html/mpi.html](http://www.boost.org/doc/libs/1_53_0/doc/html/mpi.html)
# 原文链接：[使用 Boost 的 IPC 和 MPI 库进行并发编程](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/)
**简介：** Boost `C++` 库让并发编程变得既简单又有趣。学习如何使用两个 Boost 库 —— Interprocess (IPC) 库和 Message Passing Interface (MPI) 实现共享内存对象、同步文件锁和分布式通信等功能。
使用非常流行的 Boost 库进行并发编程非常有意思。Boost 有几个用于并发编程领域的库：Interprocess (IPC) 库用于实现共享内存、内存映射的 I/O 和消息队列；Thread 库用于实现可移植的多线程；Message Passing Interface (MPI) 库用于分布式计算中的消息传递；Asio 库用于使用套接字和其他低层功能实现可移植的连网功能。本文介绍 IPC 和 MPI 库以及它们提供的一些功能。
本文中将学习如何使用 Boost IPC 库实现共享内存对象、消息队列和同步文件锁。通过使用 Boost MPI 库，了解 `environment` 和`communicator` 类，以及如何实现分布式通信。
**注意：**本文中的代码已经用 `gcc-4.3.4` 和 `boost-1.45` 包测试过了。
**常用缩写词**
- **API**：应用程序编程接口
- **I/O**：输入/输出
- **POSIX**：针对 UNIX 的便携式操作系统接口®
- **SDK**：软件开发工具包
使用 Boost IPC 库
Boost Interprocess 是一个只由头文件组成的库，所以您需要做的只是在自己的源代码中包含适当的头文件并让编译器知道 `include` 路径。这是一个非常好的特性；您只需下载 Boost 源代码（见[参考资料](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#resources) 中的链接），然后就可以开始使用了。例如，要想在自己的代码中使用共享内存，就使用[清单
 1](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list1) 所示的 `include`。
**清单 1. Boost IPC 库只由头文件组成**
```cpp
#include <boost/interprocess/shared_memory_object.hpp>
using namespace boost::interprocess; 
//… your sources follow …
```
在把信息传递给编译器时，您要求进程根据安装相应地修改 `include` 路径。然后，编译代码：
bash-4.1$  g++ ipc1.cpp –I../boost_1_45_0
创建共享内存对象
我们先从传统的 "Hello World!" 程序开始。有两个进程：第一个进程把字符串 "Hello World!" 写入内存，另一个进程读取并显示此字符串。像[清单 2](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list2) 这样创建共享内存对象。
```cpp
#include <boost/interprocess/shared_memory_object.hpp>
int main(int argc, char* argv[ ]) 
{
    using namespace using boost::interprocess; 
    try { 
    // creating our first shared memory object.
    shared_memory_object sharedmem1 (create_only, "Hello", read_write);
    // setting the size of the shared memory
    sharedmem1.truncate (256);
    // … more code follows
    } catch (interprocess_exception& e) { 
    // .. .  clean up 
    } 
}
```
`sharedmem1` 对象的类型是 `shared_memory_object`（在 Boost 头文件中声明并定义），它的构造函数有三个参数：
- 第一个参数 —` create_only `— 表示要创建这个共享内存对象而且还没有创建它。如果已经存在同名的共享对象，就会抛出异常。对于希望访问已经创建的共享内存的进程，第一个参数应该是`open_only`。
- 第二个参数 —` Hello `— 是共享内存区域的名称。另一个进程将使用这个名称访问这个共享内存。 
- 第三个参数 —` read_write `— 是共享内存对象的访问指示符。因为这个进程要修改共享内存对象的内容，所以使用 
```
read_write
```
。只从共享内存读取数据的进程使用 `read_only` 指示符。
`truncate` 方法以字节为单位设置共享内存的大小。最好把代码放在 `try-catch` 块中。例如，如果无法创建共享内存对象，就抛出类型为`boost::interprocess_exception` 的异常。
使用共享内存对象写数据
使用共享内存对象的进程必须在自己的地址空间中映射对象。使用在头文件 mapped_region.hpp 中声明并定义的 `mapped_region` 类执行映射。使用`mapped_region` 的另一个好处是可以对共享内存对象进行完全和部分访问。[清单
 3](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list3) 演示如何使用`mapped_region`。
```cpp
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
int main(int argc, char* argv[ ]) 
{
    using namespace boost::interprocess; 
    try { 
    // creating our first shared memory object.
    shared_memory_object sharedmem1 (create_only, "Hello", read_write);
    // setting the size of the shared memory
    sharedmem1.truncate (256);
   // map the shared memory to current process 
   mapped_region mmap (sharedmem1, 256); 
    // access the mapped region using get_address
    std::strcpy(static_cast<char* >(region.get_address()), "Hello World!\n");
    
    } catch (interprocess_exception& e) { 
    // .. .  clean up 
    } 
}
```
就这么简单。现在已经创建了您自己的 `mapped_region` 对象并使用 `get_address` 方法访问了它。执行了`static_cast`，因为`get_address` 返回一个`void*`。
当主进程退出时共享内存会怎么样？
当主进程退出时，并不删除共享内存。要想删除共享内存，需要调用 `shared_memory_object::remove`。第二个进程的访问机制也很简单：[清单 4](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list4) 证明了这一点。
```cpp
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstring>
#include <cstdlib>
#include <iostream>
int main(int argc, char *argv[ ])
{
      using namespace boost::interprocess; 
      try { 
      // opening an existing shared memory object 
      shared_memory_object sharedmem2 (open_only, "Hello", read_only);
      // map shared memory object in current address space
      mapped_region mmap (sharedmem2, read_only);
      // need to type-cast since get_address returns void* 
      char *str1 = static_cast<char*> (mmap.get_address());
      std::cout << str1 << std::endl;
      } catch (interprocess_exception& e) { 
          std::cout << e.what( ) << std::endl;
      } 
      return 0;
}
```
在清单 4 中，使用 `open_only` 和 `read_only` 属性创建共享内存对象。如果无法找到这个共享内存对象，就会抛出异常。现在，构建并运行[清单 3](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list3) 和[清单
 4](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list4) 中的代码。应该会在终端上看到 "Hello World!"。
接下来，在第二个进程的代码（[清单 4](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list4)）中`std::cout` 后面添加以下代码并重新构建代码：
// std::cout code here
shared_memory_object::remove("Hello");
// } catch(interprocess_exception& e) { 
连续执行代码两次，第二次执行会显示 "No such file or directory"，这证明共享内存已经被删除了。
使用消息队列实现进程间通信
现在，研究另一种流行的进程间通信机制：消息队列。每个参与通信的进程都可以在队列中添加消息和从队列读取消息。消息队列具有以下性质：
- 它有名称，进程使用名称访问它。
- 在创建队列时，用户必须指定队列的最大长度和一个消息的最大大小。
- 队列是持久的，这意味着当创建它的进程死亡之后它仍然留在内存中。可以通过显式地调用 `boost::interprocess::message_queue::remove` 删除队列。
在 [清单 5](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list5) 所示的代码片段中，进程创建了一个可包含 20 个整数的消息队列。
#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream> 
int main(int argc, char* argv[ ]) 
{
    using namespace boost::interprocess;
    try { 
      // creating a message queue
      message_queue mq (create_only,   // only create
                                       "mq",              // name
                                        20,                 //max message count
                                        sizeof(int)      //max message size
                                        );
       // … more code follows
    } catch (interprocess_exception& e) { 
       std::cout << e.what( ) << std::endl;
    } 
}
注意传递给 `message_queue` 的构造函数的 `create_only` 属性。与共享内存对象相似，对于以只读方式打开消息队列，应该把`open_only` 属性传递给构造函数。
发送和接收数据
在发送方，使用队列的 `send` 方法添加数据。`send` 方法有三个输入参数：原始数据的指针 (`void*`)、数据的大小和优先级。目前，以相同的优先级发送所有数据。[清单 6](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list6) 给出代码。
```cpp
#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream> 
int main(int argc, char* argv[ ]) 
{
    using namespace boost::interprocess;
    try { 
      // creating a message queue
      message_queue mq (create_only,   // only create
                                       "mq",              // name
                                        20,                 //max message count
                                        sizeof(int)      //max message size
                                        );
      // now send the messages to the queue
      for (int i=0; i<20; ++i) 
        mq.send(&i, sizeof(int), 0); // the 3rd argument is the priority 
    } catch (interprocess_exception& e) { 
        std::cout << e.what( ) << std::endl;
    } 
}
```
在接收方，使用 `open_only` 属性创建队列。通过调用 `message_queue` 类的 
```
receive
```
 方法从队列获取消息。[清单 7](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list7) 给出`receive` 的方法签名。
void receive (void *buffer,           
                      std::size_t buffer_size, 
                      std::size_t &recvd_size,
                      unsigned int &priority
                     );
我们来仔细看一下。第一个参数是从队列接收的数据将被存储到的位置。第二个参数是接收的数据的预期大小。第三个参数是接收的数据的实际大小。第四个参数是接收的消息的优先级。显然，如果在执行程序期间第二个和第三个参数不相等，就是出现错误了。[清单 8](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list8) 给出接收者进程的代码。
```cpp
#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream> 
int main(int argc, char* argv[ ]) 
{
    using namespace boost::interprocess;
    try { 
      // opening the message queue whose name is mq
      message_queue mq (open_only,     // only open
                                       "mq"               // name
                                        );
      size_t recvd_size; 
      unsigned int priority; 
      // now send the messages to the queue
      for (int i=0; i<20; ++i) { 
        int buffer; 
        mq.receive ((void*) &buffer, sizeof(int), recvd_size, priority); 
        if (recvd_size != sizeof(int)) 
            ; // do the error handling
        std::cout << buffer << " " << recvd_size << " " << priority;
      } 
    } catch (interprocess_exception& e) { 
        std::cout << e.what( ) << std::endl;
    } 
}
```
这相当简单。注意，仍然没有从内存中删除消息队列；与共享内存对象一样，这个队列是持久的。要想删除队列，应该在使用完队列之后添加以下行：
message_queue::remove("mq"); // remove the queue using its name
消息优先级
在发送方，做 [清单 9](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list9) 所示的修改。接收方代码不需要修改。
  message_queue::remove("mq"); // remove the old queue
      message_queue mq (…); // create as before
      for (int i=0; i<20; ++i) 
        mq.send(&i, sizeof(int), i%2); // 第 3 个参数为消息的优先级
      // … rest as usual
再次运行代码时，应该会看到 [清单 10](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list10) 所示的输出。
```cpp
1 4 1
3 4 1
5 4 1
7 4 1
9 4 1
11 4 1
13 4 1
15 4 1
17 4 1
19 4 1
0 4 0
2 4 0
4 4 0
6 4 0
8 4 0
10 4 0
12 4 0
14 4 0
16 4 0
18 4 0
```
[清单 10](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list10) 证实，第二个进程优先接收优先级高的消息。
同步对文件的访问
共享内存和消息队列很不错，但是文件 I/O 也是重要的进程间通信工具。对并发进程用于通信的文件访问进行同步并非易事，但是 Boost IPC 库提供的文件锁功能让同步变得简单了。在进一步解释之前，来看一下[清单 11](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list11)，了解`file_lock`
 对象是如何工作的。
#include <fstream> 
#include <iostream> 
#include <boost/interprocess/sync/file_lock.hpp> 
#include <cstdlib>
int main() 
{ 
    using namespace boost::interprocess; 
    std::string fileName("test"); 
    std::fstream file;
    file.open(fileName.c_str(), std::ios::out | std::ios::binary | 
std::ios::trunc); 
    if (!file.is_open() || file.bad()) 
    { 
        std::cout << "Open failed" << std::endl; 
        exit(-1); 
    }
    try { 
    file_lock f_lock(fileName.c_str());
    f_lock.lock();
    std::cout << "Locked in Process 1" << std::endl;
    file.write("Process 1", 9);
    file.flush(); 
    f_lock.unlock();
    std::cout << "Unlocked from Process 1" << std::endl;
    } catch (interprocess_exception& e) { 
    std::cout << e.what( ) << std::endl;
    }
    file.close();
    return 0; 
代码首先打开一个文件，然后使用 `file_lock` 锁定它。写操作完成之后，它刷新文件缓冲区并解除文件锁。使用 `lock` 方法获得对文件的独占访问。如果另一个进程也试图对此文件进行写操作并已经请求了锁，那么它会等待，直到第一个进程使用`unlock` 自愿地放弃锁。`file_lock` 类的构造函数接受要锁定的文件的名称，一定要在调用`lock` 之前打开文件；否则会抛出异常。
现在，复制 [清单 11](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list11) 中的代码并做一些修改。具体地说，让第二个进程请求这个锁。[清单 12](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list12) 给出相关修改。
```cpp
// .. as in Listing 11
    file_lock f_lock(fileName.c_str());
    f_lock.lock();
    std::cout << "Locked in Process 2" << std::endl;
    system("sleep 4"); 
    file.write("Process 2", 9);
    file.flush(); 
    f_lock.unlock();
    std::cout << "Unlocked from Process 2" << std::endl;
    // file.close();
```
现在，如果这两个进程同时运行，有 50% 的机会看到第一个进程等待 4 秒后才获得 `file_lock`，其他情况都不变。
在使用 `file_lock` 时，必须记住几点。这里讨论的主题是进程间通信，重点在*进程* 上。这意味着，不是使用 
```
file_lock
```
 来同步同一进程中各个线程的数据访问。在与 POSIX 兼容的系统上，文件句柄是进程属性，*而不是* 线程属性。下面是使用文件锁的几条规则：
- 对于每个进程，每个文件使用一个 `file_lock` 对象。
- 使用相同的线程来锁定和解锁文件。
- 在解锁文件之前，通过调用 `C` 的 `flush` 库例程或 `flush` 方法（如果喜欢使用`C++ fstream` 的话），刷新写入者进程中的数据。
结合使用 file_lock 和有范围（scope）的锁
在执行程序时，可能会出现抛出异常而文件没有解锁的情况。这种情况可能会导致意外的程序行为。为了避免这种情况，可以考虑把 `file_lock` 对象放在（boost/interprocess/sync/scoped_lock.hpp 中定义的）`scoped_lock` 中。如果使用`scoped_lock`，就不需要显式地锁定或解锁文件；锁定发生在构造器内，每当您退出该范围，就会自动发生解锁。[清单
 13](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list13) 给出对[清单 11](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list11) 的修改，使之使用有范围的锁。
**清单 13. 结合使用 scoped_lock 和 file_lock**
				
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
//… code as in Listing 11
file_lock f_lock(fileName.c_str());
scoped_lock<file_lock> s_lock(f_lock);  // internally calls f_lock.lock( ); 
// No need to call explicit lock anymore
std::cout << "Locked in Process 1" << std::endl;
file.write("Process 1", 9);
// … code as in Listing 11
**注意：**关于 Resource Acquisition Is Initialization (RAII) 编程习惯法的更多信息，参见[参考资料](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#resources) 中的链接。
[回页首](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#ibm-pcon)
了解 Boost MPI
如果您不熟悉 Message Passing Interface，那么在讨论 Boost MPI 之前，应该先浏览 [参考资料](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#resources) 中提供的 MPI 参考资料。MPI 是一个容易使用的标准，它采用通过传递消息实现进程间通信的模型。不需要使用套接字或其他通信原语；MPI 后端管理所有底层处理。那么，使用 Boost MPI 有什么好处？Boost MPI 的创建者提供了更高层的抽象，并在 MPI 提供的 API 之上构建了一套简单的例程，比如`MPI_Init` 和`MPI_Bcast`。
Boost MPI 不是一个单独的库，不能在下载和构建之后直接使用。相反，必须安装任意 MPI 实现（比如 MPICH 或 Open MPI）并构建 Boost Serialization 库。关于如何构建 Boost MPI 的详细信息参见[参考资料](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#resources)。通常，使用以下命令构建
 Boost MPI：
bash-4.1$ bjam –with-mpi
Windows® 用户可以从 BoostPro 下载预先构建的 MPI 库（见 [参考资料](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#resources)）。这些库与 Microsoft® HPC Pack 2008 和 2008 R2 兼容（见 [参考资料](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#resources)），适用于带 Service Pack 3 的 Windows XP 或更高版本的客户机操作环境。
[回页首](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#ibm-pcon)
用 MPI 实现 Hello World 程序
您必须了解 Boost MPI 库中的两个主要类：`environment` 类和 `communicator` 类。前者负责分布式环境的初始化；后者用于进程之间的通信。因为这里讨论的是分布式计算，我们有四个进程，它们都在终端上输出 "Hello World"。[清单
 14](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list14) 给出代码。
```cpp
#include <boost/mpi.hpp>
#include <iostream>
int main(int argc, char* argv[])
{
  boost::mpi::environment env(argc, argv);
  boost::mpi::communicator world;
  std::cout << argc << std::endl;
  std::cout << argv[0] << std::endl;
  std::cout << "Hello World! from process " << world.rank() << std::endl;
  return 0;
}
```
现在，构建 [清单 14](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list14) 中的代码并链接 Boost MPI 和 Serialization 库。在 shell 提示上运行可执行程序。应该会看到 "Hello World! from process 0"。接下来，使用 MPI 分派器工具（例如，对于 Open MPI 用户，使用`mpirun`；对于 Microsoft HPC Pack 2008，使用`mpiexec`）并运行可执行程序：
mpirun –np 4 <executable name> 
OR
mpiexec –n 4 <executable name>
现在应该会看到与 [清单 15](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list15) 相似的输出，其中的 *mympi1* 是可执行程序名称。
**清单 15. 运行 MPI 代码的输出**
				
1
mympi1
Hello, World! from process 3
1
mympi1
1
mympi1
Hello, World! from process 1
Hello, World! from process 2
1
mympi1
Hello, World! from process 0 
在 MPI 框架中，已经创建了相同进程的四个拷贝。在 MPI 环境中，每个进程有惟一的 ID（由 `communicator` 对象决定）。现在，试试进程间通信。使用`send` 和`receive` 函数调用让一个进程与另一个进程通信。发送消息的进程称为*主进程*，接收消息的进程称为*工作者进程*。主进程和接收者进程的源代码是相同的，使用`world` 对象提供的等级决定功能（见[清单
 16](http://www.ibm.com/developerworks/cn/aix/library/au-concurrent_boost/#list16)）。
```cpp
#include <boost/mpi.hpp>
#include <iostream>
int main(int argc, char* argv[]) 
{
  boost::mpi::environment env(argc, argv);
  boost::mpi::communicator world;
  if (world.rank() == 0) {
    world.send(1, 9, 32);
    world.send(2, 9, 33);
  } else { 
    int data;
    world.recv(0, 9, data);
    std::cout << "In process " << world.rank( ) << "with data " << data
                   << std::endl;
  } 
  return 0;
}
```
先看一下 `send` 函数。第一个参数是接收者进程的 ID；第二个是消息数据的 ID；第三个是实际数据。为什么需要消息标签？接收者进程在执行期间的特定点上可能希望处理具有特定标签的消息，所以这个方案会有帮助。对于进程 1 和 2，`recv` 函数被*阻塞*，这意味着程序会等待，直到从进程 0 收到标签 ID 为 9 的消息。当收到这个消息时，把信息存储在`data` 中。下面是运行代码的输出：
In process 1 with data 32
In process 2 with data 33
如果在接收方有 `world.recv(0, 1, data);` 这样的代码，会发生什么？代码阻塞，但实际上是，接收者进程在等待一个永远不会到达的消息。
结论 束语
本文只讨论了这两个库提供的功能的很小一部分。这些库提供的其他功能包括 IPC 的内存映射 I/O 和 MPI 的广播功能。从易用性的角度来说，IPC 更好。MPI 库依赖于原生的 MPI 实现，而原生 MPI 库以及预先构建的 Boost MPI 和 Serialization 库的现成可用性仍然是个问题。但是，花点儿精力构建 MPI 实现和 Boost 的源代码是值得的。
