# dmalloc arm-linux平台使用 - DoubleLi - 博客园






话说“工欲善其事，必先得其器”，用C语言写程序，最怕遇到个什么内存泄漏，内存越界访问了，心里那个急啊。。。

如果在i368-linlux上，valgrind工具是首选，但在arm-linux平台上，如何呢，dmalloc就是一个不错的选择。当然,IBM的purify是另外一个级别的选手，不提也罢。



1.　到官方网站下载dmalloc: [www.dmalloc.com](http://www.dmalloc.com/)

2. 交叉编译dmalloc

2.1 configure

因为configure过程中要在PC上执行test程序，而交叉编译的test程序在PC上无法运行，导致configure通不过，因此，我没有加上--host=arm-linux选项

./configure --prefix=/opt/crosstool/arm-linux/gcc-3.4.4-glibc-2.3.5/arm-linux/arm-linux/ --enable-cxx --enable-threads

其中opt/crosstool/arm-linux/gcc-3.4.4-glibc-2.3.5/arm-linux/arm-linux/是我的交叉编译环境的目录

2.2

修改环境变量，偷梁换柱

export PATH=/opt/crosstool/arm-linux/gcc-3.4.4-glibc-2.3.5/arm-linux/arm-linux/bin/:$PATH

其中交叉编译工具在/opt/crosstool/arm-linux/gcc-3.4.4-glibc-2.3.5/arm-linux/arm-linux/bin/下，且名字为gcc, g++等。

这时，用which gcc命令看到的将是交叉编译工具gcc



2.3

make threadscxx

make install

 这样，就编译出了支持线程和c++的dmalloc版本。



3. 环境配置

将交叉编译后的dmalloc放入嵌入式板子上，执行./dmalloc -b -l logfile -i 100 low

会看到有两行输出：

DMALLOC_OPTIONS=debug=0x4e48503,inter=100,log=logfile

export DMALLOC_OPTIONS

把这两行作为shell脚本执行一下，即是设置环境变量



4. 写代码测试dmalloc啦

简单的程序：

#include <signal.h>
#include <unistd.h>
#include <cstdlib>

int main()
{

    char *p = (char *)malloc(10);

    for(int i = 0; i < 11; ++i)
        p[i] = 'a';

    return 0;
}

上面的程序有明显的内存越界访问。

用如下命令编译之：

$ arm-linux-g++ try.cpp -DDMALLOC_FUNC_CHECK -ldmalloc

生成a.out



在板子上执行./a.out

会有logfile产生，内容如下：

948436802: 1: Dmalloc version '5.5.2' from 'http://dmalloc.com/'
948436802: 1: flags = 0x4e48503, logfile 'logfile'
948436802: 1: interval = 100, addr = 0, seen # = 0, limit = 0
948436802: 1: starting time = 948436802
948436802: 1: process pid = 5091
948436802: 1:   error details: checking user pointer
948436802: 1:   pointer '0x40016fe8' from 'unknown' prev access 'try.cpp:11'
948436802: 1:   dump of proper fence-top bytes: 'i\336\312\372'
948436802: 1:   dump of '0x40016fe8'-6: '\300\300\033\253\300\300aaaaaaaaaaa\336\312\372'
948436802: 1:   next pointer '0x40017000' (size 0) may have run under from 'unknown'
948436802: 1: ERROR: _dmalloc_chunk_heap_check: failed OVER picket-fence magic-number check (err 27)

哈哈，OK

从上面的输出可以看到，在访问try.cpp 第11行分配的内存时，出现了越界访问。



5. dmalloc也支持c++，但不完善，打印不出出错的行号

用c++时，要用-ldmallocthcxx选项，不能用-ldmalloc

其实没行号也没关系，用gdb工具及打印出的地址也是可以知道行号的。









