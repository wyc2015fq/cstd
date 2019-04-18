# #pragma comment(lib,"ws2_32.lib")来告诉编译器在链接时要搜索的库文件 - ljx0305的专栏 - CSDN博客
2008年07月30日 11:57:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：3013
#pragma comment(lib,"ws2_32.lib")
#pragma   comment(lib,"Ws2_32.lib")表示链接Ws2_32.lib这个库。   
  和在工程设置里写上链入Ws2_32.lib的效果一样，不过这种方法写的   
  程序别人在使用你的代码的时候就不用再设置工程settings了。
 告诉连接器连接的时候要找ws2_32.lib，这样你就不用在linker的lib设置里指定这个lib了。
**ws2_32.lib**是winsock2的库文件
WinSock2就相当于连接系统和你使用的软件之间交流的一个接口,可能这个功能就是修复软件与系统正确的通讯的作用。
引用：[http://blog.csdn.net/qinmi/archive/2007/05/24/1624298.aspx](http://blog.csdn.net/qinmi/archive/2007/05/24/1624298.aspx)
