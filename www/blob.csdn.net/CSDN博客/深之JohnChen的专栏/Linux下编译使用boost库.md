# Linux下编译使用boost库 - 深之JohnChen的专栏 - CSDN博客

2018年03月20日 15:06:43[byxdaz](https://me.csdn.net/byxdaz)阅读数：688



     Boost库是一个可移植、提供源代码的C++库，作为标准库的后备，是C++标准化进程的开发引擎之一。 Boost库由C++标准委员会库工作组成员发起，其中有些内容有望成为下一代C++标准库内容。在C++社区中影响甚大，是不折不扣的“准”标准库。Boost由于其对跨平台的强调，对标准C++的强调，与编写平台无关。大部分boost库功能的使用只需包括相应头文件即可，少数（如正则表达式库，文件系统库等）需要链接库。

        怎样在Linux下编译使用Boost的所有模块.

1. 先去[Boost官网](http://www.boost.org/)下载最新的Boost版本, 我下载的是boost_1_56_0版本, 解压.

2. 进入解压后目录: cd boost_1_56_0, 执行下面的命令:

`$ ./bootstrap.sh --prefix=path/to/installation/prefix`

**prefix**的值是你希望安装boost的路径, 不开启此参数的话默认安装在 /usr/local 下. 我安装在 /home/xzz/boost_1_56_0目录下:

`$ ./bootstrap.sh --prefix=/home/johnchen/boost_1_56_0`

**Note:**家目录不要用 ~ 表示, 编译脚本不识别 ~, 会在当前目前新建一个名为 '~' 的目录.

接着执行:

`$ ./b2 install`

这条命令把boost的头文件文件夹 include/ 安装在prefix定义的目录中, 并且会编译所有的boost模块, 并将编译好的库文件夹 lib/ 也放在prefix定义的目录中. 所有如果成功编译的的话, prefix目录即 /home/johnchen/boost_1_56_0目录应当包含有 include/ 和 lib/ 两个文件夹.

3. 测试

//test-boost.cpp

```cpp
#include <stdio.h>
#include <iostream>  
#include <boost/filesystem.hpp>

using namespace boost::filesystem;
int main(int argc, char *argv[])  
{  
	    if (argc < 2) 
	    {  
        std::cout << "Usage: tut1 path\n";  
        return 1;  
    	}  
    	
 		std::cout << argv[1] << " " << file_size(argv[1]) << std::endl;  
    return 0;  

}
```

编译命令

g++ test-boost.cpp -o test -I /home/johnchen/boost_1_56_0/include -L /home/johnchen/boost_1_56_0/lib -lboost_filesystem -lboost_system

执行 ./test, 这个时候会出现一个问题:

 ./test: error while loading shared libraries: libboost_system.so.1.56.0: cannot open shared object file: No such file or directory

原因是在存在动态库和静态库时, gcc优先使用动态库, 但动态库在linux下默认搜索路径是/lib, /usr/lib, /usr/local/lib. 所以程序运行的时候出错. 解决办法可以将动态库拷贝到动态库的搜索路径下。也可以指定静态库，使用下面命令就可以编译。

g++ test-boost.cpp -o test -I /home/johnchen/boost_1_56_0/include -L /home/johnchen/boost_1_56_0/lib -l:libboost_filesystem.a -l:libboost_system.a

//线程demo

```cpp
#include <boost/thread.hpp>
#include <iostream>

void task1() { 
    // do stuff
    std::cout << "This is task1!" << std::endl;
}

void task2() { 
    // do stuff
    std::cout << "This is task2!" << std::endl;
}

int main (int argc, char ** argv) {
    using namespace boost; 
    thread thread_1 = thread(task1);
    thread thread_2 = thread(task2);
       
    //sleep
    boost::this_thread::sleep(boost::posix_time::seconds(2));

    // do other stuff
    thread_2.join();
    thread_1.join();
    return 0;
}
```

g++ boost2.cpp -o test2 -I /home/johnchen/boost_1_56_0/include -L /home/johnchen/boost_1_56_0/lib -lboost_system -lboost_thread

编译之后会出现一个 example 的可执行文件，可以运行：./example ， 结果显示：

This is task2!

This is task1!

可能你在运行时会出现这样的错误：error while loading shared libraries: libboost_system.so.1.56.0: cannot open shared object file: No such file or directory

这是因为要用到的动态库不在默认的环境变量里，可以使用下面的命令添加：

export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH

添加后，再执行./test2

也可以使用指定静态库去链接生成目标文件。命令如下：

g++ boost2.cpp -o test2 -I /home/johnchen/boost_1_56_0/include -L /home/johnchen/boost_1_56_0/lib -l:libboost_system.a -l:libboost_date_time.a -l:libboost_thread.a

发现是在链接的时候出错。

/home/johnchen/boost_1_56_0/lib/libboost_thread.a(thread.o): In function `boost::detail::(anonymous namespace)::create_current_thread_tls_key()':

thread.cpp:(.text+0x13): undefined reference to `pthread_key_create'

经过查找发现clock_gettime在实时库（real time）里面，由于链接的时候没有链接这个库导致报错。

/home/johnchen/boost_1_56_0/lib/libboost_thread.a(thread.o): In function `boost::this_thread::no_interruption_point::hiden::sleep_until(timespec const&)':

thread.cpp:(.text+0x7ab): undefined reference to `clock_gettime'

g++ boost2.cpp -o test2 -I /home/johnchen/boost_1_56_0/include -L /home/johnchen/boost_1_56_0/lib -l:libboost_system.a -l:libboost_date_time.a -l:libboost_thread.a -lrt

发现是在链接的时候出错。经过查找发现clock_gettime在实时库（real time）里面，由于链接的时候没有链接这个库导致报错。

解决方案：

      只需在我们编译的时候加上-lrt即可。

-lz -lrt -lm -lc都是什么库

libz      压缩库（Z）

librt      实时库（real time）

libm      数学库（math）

libc      标准C库（C lib）

