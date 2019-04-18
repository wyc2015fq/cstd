# linux c 写的一个电子账簿 - YuDale的博客 - CSDN博客
2015年10月20日 19:15:51[YuDale](https://me.csdn.net/YuDale)阅读数：410
两个阶段
1.用户管理系统，其中包括：用户登录，用户注册，用户信息打印三个功能
2.添加账簿功能，其中包括：记账，查账
其中用到了结构体，文件的打开/读写/关闭的操作，写makefile文件
主菜单
![](https://img-blog.csdn.net/20151020193754199?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注册界面
![](https://img-blog.csdn.net/20151020194004564?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注册失败界面
![](https://img-blog.csdn.net/20151020194713509?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20151020195055356?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
登陆界面
![](https://img-blog.csdn.net/20151020200954047?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
登陆失败界面
![](https://img-blog.csdn.net/20151020201154216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20151020201244315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
打印信息界面
![](https://img-blog.csdn.net/20151020201340794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
记账界面
![](https://img-blog.csdn.net/20151020201618565?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)0
查账界面
![](https://img-blog.csdn.net/20151020202341636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
先就只贴图吧
有些困难的方面：
文件的操作
int fd=open("filename"，flags,...)
**filename : 文件名（包含路径）**
**flags :     O_RDONLY          只读方式打开**
**               O_WRONLY         只写方式打开**
**               O_RDWR              读写方式打开**
**一下三个必选一个**
**               O_CREAT              文件存在就打开，不存在则创建**
**               O_TRUNC             打开文件自动清空文件**
**O_EXCL                 文件不存在就创建，若存在，返回-1代表出错**
**               O_APPEND           采用追加方式打开文件，一般用于写**
**... :          第三个参数，只用于新建文件时，传入文件在硬盘上的权限（rwx）**
**               r----4(100)**
**               w---2(010)**
**x---1(001)**
**返回值 ：  打开（创建）文件成功则返回正整数，否则返回-1.错误信息在errno里**
**ssize_t write ( int  fd , const  void  *buf , size_t  count )**
**fd：文件描述符**
**文件描述符：本质上就是一个数字（编号），当进程启动时****系统自动为进程**
**                    打开3个文件：**
**标准输入（键盘），标准输出（显示器），标准出错（显示器）**
**标C：          stdin                      stdout                      stderr**
**UC：          STDIN_FILENO       STDOUT_FILENO      STDERR_FILENO**
**对应的数字：0                           1                                2**
**所以这时再打开一个文件，fd就是3，再打开一个，就是4****一个进程可以打开文件的个数是有限的**
**ulimit  -n**
**文件权限掩码：umask（可以通过命令或编程的方式修改）**
**buf：     源数据的缓冲区**
**count： 期望写入的字节数**
**返回值：成功时返回所写入的字节数；****返回0，说明没有写入****失败时返回-1 ，****错误原因在errno里**
**read与write类似**
**makefile文件**
**1.main:main.o login.o register.o tallt.o audit.o2.cc  main.c login.c register.c tallt.c audit.c3.main.o:main.c HeadFile.h4.cc -c main.c5.login.o:login.c HeadFile.h6.cc -c login.c7.register.o:register.c HeadFile.h8.cc -c register.c9.tallt.o:tallt.c HeadFile.h10.cc -c tallt.c11.audit.o:audit.c HeadFile.h12.cc -c audit.c**
**1.将**main.o login.o register.o tallt.o audit.o链接到main****
**2.编译**  main.c login.c register.c tallt.c audit.c****
****3.将**main.c HeadFile.h链接到main.o******
******4.编译生成main.c的.o文件******
******后面 类推******
**疑问**
**为什么makefile文件中第二行本来应该写.o的但是我写的.c同样可以**
**下面应该解决的**
**实现每天账目的总计**
**实现删除用户和修改账目的功能。**
