# ubuntu 安装更新gcc 4.9以上_用户5706034127_新浪博客
|||
14.01ubuntu 默认是gcc4.8. 安装osrm需要gcc4.9以上。
先给出gcc 和g++的区别，知乎的答案。
https://www.zhihu.com/question/20940822/answer/16667772
gcc 最开始的时候是 GNU C Compiler,
如你所知，就是一个c编译器。但是后来因为这个项目里边集成了更多其他不同语言的编译器，GCC就代表 the GNU Compiler
Collection，所以表示一堆编译器的合集。 g++则是GCC的c++编译器。
现在你在编译代码时调用的gcc，已经不是当初那个c语言编译器了，更确切的说他是一个驱动程序，根据代码的后缀名来判断调用c编译器还是c++编译器
(g++)。比如你的代码后缀是*.c，他会调用c编译器还有linker去链接c的library。如果你的代码后缀是cpp,
他会调用g++编译器，当然library call也是c++版本的。
CC则指的是一种编译器集合，GNU Compiler Collections，支持编译多种语言。不过现在有很多人将其误解为GNU的C编译器。。（比如很多ACM的OJ中，语言选GCC就意味着将代码作为.C文件编译）
G++就是GCC这个编译器集合中的C++编译器。有时候也被泛指为符合GNU C++编译器标准的C++代码。总之你在OJ中要交C++代码的话选它就ok了。

在就是更新gcc，
http://www.68idc.cn/help/server/linux/20160424611311.html
sudo apt-get install software-properties-common
1sudo add-apt-repository ppa:ubuntu-toolchain-r/test 2sudo apt-get update
1sudo apt-get upgrade 2sudo apt-get installgcc-4.8 g++-4.83sudo apt-get installgcc-4.9 g++-4.94sudo apt-get installgcc-5 g++-5
接着就是导出你刚刚安装的新版本gcc
